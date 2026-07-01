#include <components/graphs/Ring.hpp>

namespace dao {
    Ring::Ring(const f32 cx, const f32 cy, const f32 innerRadius, const f32 outerRadius,
               const ColorRGBA &color, const i32 segments)
        : m_cx(cx), m_cy(cy), m_innerRadius(innerRadius), m_outerRadius(outerRadius),
          m_segments(segments) {
        updateVertices(color);
    }

    void Ring::setPosition(const f32 cx, const f32 cy) {
        const f32 dx = cx - m_cx;
        const f32 dy = cy - m_cy;
        translate(dx, dy);
    }

    void Ring::setInnerRadius(const f32 innerRadius) {
        m_innerRadius = innerRadius;
        updateVertices(m_vertices[0].color());
    }

    void Ring::setOuterRadius(const f32 outerRadius) {
        m_outerRadius = outerRadius;
        updateVertices(m_vertices[0].color());
    }

    void Ring::setColor(const ColorRGBA &color) {
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void Ring::setSegments(const i32 segments) {
        m_segments = segments;
        updateVertices(m_vertices[0].color());
    }

    void Ring::translate(const f32 dx, const f32 dy) {
        m_cx += dx;
        m_cy += dy;
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void Ring::rotate(const f32 cx, const f32 cy, const f32 theta) {
        std::tie(m_cx, m_cy) = rotatePointRad(m_cx, m_cy, cx, cy, theta);
        for (auto &vertex: m_vertices) {
            vertex.rotate(cx, cy, theta);
        }
    }

    void Ring::writeToBatch(BatchRenderer &batchRenderer) const {
        auto vertices = batchRenderer.allocateVertices(0, m_segments * 6);
        for (i32 i = 0; i < m_segments; ++i) {
            const i32 next = (i + 1) % m_segments;
            *vertices++ = m_vertices[i].getSDLVertex();
            *vertices++ = m_vertices[i + m_segments + 1].getSDLVertex();
            *vertices++ = m_vertices[next].getSDLVertex();
            *vertices++ = m_vertices[i + m_segments + 1].getSDLVertex();
            *vertices++ = m_vertices[next + m_segments + 1].getSDLVertex();
            *vertices++ = m_vertices[next].getSDLVertex();
        }
    }

    void Ring::updateVertices(const ColorRGBA &color) {
        m_vertices.resize((m_segments + 1) * 2);

        const f32 angleStep = 2.0f * f32pi / static_cast<f32>(m_segments);
        for (i32 i = 0; i <= m_segments; ++i) {
            const f32 angle = i * angleStep;
            const f32 cosA = std::cos(angle);
            const f32 sinA = std::sin(angle);

            m_vertices[i] = Vertex(
                m_cx + m_innerRadius * cosA,
                m_cy + m_innerRadius * sinA,
                color);
            m_vertices[i + m_segments + 1] = Vertex(
                m_cx + m_outerRadius * cosA,
                m_cy + m_outerRadius * sinA,
                color);
        }
    }
}