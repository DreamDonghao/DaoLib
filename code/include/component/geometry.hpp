//
// Created by donghao on 25-12-9.
//
#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <array>
#include <SDL3/SDL_render.h>
#include <ranges>
#include <core/basic_drawing_elements/color_rgb.hpp>

namespace dao {
    /// @brief 几何图形顶点
    struct GeometryVertex {
        f32 x;      ///< x坐标
        f32 y;      ///< y坐标
        ColorRGBA color; ///< 颜色

        /// @brief 到SDL_Vertex的类型转换
        /// @details 纹理坐标用 1*1px 的纯白图片,将 255RGB 数据归一化
        explicit operator SDL_Vertex() const {
            SDL_Vertex result;
            result.position = {x, y};
            result.color = {rgbNorm[color.r], rgbNorm[color.g], rgbNorm[color.b], color.a};
            result.tex_coord = {1.0f, 1.0f};
            return result;
        }
    };

    /// @brief 将 GeometryVertex 批量转换为 SDL_Vertex
    /// @tparam N 顶点个数
    template<size_t N>
    static std::array<SDL_Vertex, N> makeVertices(const std::array<GeometryVertex, N> &v) {
        std::array<SDL_Vertex, N> arr{};
        for (size_t i = 0; i < N; ++i) {
            arr[i] = static_cast<SDL_Vertex>(v[i]);
        }
        return arr;
    }

    /// @brief 几何图形
    /// @details 一个多边形
    /// @tparam N 多边形边数
    template<size_t N>
    class Geometry {
    public:
        explicit Geometry(const std::array<GeometryVertex, N> &vertices): m_vertices(makeVertices(vertices)) {
            int index = -1;
            for (u32 i = 1; i < N - 1; ++i) {
                m_indices[++index] = 0;
                m_indices[++index] = i;
                m_indices[++index] = i + 1;
            }
        }

        void moveXY(const f32 x, const f32 y) {
            for (u32 i = 0; i < N; ++i) {
                m_vertices[i].position.x += x;
                m_vertices[i].position.y += y;
            }
        }

        /// @brief 获取顶点
        std::span<const SDL_Vertex> getVertices() const {
            return m_vertices;
        }

        /// @brief 获取索引数组
        std::span<const i32> getIndices() const {
            return m_indices;
        }

    private:
        std::array<SDL_Vertex, N> m_vertices;             ///< 顶点
        mutable std::array<i32, (N - 2) * 3> m_indices; ///< 索引
    };
} // dao

#endif //POLYGON_HPP
