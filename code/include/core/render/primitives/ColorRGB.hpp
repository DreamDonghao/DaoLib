#pragma once
#include <core/tool/better_stl.hpp>
#include <array>
#include <SDL3/SDL_pixels.h>
namespace dao {
    /// @brief RGB值归一化查找表
    /// @details 将0-255的整型RGB值预先转换为0.0-1.0的浮点数，用于提高渲染性能。
    /// 使用constexpr编译时计算，避免运行时开销。
    constexpr std::array<f32, 256> rgbNorm = [] {
        std::array<f32, 256> lut{};
        for (std::size_t i = 0; i < lut.size(); ++i) {
            lut[i] = static_cast<float>(i) / 255.0f;
        }
        return lut;
    }();


    /// @brief RGBA颜色类
    /// @details 表示一个32位RGBA颜色值，支持整数通道(0-255)和浮点透明度(0.0-1.0)。
    /// 提供十六进制字符串转换、颜色操作和格式验证功能。
    class ColorRGBA {
    public:
        /// @brief 构造函数（RGBA分量）
        /// @param r 红色通道值 (0-255)
        /// @param g 绿色通道值 (0-255)
        /// @param b 蓝色通道值 (0-255)
        /// @param a 透明度值 (0.0-1.0)，默认为不透明(1.0)
        constexpr explicit ColorRGBA(const i32 r = 0, const i32 g = 0, const i32 b = 0, const f32 a = 1.0f) noexcept
            : m_r(r), m_g(g), m_b(b), m_a(a) {
        }

        /// @brief 构造函数（十六进制字符串）
        /// @param hex 十六进制颜色字符串，支持格式：#RRGGBB 或 #RRGGBBAA
        /// @details 如果字符串格式无效，将返回白色(#FFFFFF)作为默认值
        explicit ColorRGBA(const std::string_view hex) {
            const ColorRGBA temp = hexToRGBA(hex);
            m_r = temp.m_r;
            m_g = temp.m_g;
            m_b = temp.m_b;
            m_a = temp.m_a;
        }


        /// @brief 设置所有颜色分量
        /// @param r 红色通道值 (0-255)
        /// @param g 绿色通道值 (0-255)
        /// @param b 蓝色通道值 (0-255)
        /// @param a 透明度值 (0.0-1.0)，默认为不透明(1.0)
        void set(const i32 r, const i32 g, const i32 b, const f32 a = 1.0f) noexcept {
            m_r = r;
            m_g = g;
            m_b = b;
            m_a = a;
        }

        /// @brief 通过十六进制字符串设置颜色
        /// @param hex 十六进制颜色字符串，支持格式：#RRGGBB 或 #RRGGBBAA
        void set(const std::string_view hex) {
            *this = ColorRGBA(hex);
        }

        /// @brief 设置红色通道值
        void setR(const i32 r) noexcept { m_r = r; }

        /// @brief 设置绿色通道值
        void setG(const i32 g) noexcept { m_g = g; }

        /// @brief 设置蓝色通道值
        void setB(const i32 b) noexcept { m_b = b; }

        /// @brief 设置透明度值
        void setA(const f32 a) noexcept { m_a = a; }

        /// @brief 获取红色通道值
        [[nodiscard]] constexpr i32 r() const noexcept { return m_r; }

        /// @brief 获取绿色通道值
        [[nodiscard]] constexpr i32 g() const noexcept { return m_g; }

        /// @brief 获取蓝色通道值
        [[nodiscard]] constexpr i32 b() const noexcept { return m_b; }

        /// @brief 获取透明度值
        [[nodiscard]] constexpr f32 a() const noexcept { return m_a; }

        /// @brief 相等比较操作符
        [[nodiscard]] constexpr bool operator==(const ColorRGBA &other) const noexcept {
            constexpr f32 epsilon = 1e-5f;
            const f32 diff = (m_a > other.m_a) ? (m_a - other.m_a) : (other.m_a - m_a);
            return m_r == other.m_r && m_g == other.m_g && m_b == other.m_b && diff < epsilon;
        }

        /// @brief 不等比较操作符
        [[nodiscard]] constexpr bool operator!=(const ColorRGBA &other) const noexcept {
            return !(*this == other);
        }

        /// @brief 检查颜色是否为有效颜色
        /// @return true 如果所有颜色分量在有效范围内，否则false
        /// @details 有效范围：RGB分量0-255，Alpha分量0.0-1.0
        [[nodiscard]] constexpr bool isValid() const noexcept {
            return m_r >= 0 && m_r <= 255 &&
                   m_g >= 0 && m_g <= 255 &&
                   m_b >= 0 && m_b <= 255 &&
                   m_a >= 0.0f && m_a <= 1.0f;
        }

        /// @brief 检查颜色是否为NullRGBA（无效颜色哨兵值）
        /// @return true 如果此颜色等于NullRGBA常量
        [[nodiscard]] constexpr bool isNull() const noexcept {
            // 直接比较NullRGBA的值：(-1, -1, -1, -1.0f)
            return m_r == -1 && m_g == -1 && m_b == -1 && std::abs(m_a - (-1.0f)) < 1e-5f;
        }

