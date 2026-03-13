#pragma once
#include <memory>
#include <optional>
#include <web/http/Headers.hpp>
#include <web/http/HttpTypes.hpp>

namespace dao::web {
    /// @brief HTTP客户端
    /// @details 用于发送HTTP请求，支持同步和异步请求
    class HttpClient {
    public:
        /// @brief 构造函数
        /// @param host 主机名
        /// @param port 端口号，默认为80
        explicit HttpClient(const std::string &host, int port = 80);

        /// @brief 析构函数
        ~HttpClient();

        /// @brief 从URL构造客户端
        /// @param url URL字符串
        /// @return HttpClient对象
        static HttpClient fromURL(const std::string &url);

        // 禁止拷贝
        HttpClient(const HttpClient &) = delete;

        HttpClient &operator=(const HttpClient &) = delete;

        // 允许移动
        HttpClient(HttpClient &&) noexcept;

        HttpClient &operator=(HttpClient &&) noexcept;

        /// @brief 发送GET请求（同步）
        HttpResponse get(const std::string &path) const;

        /// @brief 发送POST请求（同步）
        HttpResponse post(const std::string &path, const std::string &body,
                          const std::string &content_type = "application/json") const;

        /// @brief 发送带请求头的POST请求（同步）
        HttpResponse post(const std::string &path, const Headers &headers,
                          const std::string &body,
                          const std::string &content_type = "application/json") const;

        /// @brief 发送GET请求（异步）
        RequestId getAsync(const std::string &path) const;

        /// @brief 发送POST请求（异步）
        RequestId postAsync(const std::string &path, const std::string &body,
                            const std::string &content_type = "application/json") const;

        /// @brief 发送带请求头的POST请求（异步）
        RequestId postAsync(const std::string &path, const Headers &headers,
                            const std::string &body,
                            const std::string &content_type = "application/json") const;

        /// @brief 检查异步请求是否完成
        [[nodiscard]] bool isReady(RequestId id) const;

        /// @brief 获取异步请求响应（非阻塞）
        std::optional<HttpResponse> getResponse(RequestId id) const;

        /// @brief 等待异步请求响应（阻塞）
        HttpResponse waitResponse(RequestId id) const;

        /// @brief 取消异步请求
        void cancelRequest(RequestId id) const;

        /// @brief 设置超时时间
        void setTimeout(int connection_sec, int read_sec) const;

        /// @brief 获取主机名
        [[nodiscard]] const std::string &getHost() const;

        /// @brief 获取端口号
        [[nodiscard]] int getPort() const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
