#include <web/http/Headers.hpp>

namespace dao::web {
    Headers::Headers(std::initializer_list<std::pair<std::string, std::string>> init) {
        for (const auto &[key, value] : init) {
            m_headers.emplace(key, value);
        }
    }

    void Headers::add(const std::string &key, const std::string &value) {
        m_headers.emplace(key, value);
    }

    void Headers::set(const std::string &key, const std::string &value) {
        m_headers.erase(key);
        m_headers.emplace(key, value);
    }

    std::string Headers::get(const std::string &key) const {
        const auto it = m_headers.find(key);
        return (it != m_headers.end()) ? it->second : "";
    }

    void Headers::remove(const std::string &key) {
        m_headers.erase(key);
    }

    void Headers::clear() {
        m_headers.clear();
    }

    bool Headers::empty() const {
        return m_headers.empty();
    }

    size_t Headers::size() const {
        return m_headers.size();
    }
}