#include <components/graphs/Triangle.hpp>

namespace dao {
    Triangle::Triangle(const f32 x1, const f32 y1, const f32 x2, const f32 y2,
                       const f32 x3, const f32 y3, const ColorRGBA &color) {
        m_vertices[0] = Vertex(x1, y1, color);
        m_vertices[1] = Vertex(x2, y2, color);
        m_vertices[2] = Vertex(x3, y3, color);
    }

    void Triangle::setVertex(const i32 index, const f32 x, const f32 y) {
        if (index >= 0 && index < 3) {
            m_vertices[index].setPosition(x, y);
        }
    }

    void Triangle::setColor(const ColorRGBA &color) {
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void Triangle::translate(const f32 dx, const f32 dy) {
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void Triangle::rotate(const f32 cx, const f32 cy, const f32 theta) {
        for (auto &vertex: m_vertices) {
            vertex.rotate(cx, cy, theta);
        }
    }

    void Triangle::writeToBatch(BatchRenderer &batchRenderer) const {
        auto vertices = batchRenderer.allocateVertices(0, 3);
        *vertices++ = m_vertices[0].getSDLVertex();
        *vertices++ = m_vertices[1].getSDLVertex();
        *vertices = m_vertices[2].getSDLVertex();
    }
}