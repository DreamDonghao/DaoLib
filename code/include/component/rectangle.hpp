#pragma once
#include <interface/drawable.hpp>
#include <component/geometry.hpp>
#include <array>

namespace dao {
    /// @brief 矩形
    class Rectangle : public Drawable {
    public:
        Rectangle(const f32 x, const f32 y, const f32 w, const f32 h, const ColorRGBA color)
            : m_triangle{
                Triangle(Vertex{x, y,color}, Vertex{x + w, y,color}, Vertex{x + w, y + h,color}),
                Triangle(Vertex{x + w, y + h,color}, Vertex{x, y + h,color}, Vertex{x, y,color})
            } {
        }

        Rectangle(const BoundingBox box, const ColorRGBA color)
            : Rectangle(box.getLeft(), box.getTop(), box.getWidth(), box.getHeight(), color) {
        }

        void setPosition(const f32 x, const f32 y) {
            const f32 dx = x - m_triangle[0].vertex(0).x();
            const f32 dy = y - m_triangle[1].vertex(0).y();
            m_triangle[0].translate(dx,dy);
        }

        void writeToBatch(VertexBatchBuilder &builder) const override {
            builder.addToBatch(m_triangle[0]);
            builder.addToBatch(m_triangle[1]);
        }

    private:
        std::array<Triangle, 2> m_triangle;
    };
}

