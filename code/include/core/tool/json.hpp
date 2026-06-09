#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <optional>
#include <fstream>

namespace dao {
    using json = nlohmann::json;
    /// @brief 从文件读取 JSON
    /// @param path 文件路径
    /// @return JSON 对象，失败返回空
    inline std::optional<json> readJson(const std::filesystem::path &path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) return std::nullopt;

        const auto size = file.tellg();
        if (size == -1) return std::nullopt;

        std::string content(size, '\0');
        file.seekg(0);
        if (!file.read(content.data(), size)) return std::nullopt;

        try {
            return json::parse(std::move(content));
        } catch (...) {
            return std::nullopt;
        }
    }

    /// @brief 将 JSON 写入文件
    /// @param path 文件路径
    /// @param j JSON 对象
    /// @param indent 缩进空格数，-1 表示紧凑格式
    /// @return 是否成功
    inline bool writeJson(const std::filesystem::path &path, const nlohmann::json &j, const int indent = 4) {
        const auto tempPath = path.string() + ".tmp"; {
            std::ofstream file(tempPath, std::ios::binary | std::ios::trunc);
            if (!file) return false;
            file << j.dump(indent);
        }
        std::error_code ec;
        std::filesystem::rename(tempPath, path, ec);
        return !ec;
    }
}
