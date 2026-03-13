#pragma once
#include <core/tool/type.hpp>

namespace dao::web {
    /// @brief URL解析结果
    struct ParsedURL {
        std::string host;       ///< 主机名
        i32 port = 80;          ///< 端口号
        std::string path = "/"; ///< 请求路径
    };

    /// @brief 解析URL，提取主机名、端口和路径
    /// @param url URL字符串，支持带或不带http/https前缀
    /// @return 解析后的URL结构体
    ParsedURL parseHost(const std::string &url);

    /// @brief 请求ID类型
    using RequestId = i32;

    /// @brief HTTP/HTTPS响应结构体
    struct HttpResponse {
        int status = 0;       ///< HTTP状态码
        std::string body;     ///< 响应体
        std::string error;    ///< 错误信息
        bool success = false; ///< 请求是否成功（status == 200）
    };
}
