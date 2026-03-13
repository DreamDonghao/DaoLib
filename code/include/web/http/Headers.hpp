#pragma once
#include <initializer_list>
#include <map>
#include <string>

namespace dao::web {
    /// @brief HTTP请求头
    /// @details 支持初始化列表构造
    /// @code
    /// Headers headers = {
    ///     {"Content-Type", "application/json"},
    ///     {"Authorization", "Bearer token"}
    /// };
    /// @endcode
    class Headers {
    public:
        Headers() = default;
        Headers(std::initializer_list<std::pair<std::string, std::string>> init);

        void add(const std::string &key, const std::string &value);
        void set(const std::string &key, const std::string &value);
        [[nodiscard]] std::string get(const std::string &key) const;
        void remove(const std::string &key);
        void clear();
        [[nodiscard]] bool empty() const;
        [[nodiscard]] size_t size() const;

        auto begin() const { return m_headers.begin(); }
        auto end() const { return m_headers.end(); }

    private:
        std::multimap<std::string, std::string> m_headers;
    };
}