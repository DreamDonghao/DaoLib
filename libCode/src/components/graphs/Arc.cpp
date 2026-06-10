#include <components/graphs/Arc.hpp>

namespace dao {
    Arc::Arc(const f32 cx, const f32 cy, const f32 radius,
             const f32 startAngle, const f32 endAngle,
             const f32 thickness, const ColorRGBA &color,
             const i32 segments)
        : m_cx(cx), m_cy(cy), m_radius(radius),
          m_startAngle(startAngle), m_endAngle(endAngle),
          m_thickness(thickness), m_segments(segments) {
        updateVertices(color);
    }

    void Arc::setPosition(const f32 cx, const f32 cy) {
        const f32 dx = cx - m_cx;
        const f32 dy = cy - m_cy;
        translate(dx, dy);
    }

    void Arc::setRadius(const f32 radius) {
        m_radius = radius;
        updateVertices(m_vertices[0].color());
    }

    void Arc::setAngles(const f32 startAngle, const f32 endAngle) {
        m_startAngle = startAngle;
        m_endAngle = endAngle;
        updateVertices(m_vertices[0].color());
    }

    void Arc::setThickness(const f32 thickness) {
        m_thickness = thickness;
        updateVertices(m_vertices[0].color());
    }

    void Arc::setColor(const ColorRGBA &color) {
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void Arc::setSegments(const i32 segments) {
        m_segments = segments;
        updateVertices(m_vertices[0].color());
    }

    void Arc::translate(const f32 dx, const f32 dy) {
        m_cx += dx;
        m_cy += dy;
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void Arc::rotate(const f32 cx, const f32 cy, const f32 theta) {
        std::tie(m_cx, m_cy) = rotatePointRad(m_cx, m_cy, cx, cy, theta);
        m_startAngle += theta;
        m_endAngle += theta;
        updateVertices(m_vertices[0].color());
    }

    void Arc::writeToBatch(BatchRenderer &batchRenderer) const {
        auto vertices = batchRenderer.allocateVertices(0, m_segments * 6);
        for (i32 i = 0; i < m_segments; ++i) {
            *vertices++ = m_vertices[i * 2].getSDLVertex();
            *vertices++ = m_vertices[i * 2 + 1].getSDLVertex();
            *vertices++ = m_vertices[i * 2 + 2].getSDLVertex();
            *vertices++ = m_vertices[i * 2 + 1].getSDLVertex();
            *vertices++ = m_vertices[i * 2 + 3].getSDLVertex();
            *vertices++ = m_vertices[i * 2 + 2].getSDLVertex();
        }
    }

    void Arc::updateVertices(const ColorRGBA &color) {
        m_vertices.resize((m_segments + 1) * 2);

        const f32 angleStep = (m_endAngle - m_startAngle) / static_cast<f32>(m_segments);
        const f32 innerR = m_radius - m_thickness / 2.0f;
        const f32 outerR = m_radius + m_thickness / 2.0f;

        for (i32 i = 0; i <= m_segments; ++i) {
            const f32 angle = m_startAngle + i * angleStep;
            const f32 cosA = std::cos(angle);
            const f32 sinA = std::sin(angle);

            m_vertices[i * 2] = Vertex(
                m_cx + innerR * cosA,
                m_cy + innerR * sinA,
                color);
            m_vertices[i * 2 + 1] = Vertex(
                m_cx + outerR * cosA,
                m_cy + outerR * sinA,
                color);
        }
    }
}