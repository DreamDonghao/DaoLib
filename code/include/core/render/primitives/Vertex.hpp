#pragma once
#include <core/tool/better_stl.hpp>
#include <core/render/primitives/color_rgb.hpp>
#include <SDL3/SDL_render.h>
namespace dao {
    /// @brief 几何图形顶点
    class Vertex {
    public:
        explicit Vertex(const f32 x = 0, const f32 y = 0, const ColorRGBA color = White)
            : m_x(x), m_y(y), m_color(color) {
        }

        void set(const f32 x, const f32 y, const ColorRGBA &color) {
            m_x = x;
            m_y = y;
            m_color = color;
        }

        void setColor(const ColorRGBA &color) {
            m_color = color;
        }

        void setPosition(const f32 x, const f32 y) {
            m_x = x;
            m_y = y;
        }

        /// @brief 获取x坐标
        [[nodiscard]] constexpr f32 x() const noexcept { return m_x; }

        /// @brief 获取y坐标
        [[nodiscard]] constexpr f32 y() const noexcept { return m_y; }

        /// @brief 获取颜色
        [[nodiscard]] constexpr ColorRGBA color() const noexcept { return m_color; }

        /// @brief 到SDL_Vertex的类型转换
        /// @details 纹理坐标用 1*1px 的纯白图片,将 255RGB 数据归一化
        explicit operator SDL_Vertex() const {
            SDL_Vertex result;
            result.position = {m_x, m_y};
            result.color = {rgbNorm[m_color.r()], rgbNorm[m_color.g()], rgbNorm[m_color.b()], m_color.a()};
            result.tex_coord = {1.0f, 1.0f};
            return result;
        }

    private:
        f32 m_x;                         ///< x坐标
        f32 m_y;                         ///< y坐标
        ColorRGBA m_color{0, 0, 0, 1.f}; ///< 颜色
    };
}
