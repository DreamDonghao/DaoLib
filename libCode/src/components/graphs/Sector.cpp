#include <components/graphs/Sector.hpp>

namespace dao {
    Sector::Sector(const f32 cx, const f32 cy, const f32 radius,
                   const f32 startAngle, const f32 endAngle,
                   const ColorRGBA &color, const i32 segments)
        : m_cx(cx), m_cy(cy), m_radius(radius),
          m_startAngle(startAngle), m_endAngle(endAngle), m_segments(segments) {
        updateVertices(color);
    }

    void Sector::setPosition(const f32 cx, const f32 cy) {
        const f32 dx = cx - m_cx;
        const f32 dy = cy - m_cy;
        translate(dx, dy);
    }

    void Sector::setRadius(const f32 radius) {
        m_radius = radius;
        updateVertices(m_vertices[0].color());
    }

    void Sector::setAngles(const f32 startAngle, const f32 endAngle) {
        m_startAngle = startAngle;
        m_endAngle = endAngle;
        updateVertices(m_vertices[0].color());
    }

    void Sector::setColor(const ColorRGBA &color) {
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void Sector::setSegments(const i32 segments) {
        m_segments = segments;
        updateVertices(m_vertices[0].color());
    }

    void Sector::translate(const f32 dx, const f32 dy) {
        m_cx += dx;
        m_cy += dy;
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void Sector::rotate(const f32 cx, const f32 cy, const f32 theta) {
        std::tie(m_cx, m_cy) = rotatePointRad(m_cx, m_cy, cx, cy, theta);
        m_startAngle += theta;
        m_endAngle += theta;
        updateVertices(m_vertices[0].color());
    }

    void Sector::writeToBatch(BatchRenderer &batchRenderer) const {
        auto vertices = batchRenderer.allocateVertices(0, m_segments * 3);
        for (i32 i = 0; i < m_segments; ++i) {
            *vertices++ = m_vertices[0].getSDLVertex();
            *vertices++ = m_vertices[i + 1].getSDLVertex();
            *vertices++ = m_vertices[i + 2].getSDLVertex();
        }
    }

    void Sector::updateVertices(const ColorRGBA &color) {
        m_vertices.resize(m_segments + 2);
        m_vertices[0] = Vertex(m_cx, m_cy, color);

        const f32 angleStep = (m_endAngle - m_startAngle) / static_cast<f32>(m_segments);
        for (i32 i = 0; i <= m_segments; ++i) {
            const f32 angle = m_startAngle + i * angleStep;
            const f32 x = m_cx + m_radius * std::cos(angle);
            const f32 y = m_cy + m_radius * std::sin(angle);
            m_vertices[i + 1] = Vertex(x, y, color);
        }
    }
}