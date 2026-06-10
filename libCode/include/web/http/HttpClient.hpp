#pragma once
#include <memory>
#include <functional>
#include <web/http/HttpTypes.hpp>

namespace dao::web {
    /// @brief 统一 HTTP/HTTPS 客户端
    /// @details 通过 port 或 URL scheme 自动选择底层协议，内部维护固定大小线程池执行异步请求。
    /// httplib::Client / SSLClient 无公共基类且不可拷贝，底层用 variant<unique_ptr<>> 擦除类型差异。
    ///
    /// 同步请求:
    /// @code
    ///   auto client = HttpClient::fromURL("https://api.example.com");
    ///   auto resp = client.get("/data");
    /// @endcode
    ///
    /// 回调式异步:
    /// @code
    ///   client.get("/api/status", [this](HttpResponse resp) {
    ///       std::lock_guard lock(m_mutex);
    ///       m_data = std::move(resp.body);
    ///   });
    /// @endcode
    class HttpClient {
    public:
        /// @brief 直接构造
        /// @param host 主机名
        /// @param port 端口号，默认为 80。port==443 时自动启用 SSL
        explicit HttpClient(const std::string &host, int port = 80);

        ~HttpClient();

        /// @brief 从 URL 构造，根据 scheme 自动选择 HTTP/HTTPS
        /// @param url 完整 URL（如 "https://api.example.com:8443/path"）
        static HttpClient fromURL(const std::string &url);

        /// @brief 显式构造 HTTPS 客户端
        /// @param host 主机名
        /// @param port 端口号，默认为 443
        static HttpClient https(const std::string &host, int port = 443);

        /// @brief 禁止拷贝
        HttpClient(const HttpClient &) = delete;

        HttpClient &operator=(const HttpClient &) = delete;

        /// @brief 允许移动
        HttpClient(HttpClient &&) noexcept;

        HttpClient &operator=(HttpClient &&) noexcept;

        // ---- 同步请求 ----

        /// @brief 发送 GET 请求（同步，阻塞当前线程）
        /// @param path 请求路径
        [[nodiscard]] HttpResponse get(const std::string &path) const;

        /// @brief 发送通用请求（同步）
        /// @param req 请求描述（方法、路径、请求头、超时等）
        [[nodiscard]] HttpResponse request(const HttpRequest &req) const;

        // ---- 回调式异步请求（内部线程池执行）----

        /// @brief 发送 GET 请求（异步，回调在线程池线程执行）
        /// @param path 请求路径
        /// @param callback 完成回调
        void get(const std::string &path, HttpCallback callback);

        /// @brief 发送 POST 请求（异步）
        /// @param path 请求路径
        /// @param body 请求体
        /// @param callback 完成回调
        /// @param content_type Content-Type，默认 application/json
        void post(const std::string &path, const std::string &body,
                  HttpCallback callback, const std::string &content_type = "application/json");

        /// @brief 发送带自定义请求头的 POST 请求（异步）
        /// @param path 请求路径
        /// @param headers 自定义请求头
        /// @param body 请求体
        /// @param callback 完成回调
        /// @param content_type Content-Type，默认 application/json
        void post(const std::string &path, const Headers &headers,
                  const std::string &body, HttpCallback callback,
                  const std::string &content_type = "application/json");

        /// @brief 发送通用请求（异步）
        /// @param req 请求描述
        /// @param callback 完成回调
        void request(const HttpRequest &req, HttpCallback callback);

        // ---- 主线程回调投递 ----

        /// @brief 设置后，所有异步回调自动通过此 handler 投递到主线程
        /// @param handler 接收一个 task（function<void()>），由调用方决定何时在何线程执行
        /// @note 典型用法：handler 将 task 存入队列，在 Page::update() 中逐个执行
        void setMainThreadCallbackHandler(std::function<void(std::function<void()>)> handler);

        // ---- SSL 配置 ----

        /// @brief 设置 CA 证书路径并启用验证
        /// @param ca_cert_path CA 证书文件路径
        void setCaCertPath(const std::string &ca_cert_path) const;

        /// @brief 启用或禁用 SSL 证书验证
        /// @param enable true 启用，false 禁用（默认禁用）
        /// @warning 禁用证书验证可能导致中间人攻击
        void enableSSLVerification(bool enable) const;

        // ---- 超时 & 信息 ----

        /// @brief 设置超时
        /// @param connection_sec 连接超时（秒）
        /// @param read_sec 读取超时（秒）
        void setTimeout(int connection_sec, int read_sec) const;

        /// @brief 获取主机名
        [[nodiscard]] const std::string &getHost() const;

        /// @brief 获取端口号
        [[nodiscard]] int getPort() const;

        /// @brief 是否 HTTPS
        [[nodiscard]] bool isHttps() const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl; ///< Pimpl 模式，隐藏线程池和 httplib 实现
    };
}
