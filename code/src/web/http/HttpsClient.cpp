#include <web/http/HttpsClient.hpp>
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

    class HttpsClient::Impl {
    public:
        Impl(const std::string &host, const int port)
            : client(host, port), host(host), port(port) {
            client.set_connection_timeout(5, 0);
            client.set_read_timeout(10, 0);
            client.enable_server_certificate_verification(false);
        }

        httplib::SSLClient client;
        std::string host;
        int port;
        std::atomic<RequestId> nextId{0};
        std::unordered_map<RequestId, std::shared_future<HttpResponse>> futures;
    };

    HttpsClient::HttpsClient(const std::string &host, int port)
        : m_impl(std::make_unique<Impl>(host, port)) {}

    HttpsClient::~HttpsClient() = default;

    HttpsClient::HttpsClient(HttpsClient&&) noexcept = default;
    HttpsClient& HttpsClient::operator=(HttpsClient&&) noexcept = default;

    HttpsClient HttpsClient::fromURL(const std::string &url) {
        const auto parsed = parseHost(url);
        const int port = (parsed.port == 80) ? 443 : parsed.port;
        return HttpsClient(parsed.host, port);
    }

    void HttpsClient::setCaCertPath(const std::string &ca_cert_path) const {
        m_impl->client.set_ca_cert_path(ca_cert_path);
        m_impl->client.enable_server_certificate_verification(true);
    }

    void HttpsClient::enableSSLVerification(const bool enable) const {
        m_impl->client.enable_server_certificate_verification(enable);
    }

    HttpResponse HttpsClient::get(const std::string &path) const {
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

    HttpResponse HttpsClient::post(const std::string &path, const std::string &body,
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

    HttpResponse HttpsClient::post(const std::string &path, const Headers &headers,
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

    RequestId HttpsClient::getAsync(const std::string &path) const {
        const RequestId id = ++m_impl->nextId;
        m_impl->futures[id] = std::async(std::launch::async, [this, path]() {
            return get(path);
        }).share();
        return id;
    }

    RequestId HttpsClient::postAsync(const std::string &path, const std::string &body,
                                      const std::string &content_type) const {
        const RequestId id = ++m_impl->nextId;
        m_impl->futures[id] = std::async(std::launch::async, [this, path, body, content_type]() {
            return post(path, body, content_type);
        }).share();
        return id;
    }

    RequestId HttpsClient::postAsync(const std::string &path, const Headers &headers,
                                      const std::string &body, const std::string &content_type) const {
        const RequestId id = ++m_impl->nextId;
        m_impl->futures[id] = std::async(std::launch::async, [this, path, headers, body, content_type]() {
            return post(path, headers, body, content_type);
        }).share();
        return id;
    }

    bool HttpsClient::isReady(const RequestId id) const {
        const auto it = m_impl->futures.find(id);
        if (it == m_impl->futures.end() || !it->second.valid()) {
            return true;
        }
        return it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    std::optional<HttpResponse> HttpsClient::getResponse(const RequestId id) const {
        const auto it = m_impl->futures.find(id);
        if (it == m_impl->futures.end() || !it->second.valid()) {
            return std::nullopt;
        }
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            return it->second.get();
        }
        return std::nullopt;
    }

    HttpResponse HttpsClient::waitResponse(const RequestId id) const {
        const auto it = m_impl->futures.find(id);
        if (it == m_impl->futures.end() || !it->second.valid()) {
            return HttpResponse{.error = "Invalid request ID"};
        }
        auto result = it->second.get();
        m_impl->futures.erase(it);
        return result;
    }

    void HttpsClient::cancelRequest(const RequestId id) const {
        m_impl->futures.erase(id);
    }

    void HttpsClient::setTimeout(const int connection_sec, const int read_sec) const {
        m_impl->client.set_connection_timeout(connection_sec, 0);
        m_impl->client.set_read_timeout(read_sec, 0);
    }

    const std::string &HttpsClient::getHost() const {
        return m_impl->host;
    }

    int HttpsClient::getPort() const {
        return m_impl->port;
    }
}
