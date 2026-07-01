#include <web/http/HttpClient.hpp>
#include <httplib.h>
#include <queue>
#include <thread>
#include <condition_variable>

namespace dao::web {
    static httplib::Headers toHttplibHeaders(const Headers &headers) {
        httplib::Headers result;
        for (const auto &[key, value]: headers)
            result.emplace(key, value);
        return result;
    }

    /// @brief 固定大小线程池
    /// @details 停止协议: ~ThreadPool() 设 m_stop → notify_all → 成员逆序析构
    /// 成员声明顺序决定析构顺序，m_threads 在最后 → 最先销毁，join 时 mutex/cv 仍存活
    class ThreadPool {
    public:
        explicit ThreadPool(const size_t numThreads = 4) {
            for (size_t i = 0; i < numThreads; ++i) {
                m_threads.emplace_back([this](const std::stop_token &st) {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock lock(m_mutex);
                            m_cv.wait(lock, st, [this] {
                                return m_stop || !m_tasks.empty();
                            });
                            if ((m_stop && m_tasks.empty()) || st.stop_requested())
                                return;
                            task = std::move(m_tasks.front());
                            m_tasks.pop();
                        }
                        if (task) task();
                    }
                });
            }
        }

        ~ThreadPool() {
            {
                std::lock_guard lock(m_mutex);
                m_stop = true;
            }
            m_cv.notify_all();
        }

        void enqueue(std::function<void()> task) {
            {
                std::lock_guard lock(m_mutex);
                m_tasks.push(std::move(task));
            }
            m_cv.notify_one();
        }

    private:
        std::atomic<bool> m_stop{false};
        std::mutex m_mutex;
        std::condition_variable_any m_cv;
        std::queue<std::function<void()> > m_tasks;
        std::vector<std::jthread> m_threads; // 最后声明，最先析构（join 时依赖前面所有成员存活）
    };

    class HttpClient::Impl {
    public:
        /// @brief 统一 HTTP/HTTPS 底层客户端
        /// @details httplib::Client / SSLClient 无公共基类且不可拷贝，用 unique_ptr + variant 擦除类型差异
        using ClientVariant = std::variant<
            std::unique_ptr<httplib::Client>,
            std::unique_ptr<httplib::SSLClient>
        >;

        static ClientVariant makeClient(const std::string &host, int port, bool useHttps) {
            if (useHttps) {
                auto c = std::make_unique<httplib::SSLClient>(host, port);
                c->set_connection_timeout(5, 0);
                c->set_read_timeout(10, 0);
                c->enable_server_certificate_verification(false);
                return c;
            }
            auto c = std::make_unique<httplib::Client>(host, port);
            c->set_connection_timeout(5, 0);
            c->set_read_timeout(10, 0);
            return c;
        }

        Impl(const std::string &host, const int port, const bool useHttps)
            : m_host(host), m_port(port), m_isHttps(useHttps),
              m_client(makeClient(host, port, useHttps)) {
        }

        /// @brief 在 variant 上执行操作，visit 展开 unique_ptr 后透传引用给 f
        template<typename F>
        auto withClient(F &&f) {
            return std::visit([&](auto &c) { return f(*c); }, m_client);
        }

        /// @brief 同步执行 HTTP 请求，按 HttpMethod 分发到 httplib 对应方法
        HttpResponse doRequest(const HttpRequest &req) {
            HttpResponse resp;
            try {
                withClient([&](auto &c) {
                    httplib::Result res;
                    const auto hdrs = toHttplibHeaders(req.headers);
                    switch (req.method) {
                        case HttpMethod::Get:
                            res = c.Get(req.path, hdrs);
                            break;
                        case HttpMethod::Post:
                            res = c.Post(req.path, hdrs, req.body, req.content_type);
                            break;
                        case HttpMethod::Put:
                            res = c.Put(req.path, hdrs, req.body, req.content_type);
                            break;
                        case HttpMethod::Delete:
                            res = c.Delete(req.path, hdrs, req.body, req.content_type);
                            break;
                        case HttpMethod::Patch:
                            res = c.Patch(req.path, hdrs, req.body, req.content_type);
                            break;
                    }
                    if (res) {
                        resp.status = res->status;
                        resp.body = std::move(res->body);
                        resp.success = (res->status == 200);
                        if (res->status != 200)
                            resp.error = "HTTP Error: " + std::to_string(res->status);
                    } else {
                        resp.error = "Request failed: " + httplib::to_string(res.error());
                    }
                });
            } catch (const std::exception &e) {
                resp.error = std::string("Exception: ") + e.what();
            }
            return resp;
        }

        std::function<void(std::function<void()>)> getMainThreadHandler() const {
            std::lock_guard lock(m_mainThreadMutex);
            return m_mainThreadHandler;
        }

        ClientVariant m_client;
        std::string m_host;
        int m_port;
        bool m_isHttps;

        std::function<void(std::function<void()>)> m_mainThreadHandler;
        mutable std::mutex m_mainThreadMutex;
        ThreadPool m_pool{4}; // 最后声明，最先析构（join 时 client 还存活）
    };

    // ---- 构造 / 析构 ----

    HttpClient::HttpClient(const std::string &host, int port)
        : m_impl(std::make_unique<Impl>(host, port, port == 443)) {
    }

    HttpClient::~HttpClient() = default;

    HttpClient::HttpClient(HttpClient &&) noexcept = default;

    HttpClient &HttpClient::operator=(HttpClient &&) noexcept = default;

    /// @details 公开构造函数按 port==443 推断 SSL，不适用 scheme 场景。先占位构造再根据 parseHost 结果替换 Impl
    HttpClient HttpClient::fromURL(const std::string &url) {
        const auto parsed = parseHost(url);
        HttpClient client("", 80);
        client.m_impl = std::make_unique<Impl>(parsed.host, parsed.port, parsed.isHttps);
        return client;
    }

    /// @details 绕开公开构造函数的 port 推断，强制使用 SSL
    HttpClient HttpClient::https(const std::string &host, int port) {
        HttpClient client(host, port);
        client.m_impl = std::make_unique<Impl>(host, port, true);
        return client;
    }

    // ---- 同步请求 ----

    HttpResponse HttpClient::get(const std::string &path) const {
        return m_impl->doRequest(HttpRequest{.path = path});
    }

    HttpResponse HttpClient::request(const HttpRequest &req) const {
        m_impl->withClient([&](auto &c) {
            c.set_connection_timeout(req.connect_timeout_sec, 0);
            c.set_read_timeout(req.read_timeout_sec, 0);
        });
        return m_impl->doRequest(req);
    }

    // ---- 回调式异步请求 ----

    void HttpClient::get(const std::string &path, HttpCallback callback) {
        m_impl->m_pool.enqueue([this, path, cb = std::move(callback)]() mutable {
            auto resp = m_impl->doRequest(HttpRequest{.path = path});
            if (auto handler = m_impl->getMainThreadHandler()) {
                handler([cb = std::move(cb), resp = std::move(resp)]() { cb(resp); });
            } else {
                cb(resp);
            }
        });
    }

    void HttpClient::post(const std::string &path, const std::string &body,
                          HttpCallback callback, const std::string &content_type) {
        m_impl->m_pool.enqueue([this, path, body, content_type, cb = std::move(callback)]() mutable {
            auto resp = m_impl->doRequest(HttpRequest{
                .method = HttpMethod::Post,
                .path = path,
                .body = body,
                .content_type = content_type
            });
            if (auto handler = m_impl->getMainThreadHandler()) {
                handler([cb = std::move(cb), resp = std::move(resp)]() { cb(resp); });
            } else {
                cb(resp);
            }
        });
    }

    void HttpClient::post(const std::string &path, const Headers &headers,
                          const std::string &body, HttpCallback callback,
                          const std::string &content_type) {
        m_impl->m_pool.enqueue([this, path, headers, body, content_type, cb = std::move(callback)]() mutable {
            auto resp = m_impl->doRequest(HttpRequest{
                .method = HttpMethod::Post,
                .path = path,
                .headers = headers,
                .body = body,
                .content_type = content_type
            });
            if (auto handler = m_impl->getMainThreadHandler()) {
                handler([cb = std::move(cb), resp = std::move(resp)]() { cb(resp); });
            } else {
                cb(resp);
            }
        });
    }

    void HttpClient::request(const HttpRequest &req, HttpCallback callback) {
        m_impl->m_pool.enqueue([this, req, cb = std::move(callback)]() mutable {
            auto resp = m_impl->doRequest(req);
            if (auto handler = m_impl->getMainThreadHandler()) {
                handler([cb = std::move(cb), resp = std::move(resp)]() { cb(resp); });
            } else {
                cb(resp);
            }
        });
    }

    // ---- 主线程回调投递 ----

    void HttpClient::setMainThreadCallbackHandler(std::function<void(std::function<void()>)> handler) {
        std::lock_guard lock(m_impl->m_mainThreadMutex);
        m_impl->m_mainThreadHandler = std::move(handler);
    }

    // ---- SSL 配置 ----

    void HttpClient::setCaCertPath(const std::string &ca_cert_path) const {
        m_impl->withClient([&](auto &c) {
            c.set_ca_cert_path(ca_cert_path);
            c.enable_server_certificate_verification(true);
        });
    }

    void HttpClient::enableSSLVerification(const bool enable) const {
        m_impl->withClient([&](auto &c) {
            c.enable_server_certificate_verification(enable);
        });
    }

    void HttpClient::setTimeout(const int connection_sec, const int read_sec) const {
        m_impl->withClient([&](auto &c) {
            c.set_connection_timeout(connection_sec, 0);
            c.set_read_timeout(read_sec, 0);
        });
    }

    const std::string &HttpClient::getHost() const { return m_impl->m_host; }
    int HttpClient::getPort() const { return m_impl->m_port; }
    bool HttpClient::isHttps() const { return m_impl->m_isHttps; }
}
