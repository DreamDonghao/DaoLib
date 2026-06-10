#include <components/graphs/Rectangle.hpp>

namespace dao {
    Rectangle::Rectangle(const f32 x, const f32 y, const f32 width, const f32 height,
                         const ColorRGBA &color)
        : m_x(x), m_y(y), m_width(width), m_height(height) {
        updateVertices(color);
    }

    void Rectangle::setPosition(const f32 x, const f32 y) {
        const f32 dx = x - m_x;
        const f32 dy = y - m_y;
        translate(dx, dy);
    }

    void Rectangle::setSize(const f32 width, const f32 height) {
        m_width = width;
        m_height = height;
        updateVertices(m_vertices[0].color());
    }

    void Rectangle::setColor(const ColorRGBA &color) {
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void Rectangle::translate(const f32 dx, const f32 dy) {
        m_x += dx;
        m_y += dy;
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void Rectangle::rotate(const f32 cx, const f32 cy, const f32 theta) {
        for (auto &vertex: m_vertices) {
            vertex.rotate(cx, cy, theta);
        }
        m_x = m_vertices[0].x();
        m_y = m_vertices[0].y();
    }

    void Rectangle::writeToBatch(BatchRenderer &batchRenderer) const {
        auto vertices = batchRenderer.allocateVertices(0, 6);
        *vertices++ = m_vertices[0].getSDLVertex();
        *vertices++ = m_vertices[1].getSDLVertex();
        *vertices++ = m_vertices[2].getSDLVertex();
        *vertices++ = m_vertices[1].getSDLVertex();
        *vertices++ = m_vertices[3].getSDLVertex();
        *vertices++ = m_vertices[2].getSDLVertex();
    }

    void Rectangle::updateVertices(const ColorRGBA &color) {
        m_vertices.resize(4);
        m_vertices[0] = Vertex(m_x, m_y, color);
        m_vertices[1] = Vertex(m_x + m_width, m_y, color);
        m_vertices[2] = Vertex(m_x, m_y + m_height, color);
        m_vertices[3] = Vertex(m_x + m_width, m_y + m_height, color);
    }
}