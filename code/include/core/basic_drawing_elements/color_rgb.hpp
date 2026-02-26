//
// Created by donghao on 25-12-10.
//
#ifndef COLOR_HPP
#define COLOR_HPP
#include <core/tool/better_stl.hpp>
#include <array>

namespace dao {
    /// @brief 将RGB 255 归一化
    constexpr std::array<f32, 256> rgbNorm = [] {
        std::array<f32, 256> lut{};
        for (std::size_t i = 0; i < lut.size(); ++i) {
            lut[i] = static_cast<float>(i) / 255.0f;
        }
        return lut;
    }();

    /// @brief RGBA 颜色
    struct ColorRGBA {
        i32 r, g, b;
        float a;
    };

    constexpr i32 hexByteToU8(const std::string_view sv) noexcept {
        i32 value{0};
        for (const char c: sv) {
            value *= 16;
            if (c >= '0' && c <= '9') value += c - '0';
            else if (c >= 'A' && c <= 'F') value += c - 'A' + 10;
            else if (c >= 'a' && c <= 'f') value += c - 'a' + 10;
            else return 0;
        }
        return value;
    }

    /// @brief 将 hex 颜色格式转换为 RGBA 格式
    constexpr ColorRGBA hexToRGBA(std::string_view hex) {
        if (!hex.empty() && hex[0] == '#')
            hex.remove_prefix(1);
        const auto len{hex.size()};
        if (len != 6 && len != 8) {
            return {255, 255, 255, 1.0f};
        }
        const i32 r{hexByteToU8(hex.substr(0, 2))};
        const i32 g = hexByteToU8(hex.substr(2, 2));
        const i32 b = hexByteToU8(hex.substr(4, 2));
        const f32 a = (len == 8) ? rgbNorm[hexByteToU8(hex.substr(6, 2))] : 1.0f;

        if ((r == 0 && (hex[0] != '0' || hex[1] != '0')) ||
            (g == 0 && (hex[2] != '0' || hex[3] != '0')) ||
            (b == 0 && (hex[4] != '0' || hex[5] != '0')) ||
            (len == 8 && hexByteToU8(hex.substr(6, 2)) == 0 && (hex[6] != '0' || hex[7] != '0'))) {
            return {255, 255, 255, 1.0f};
        }
        return {r, g, b, a};
    }
}
#endif //COLOR_HPP
