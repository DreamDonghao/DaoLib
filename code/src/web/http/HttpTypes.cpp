#include <web/http/HttpTypes.hpp>

namespace dao::web {
    ParsedURL parseHost(const std::string &url) {
        ParsedURL result;
        result.port = 0;
        result.path = "/";

        size_t pos = 0;

        if (url.compare(0, 7, "http://") == 0) {
            pos = 7;
        } else if (url.compare(0, 8, "https://") == 0) {
            pos = 8;
        }

        if (const size_t host_end = url.find_first_of(":/", pos); host_end == std::string::npos) {
            result.host = url.substr(pos);
        } else {
            result.host = url.substr(pos, host_end - pos);

            if (url[host_end] == ':') {
                if (const size_t port_end = url.find('/', host_end); port_end == std::string::npos) {
                    result.port = std::stoi(url.substr(host_end + 1));
                } else {
                    result.port = std::stoi(url.substr(host_end + 1, port_end - host_end - 1));
                    result.path = url.substr(port_end);
                }
            } else if (url[host_end] == '/') {
                result.path = url.substr(host_end);
            }
        }

        if (result.port == 0) {
            result.port = 80;
        }

        return result;
    }


}