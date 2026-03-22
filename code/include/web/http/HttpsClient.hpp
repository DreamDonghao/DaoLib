#pragma once
#include <memory>
#include <optional>
#include <web/http/Headers.hpp>
#include <web/http/HttpTypes.hpp>

namespace dao::web {
    /// @brief HTTPS客户端
    /// @details 用于发送HTTPS请求，支持同步和异步请求
    /// @note 默认关闭SSL证书验证，生产环境应调用setCaCertPath()设置CA证书
    ///
    /// @example
    /// @code
    /// // 同步请求
    /// dao::web::HttpsClient client("api.example.com", 443);
    /// client.setTimeout(10, 30);
    ///
    /// auto resp = client.get("/api/data");
    /// if (resp.success) {
    ///     std::cout << resp.body << std::endl;
    /// }
    ///
    /// // POST请求
    /// dao::web::Headers headers = {{"Authorization", "Bearer token"}};
    /// auto resp = client.post("/api/submit", headers, R"({"key": "value"})");
    ///
    /// // 异步请求
    /// auto requestId = client.postAsync("/api/submit", "{}");
    /// if (client.isReady(requestId)) {
    ///     auto resp = client.getResponse(requestId);
    /// }
    /// @endcode
    class HttpsClient {
    public:
        /// @brief 构造函数
        /// @param host 主机名（不含协议，如 "api.example.com"）
        /// @param port 端口号，默认为443
        explicit HttpsClient(const std::string &host, int port = 443);

        /// @brief 析构函数
        ~HttpsClient();

        /// @brief 从完整URL构造客户端
        /// @param url 完整URL（如 "https://api.example.com:8443/path"）
        /// @return HttpsClient实例
        /// @note 会自动解析URL中的主机名和端口
        static HttpsClient fromURL(const std::string &url);

        // 禁止拷贝
        HttpsClient(const HttpsClient &) = delete;
        HttpsClient &operator=(const HttpsClient &) = delete;

        // 允许移动
        HttpsClient(HttpsClient &&) noexcept;
        HttpsClient &operator=(HttpsClient &&) noexcept;

        /// @brief 设置CA证书路径
        /// @param ca_cert_path CA证书文件的路径
        /// @note 设置后会自动启用SSL证书验证
        void setCaCertPath(const std::string &ca_cert_path) const;

        /// @brief 启用或禁用SSL证书验证
        /// @param enable true启用，false禁用
        /// @warning 禁用证书验证可能导致中间人攻击，生产环境不建议禁用
        void enableSSLVerification(bool enable) const;

        /// @brief 发送GET请求（同步）
        /// @param path 请求路径（如 "/api/data"）
        /// @return HttpResponse响应对象
        [[nodiscard]] HttpResponse get(const std::string &path) const;

        /// @brief 发送POST请求（同步）
        /// @param path 请求路径
        /// @param body 请求体内容
        /// @param content_type Content-Type，默认"application/json"
        /// @return HttpResponse响应对象
        [[nodiscard]] HttpResponse post(const std::string &path, const std::string &body,
                                        const std::string &content_type = "application/json") const;

        /// @brief 发送带自定义请求头的POST请求（同步）
        /// @param path 请求路径
        /// @param headers 自定义请求头
        /// @param body 请求体内容
        /// @param content_type Content-Type，默认"application/json"
        /// @return HttpResponse响应对象
        [[nodiscard]] HttpResponse post(const std::string &path, const Headers &headers,
                                        const std::string &body,
                                        const std::string &content_type = "application/json") const;

        /// @brief 发送GET请求（异步）
        /// @param path 请求路径
        /// @return RequestId请求ID，用于后续查询响应
        [[nodiscard]] RequestId getAsync(const std::string &path) const;

        /// @brief 发送POST请求（异步）
        /// @param path 请求路径
        /// @param body 请求体内容
        /// @param content_type Content-Type，默认"application/json"
        /// @return RequestId请求ID
        [[nodiscard]] RequestId postAsync(const std::string &path, const std::string &body,
                                           const std::string &content_type = "application/json") const;

        /// @brief 发送带自定义请求头的POST请求（异步）
        /// @param path 请求路径
        /// @param headers 自定义请求头
        /// @param body 请求体内容
        /// @param content_type Content-Type，默认"application/json"
        /// @return RequestId请求ID
        [[nodiscard]] RequestId postAsync(const std::string &path, const Headers &headers,
                                           const std::string &body,
                                           const std::string &content_type = "application/json") const;

        /// @brief 检查异步请求是否完成
        /// @param id 请求ID
        /// @return true已完成，false未完成
        [[nodiscard]] bool isReady(RequestId id) const;

        /// @brief 查看异步请求响应（非阻塞）
        /// @param id 请求ID
        /// @return 若请求完成则返回响应，否则返回空
        /// @note 不会清除内部存储的响应
        [[nodiscard]] std::optional<HttpResponse> getResponse(RequestId id) const;

        /// @brief 提取异步请求响应（非阻塞）
        /// @param id 请求ID
        /// @return 若请求完成则返回响应并清除内部存储，否则返回空
        [[nodiscard]] std::optional<HttpResponse> takeResponse(RequestId id) const;

        /// @brief 等待异步请求响应（阻塞）
        /// @param id 请求ID
        /// @return HttpResponse响应对象
        /// @note 此方法会阻塞直到请求完成，不会清除内部存储的响应
        [[nodiscard]] HttpResponse waitResponse(RequestId id) const;

        /// @brief 取消异步请求
        /// @param id 请求ID
        void cancelRequest(RequestId id) const;

        /// @brief 设置超时时间
        /// @param connection_sec 连接超时（秒）
        /// @param read_sec 读取超时（秒）
        void setTimeout(int connection_sec, int read_sec) const;

        /// @brief 获取主机名
        /// @return 主机名
        [[nodiscard]] const std::string &getHost() const;

        /// @brief 获取端口号
        /// @return 端口号
        [[nodiscard]] int getPort() const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl; ///< Pimpl模式，隐藏实现细节
    };
}