//
// Created by donghao on 25-12-8.
//
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include <interface/drawable.hpp>
#include <component/geometry.hpp>
#include <array>

namespace dao {
    /// @brief 矩形
    class Rectangle : public Drawable {
    public:
        Rectangle(const f32 x, const f32 y, const f32 w, const f32 h, const ColorRGBA color)
            : m_vertices(std::array<GeometryVertex, 4>(
                {
                    {x, y, color},
                    {x + w, y, color},
                    {x + w, y + h, color},
                    {x, y + h, color}
                })) {
        }

        Rectangle(const BoundingBox box, const ColorRGBA color)
            : Rectangle(box.getLeft(), box.getTop(), box.getWidth(), box.getHeight(), color) {
        }

        void setPosition(const f32 x, const f32 y) {
            const f32 dx = x - m_vertices.getVertices()[0].position.x;
            const f32 dy = y - m_vertices.getVertices()[0].position.y;
            m_vertices.moveXY(dx, dy);
        }

        void writeToBatch(VertexBatchBuilder &builder) const override {
            builder.addToBatch(m_vertices.getVertices(), m_vertices.getIndices());
        }

    private:
        Geometry<4> m_vertices;
    };
}
#endif //RECTANGLE_HPP
