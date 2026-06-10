#pragma once
#include <core/tool/type.hpp>
#include <functional>
#include <web/http/Headers.hpp>

namespace dao::web {
    /// @brief HTTP 方法枚举
    enum class HttpMethod : i8 {
        Get,
        Post,
        Put,
        Delete,
        Patch
    };

    /// @brief URL 解析结果
    struct ParsedURL {
        std::string host;       ///< 主机名
        i32 port = 80;          ///< 端口号
        std::string path = "/"; ///< 请求路径
        bool isHttps = false;   ///< 是否 HTTPS
    };

    /// @brief 解析 URL，提取主机名、端口、路径和协议
    /// @param url URL 字符串，支持带或不带 http/https 前缀
    /// @return 解析后的 URL 结构体
    ParsedURL parseHost(const std::string &url);

    /// @brief HTTP 响应
    struct HttpResponse {
        int status = 0;       ///< HTTP 状态码
        std::string body;     ///< 响应体
        std::string error;    ///< 错误信息
        bool success = false; ///< 请求是否成功（status == 200）
    };

    /// @brief HTTP 请求
    struct HttpRequest {
        HttpMethod method = HttpMethod::Get;           ///< 请求方法
        std::string path;                              ///< 请求路径
        Headers headers;                               ///< 自定义请求头
        std::string body;                              ///< 请求体
        std::string content_type = "application/json"; ///< Content-Type
        i32 connect_timeout_sec = 5;                   ///< 连接超时（秒）
        i32 read_timeout_sec = 10;                     ///< 读取超时（秒）
    };

    /// @brief HTTP 异步回调
    /// @details 回调在内部线程池中执行，不能直接操作 SDL UI。
    /// 如需安全更新 UI，用 HttpClient::setMainThreadCallbackHandler 投递到主线程
    using HttpCallback = std::function<void(HttpResponse)>;
}