        /// @brief 将十六进制颜色字符串转换为ColorRGBA对象
        /// @param hex 十六进制颜色字符串，支持格式：RRGGBB、#RRGGBB、RRGGBBAA或#RRGGBBAA
        /// @return 转换后的ColorRGBA对象，如果格式无效则返回白色(#FFFFFF)
        /// @details 算法步骤：
        /// 1. 移除可选的'#'前缀
        /// 2. 验证长度必须是6(RGB)或8(RGBA)
        /// 3. 将每两个字符转换为一个字节值(0-255)
        /// 4. 检测转换错误：值为0但对应的十六进制字符不是"00"
        static constexpr ColorRGBA hexToRGBA(std::string_view hex) noexcept {
            // 移除'#'前缀
            if (!hex.empty() && hex[0] == '#')
                hex.remove_prefix(1);

            const auto len = hex.size();
            // 验证长度
            if (len != 6 && len != 8) {
                return ColorRGBA{255, 255, 255, 1.0f}; // 无效格式，返回白色
            }

            // 转换RGB分量
            const i32 r = hexByteToU8(hex.substr(0, 2));
            const i32 g = hexByteToU8(hex.substr(2, 2));
            const i32 b = hexByteToU8(hex.substr(4, 2));

            // 转换Alpha分量（如果存在）
            const f32 a = (len == 8) ? rgbNorm[hexByteToU8(hex.substr(6, 2))] : 1.0f;

            // 验证转换结果：检查是否因无效字符导致转换失败（返回0但hex不是"00"）
            const auto isValidHexByte = [&hex](i32 value, size_t pos1, size_t pos2) {
                return !(value == 0 && (hex[pos1] != '0' || hex[pos2] != '0'));
            };

            if (!isValidHexByte(r, 0, 1) || !isValidHexByte(g, 2, 3) || !isValidHexByte(b, 4, 5) ||
                (len == 8 && !isValidHexByte(hexByteToU8(hex.substr(6, 2)), 6, 7))) {
                return ColorRGBA{255, 255, 255, 1.0f}; // 无效字符，返回白色
            }

            return ColorRGBA{r, g, b, a};
        }

         constexpr  SDL_FColor getSDLFColor() const noexcept {
            return {rgbNorm[m_r], rgbNorm[m_g], rgbNorm[m_b], m_a};
        }

    private:
        i32 m_r; ///< 红色通道值 (0-255)
        i32 m_g; ///< 绿色通道值 (0-255)
        i32 m_b; ///< 蓝色通道值 (0-255)
        f32 m_a; ///< 透明度值 (0.0-1.0)

        /// @brief 将两个十六进制字符转换为对应的字节值(0-255)
        /// @param sv 包含1-2个十六进制字符的字符串视图
        /// @return 转换后的字节值，如果包含无效字符则返回0
        /// @details 支持大写和小写十六进制字符(A-F, a-f)。算法从左到右处理每个字符，
        /// 每次将当前值乘以16并加上新字符的值。
        static constexpr i32 hexByteToU8(const std::string_view sv) noexcept {
            i32 value = 0;
            for (const char c: sv) {
                value *= 16;
                if (c >= '0' && c <= '9') {
                    value += static_cast<i32>(c - '0');
                } else if (c >= 'A' && c <= 'F') {
                    value += static_cast<i32>(c - 'A' + 10);
                } else if (c >= 'a' && c <= 'f') {
                    value += static_cast<i32>(c - 'a' + 10);
                } else {
                    return 0; // 无效字符
                }
            }
            return value;
        }
    };

    // 常见颜色常量（类外部定义）
    inline constexpr ColorRGBA NullRGBA(-1, -1, -1, -1.0f); ///< 无效颜色哨兵值，用于表示未初始化或空颜色
    inline constexpr ColorRGBA Black(0, 0, 0, 1.0f);        ///< 黑色 #000000
    inline constexpr ColorRGBA White(255, 255, 255, 1.0f);  ///< 白色 #FFFFFF
    inline constexpr ColorRGBA Red(255, 0, 0, 1.0f);        ///< 红色 #FF0000
    inline constexpr ColorRGBA Green(0, 255, 0, 1.0f);      ///< 绿色 #00FF00
    inline constexpr ColorRGBA Blue(0, 0, 255, 1.0f);       ///< 蓝色 #0000FF
    inline constexpr ColorRGBA Yellow(255, 255, 0, 1.0f);   ///< 黄色 #FFFF00
    inline constexpr ColorRGBA Cyan(0, 255, 255, 1.0f);     ///< 青色 #00FFFF
    inline constexpr ColorRGBA Magenta(255, 0, 255, 1.0f);  ///< 洋红色 #FF00FF
    inline constexpr ColorRGBA Transparent(0, 0, 0, 0.0f);  ///< 完全透明 #00000000
}
