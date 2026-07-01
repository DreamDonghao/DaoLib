#include <components/graphs/Ellipse.hpp>

namespace dao {
    Ellipse::Ellipse(const f32 cx, const f32 cy, const f32 radiusX, const f32 radiusY,
                     const ColorRGBA &color, const i32 segments)
        : m_cx(cx), m_cy(cy), m_radiusX(radiusX), m_radiusY(radiusY), m_segments(segments) {
        updateVertices(color);
    }

    void Ellipse::setPosition(const f32 cx, const f32 cy) {
        const f32 dx = cx - m_cx;
        const f32 dy = cy - m_cy;
        translate(dx, dy);
    }

    void Ellipse::setRadius(const f32 radiusX, const f32 radiusY) {
        m_radiusX = radiusX;
        m_radiusY = radiusY;
        updateVertices(m_vertices[0].color());
    }

    void Ellipse::setColor(const ColorRGBA &color) {
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void Ellipse::setSegments(const i32 segments) {
        m_segments = segments;
        updateVertices(m_vertices[0].color());
    }

    void Ellipse::translate(const f32 dx, const f32 dy) {
        m_cx += dx;
        m_cy += dy;
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void Ellipse::rotate(const f32 cx, const f32 cy, const f32 theta) {
        std::tie(m_cx, m_cy) = rotatePointRad(m_cx, m_cy, cx, cy, theta);
        for (auto &vertex: m_vertices) {
            vertex.rotate(cx, cy, theta);
        }
    }

    void Ellipse::writeToBatch(BatchRenderer &batchRenderer) const {
        auto vertices = batchRenderer.allocateVertices(0, m_segments * 3);
        for (i32 i = 0; i < m_segments; ++i) {
            *vertices++ = m_vertices[0].getSDLVertex();
            *vertices++ = m_vertices[i + 1].getSDLVertex();
            *vertices++ = m_vertices[(i + 1) % m_segments + 1].getSDLVertex();
        }
    }

    void Ellipse::updateVertices(const ColorRGBA &color) {
        m_vertices.resize(m_segments + 1);
        m_vertices[0] = Vertex(m_cx, m_cy, color);

        const f32 angleStep = 2.0f * f32pi / static_cast<f32>(m_segments);
        for (i32 i = 0; i < m_segments; ++i) {
            const f32 angle = i * angleStep;
            const f32 x = m_cx + m_radiusX * std::cos(angle);
            const f32 y = m_cy + m_radiusY * std::sin(angle);
            m_vertices[i + 1] = Vertex(x, y, color);
        }
    }
}