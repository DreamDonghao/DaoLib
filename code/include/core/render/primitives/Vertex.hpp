#pragma once
#include <core/tool/better_stl.hpp>
#include <core/render/primitives/ColorRGB.hpp>
#include <SDL3/SDL_render.h>

namespace dao {
    /// @brief 几何图形顶点
    class Vertex {
    public:
        explicit Vertex(const f32 x = 0, const f32 y = 0, const ColorRGBA color = White)
            : m_color(color), m_data{
                  {x, y}, static_cast<SDL_FColor>(color), {1.0f, 1.0f}
              } {
        }

        void set(const f32 x, const f32 y, const ColorRGBA &color) {
            m_data.position = {x, y};
            m_color = color;
        }

        void setColor(const ColorRGBA &color) {
            m_color = color;
            m_data.color = static_cast<SDL_FColor>(color);
        }

        void setPosition(const f32 x, const f32 y) {
            m_data.position = {x, y};
        }

        void translate(const f32 dx, const f32 dy) {
            m_data.position.x += dx;
            m_data.position.y += dy;
        }

        /// @brief 获取x坐标
        [[nodiscard]] constexpr f32 x() const noexcept { return m_data.position.x; }

        /// @brief 获取y坐标
        [[nodiscard]] constexpr f32 y() const noexcept { return m_data.position.y; }

        /// @brief 获取颜色
        [[nodiscard]] constexpr ColorRGBA color() const noexcept { return m_color; }

        /// @brief 到SDL_Vertex的类型转换
        /// @details 纹理坐标用 1*1px 的纯白图片,将 255RGB 数据归一化
        explicit constexpr operator SDL_Vertex() const noexcept {
            return m_data;
        }

    private:
        ColorRGBA m_color{0, 0, 0, 1.f}; ///< 颜色
        SDL_Vertex m_data;
    };
}
