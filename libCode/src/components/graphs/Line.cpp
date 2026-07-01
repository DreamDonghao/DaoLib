#include <components/graphs/Line.hpp>

namespace dao {
    Line::Line(const f32 x1, const f32 y1, const f32 x2, const f32 y2,
               const f32 thickness, const ColorRGBA &color)
        : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_thickness(thickness) {
        updateVertices(color);
    }

    void Line::setStart(const f32 x, const f32 y) {
        m_x1 = x;
        m_y1 = y;
        updateVertices(m_vertices[0].color());
    }

    void Line::setEnd(const f32 x, const f32 y) {
        m_x2 = x;
        m_y2 = y;
        updateVertices(m_vertices[0].color());
    }

    void Line::setThickness(const f32 thickness) {
        m_thickness = thickness;
        updateVertices(m_vertices[0].color());
    }

    void Line::setColor(const ColorRGBA &color) {
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void Line::translate(const f32 dx, const f32 dy) {
        m_x1 += dx;
        m_y1 += dy;
        m_x2 += dx;
        m_y2 += dy;
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void Line::rotate(const f32 cx, const f32 cy, const f32 theta) {
        std::tie(m_x1, m_y1) = rotatePointRad(m_x1, m_y1, cx, cy, theta);
        std::tie(m_x2, m_y2) = rotatePointRad(m_x2, m_y2, cx, cy, theta);
        updateVertices(m_vertices[0].color());
    }

    void Line::writeToBatch(BatchRenderer &batchRenderer) const {
        auto vertices = batchRenderer.allocateVertices(0, 6);
        *vertices++ = m_vertices[0].getSDLVertex();
        *vertices++ = m_vertices[1].getSDLVertex();
        *vertices++ = m_vertices[2].getSDLVertex();
        *vertices++ = m_vertices[1].getSDLVertex();
        *vertices++ = m_vertices[3].getSDLVertex();
        *vertices++ = m_vertices[2].getSDLVertex();
    }

    void Line::updateVertices(const ColorRGBA &color) {
        m_vertices.resize(4);

        const f32 dx = m_x2 - m_x1;
        const f32 dy = m_y2 - m_y1;
        const f32 len = std::sqrt(dx * dx + dy * dy);

        if (len < 1e-6f) {
            m_vertices[0] = Vertex(m_x1 - m_thickness / 2, m_y1 - m_thickness / 2, color);
            m_vertices[1] = Vertex(m_x1 + m_thickness / 2, m_y1 - m_thickness / 2, color);
            m_vertices[2] = Vertex(m_x1 - m_thickness / 2, m_y1 + m_thickness / 2, color);
            m_vertices[3] = Vertex(m_x1 + m_thickness / 2, m_y1 + m_thickness / 2, color);
            return;
        }

        const f32 nx = -dy / len;
        const f32 ny = dx / len;

        const f32 half = m_thickness / 2.0f;
        const f32 offsetX = nx * half;
        const f32 offsetY = ny * half;

        m_vertices[0] = Vertex(m_x1 + offsetX, m_y1 + offsetY, color);
        m_vertices[1] = Vertex(m_x1 - offsetX, m_y1 - offsetY, color);
        m_vertices[2] = Vertex(m_x2 + offsetX, m_y2 + offsetY, color);
        m_vertices[3] = Vertex(m_x2 - offsetX, m_y2 - offsetY, color);
    }
}