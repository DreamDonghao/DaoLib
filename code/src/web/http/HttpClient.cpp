#include <web/http/HttpClient.hpp>
#include <httplib.h>
#include <atomic>
#include <unordered_map>
#include <future>
#include <web/http/Headers.hpp>

namespace dao::web {
    /// @brief 将 Headers 转换为 httplib::Headers
    inline httplib::Headers toHttplibHeaders(const Headers &headers) {
        httplib::Headers result;
        for (const auto &[key, value] : headers) {
            result.emplace(key, value);
        }
        return result;
    }

    class HttpClient::Impl {
    public:
        Impl(const std::string &host, const int port)
            : client(host, port), host(host), port(port) {
            client.set_connection_timeout(5, 0);
            client.set_read_timeout(10, 0);
        }

        httplib::Client client;
        std::string host;
        int port;
        std::atomic<RequestId> nextId{0};
        std::unordered_map<RequestId, std::shared_future<HttpResponse>> futures;
    };

    HttpClient::HttpClient(const std::string &host, int port)
        : m_impl(std::make_unique<Impl>(host, port)) {}

    HttpClient::~HttpClient() = default;

    HttpClient::HttpClient(HttpClient&&) noexcept = default;
    HttpClient& HttpClient::operator=(HttpClient&&) noexcept = default;

    HttpClient HttpClient::fromURL(const std::string &url) {
        const auto parsed = parseHost(url);
        return HttpClient(parsed.host, parsed.port);
    }

    HttpResponse HttpClient::get(const std::string &path) const {
        HttpResponse resp;
        try {
            if (auto res = m_impl->client.Get(path)) {
                resp.status = res->status;
                resp.body = std::move(res->body);
                resp.success = (res->status == 200);
                if (res->status != 200) {
                    resp.error = "HTTP Error: " + std::to_string(res->status);
                }
            } else {
                resp.error = "Request failed: " + httplib::to_string(res.error());
            }
        } catch (const std::exception &e) {
            resp.error = std::string("Exception: ") + e.what();
        }
        return resp;
    }

    HttpResponse HttpClient::post(const std::string &path, const std::string &body,
                                   const std::string &content_type) const {
        HttpResponse resp;
        try {
            if (auto res = m_impl->client.Post(path, body, content_type)) {
                resp.status = res->status;
                resp.body = std::move(res->body);
                resp.success = (res->status == 200);
                if (res->status != 200) {
                    resp.error = "HTTP Error: " + std::to_string(res->status);
                }
            } else {
                resp.error = "Request failed: " + httplib::to_string(res.error());
            }
        } catch (const std::exception &e) {
            resp.error = std::string("Exception: ") + e.what();
        }
        return resp;
    }

    HttpResponse HttpClient::post(const std::string &path, const Headers &headers,
                                   const std::string &body, const std::string &content_type) const {
        HttpResponse resp;
        try {
            if (auto res = m_impl->client.Post(path, toHttplibHeaders(headers), body, content_type)) {
                resp.status = res->status;
                resp.body = std::move(res->body);
                resp.success = (res->status == 200);
                if (res->status != 200) {
                    resp.error = "HTTP Error: " + std::to_string(res->status);
                }
            } else {
                resp.error = "Request failed: " + httplib::to_string(res.error());
            }
        } catch (const std::exception &e) {
            resp.error = std::string("Exception: ") + e.what();
        }
        return resp;
    }

    RequestId HttpClient::getAsync(const std::string &path) const {
        const RequestId id = ++m_impl->nextId;
        m_impl->futures[id] = std::async(std::launch::async, [this, path]() {
            return get(path);
        }).share();
        return id;
    }

    RequestId HttpClient::postAsync(const std::string &path, const std::string &body,
                                     const std::string &content_type) const {
        const RequestId id = ++m_impl->nextId;
        m_impl->futures[id] = std::async(std::launch::async, [this, path, body, content_type]() {
            return post(path, body, content_type);
        }).share();
        return id;
    }

    RequestId HttpClient::postAsync(const std::string &path, const Headers &headers,
                                     const std::string &body, const std::string &content_type) const {
        const RequestId id = ++m_impl->nextId;
        m_impl->futures[id] = std::async(std::launch::async, [this, path, headers, body, content_type]() {
            return post(path, headers, body, content_type);
        }).share();
        return id;
    }

    bool HttpClient::isReady(const RequestId id) const {
        const auto it = m_impl->futures.find(id);
        if (it == m_impl->futures.end() || !it->second.valid()) {
            return true;
        }
        return it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    std::optional<HttpResponse> HttpClient::getResponse(const RequestId id) const {
        const auto it = m_impl->futures.find(id);
        if (it == m_impl->futures.end() || !it->second.valid()) {
            return std::nullopt;
        }
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            return it->second.get();
        }
        return std::nullopt;
    }

    HttpResponse HttpClient::waitResponse(const RequestId id) const {
        const auto it = m_impl->futures.find(id);
        if (it == m_impl->futures.end() || !it->second.valid()) {
            return HttpResponse{.error = "Invalid request ID"};
        }
        auto result = it->second.get();
        m_impl->futures.erase(it);
        return result;
    }

    void HttpClient::cancelRequest(const RequestId id) const {
        m_impl->futures.erase(id);
    }

    void HttpClient::setTimeout(const int connection_sec, const int read_sec) const {
        m_impl->client.set_connection_timeout(connection_sec, 0);
        m_impl->client.set_read_timeout(read_sec, 0);
    }

    const std::string &HttpClient::getHost() const {
        return m_impl->host;
    }

    int HttpClient::getPort() const {
        return m_impl->port;
    }
}
