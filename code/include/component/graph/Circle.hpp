#pragma once
#include <cmath>
#include <component/graph/MaxPolygon.hpp>
#include <core/tool/Point.hpp>

namespace dao {
    /// @brief 圆形图形
    /// @details 由多个三角形扇形组成，支持径向渐变（从圆心向边缘）
    class Circle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        /// @param radius 半径
        /// @param colorCenter 圆心颜色
        /// @param colorEdge 边缘颜色
        /// @param segments 圆形分段数（默认32）
        Circle(const f32 cx, const f32 cy, const f32 radius,
            const ColorRGBA &colorCenter, const ColorRGBA &colorEdge, const i32 segments = 32)
            : m_cx(cx), m_cy(cy), m_radius(radius),
              m_colorCenter(colorCenter), m_colorEdge(colorEdge),
              m_segments(segments), m_dirty(true) {
        }

        /// @brief 平移圆形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        void translate(const f32 dx, const f32 dy) override {
            m_cx += dx;
            m_cy += dy;
            m_dirty = true;
        }

        /// @brief 旋转圆形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            auto [rx, ry] = rotatePointRad(m_cx, m_cy, cx, cy, theta);
            m_cx = rx;
            m_cy = ry;
            m_dirty = true;
        }

        /// @brief 将圆形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            if (m_dirty) {
                updateRenderBase();
            }
            m_polygon.writeToBatch(batchRenderer);
        }

    private:
        f32 m_cx;                   ///< 圆心 x 坐标
        f32 m_cy;                   ///< 圆心 y 坐标
        f32 m_radius;               ///< 半径
        ColorRGBA m_colorCenter;    ///< 圆心颜色
        ColorRGBA m_colorEdge;      ///< 边缘颜色
        u32 m_segments;             ///< 分段数

        mutable bool m_dirty{true};               ///< 标记是否需要更新渲染基元
        mutable MaxPolygon m_polygon;     ///< 多边形基类

        /// @brief 更新渲染基元，构建多边形顶点
        void updateRenderBase() const {
            std::vector<Vertex> vertexes;
            vertexes.reserve(m_segments + 1);

            // 圆心作为第一个顶点（扇形三角化的公共顶点）
            vertexes.emplace_back(m_cx, m_cy, m_colorCenter);

            // 边缘顶点
            for (u32 i = 0; i <= m_segments; ++i) {
                const f32 theta = 2.0f * f32pi * static_cast<f32>(i) / static_cast<f32>(m_segments);
                const f32 x = m_cx + m_radius * std::cos(theta);
                const f32 y = m_cy + m_radius * std::sin(theta);
                vertexes.emplace_back(x, y, m_colorEdge);
            }

            m_polygon.setVertexes(std::move(vertexes));
            m_dirty = false;
        }
    };
}