#include <components/graphs/RoundedRectangle.hpp>

namespace dao {
    RoundedRectangle::RoundedRectangle(const f32 x, const f32 y, const f32 width, const f32 height,
                                       const f32 cornerRadius, const ColorRGBA &color,
                                       const i32 cornerSegments)
        : m_x(x), m_y(y), m_width(width), m_height(height),
          m_cornerRadius(cornerRadius), m_cornerSegments(cornerSegments), m_color(color) {
        updateVertices();
    }

    void RoundedRectangle::setPosition(const f32 x, const f32 y) {
        const f32 dx = x - m_x;
        const f32 dy = y - m_y;
        translate(dx, dy);
    }

    void RoundedRectangle::setSize(const f32 width, const f32 height) {
        m_width = width;
        m_height = height;
        updateVertices();
    }

    void RoundedRectangle::setCornerRadius(const f32 radius) {
        m_cornerRadius = radius;
        updateVertices();
    }

    void RoundedRectangle::setColor(const ColorRGBA &color) {
        m_color = color;
        for (auto &vertex: m_vertices) {
            vertex.setColor(color);
        }
    }

    void RoundedRectangle::setCornerSegments(const i32 segments) {
        m_cornerSegments = segments;
        updateVertices();
    }

    void RoundedRectangle::translate(const f32 dx, const f32 dy) {
        m_x += dx;
        m_y += dy;
        for (auto &vertex: m_vertices) {
            vertex.translate(dx, dy);
        }
    }

    void RoundedRectangle::rotate(const f32 cx, const f32 cy, const f32 theta) {
        for (auto &vertex: m_vertices) {
            vertex.rotate(cx, cy, theta);
        }
        m_x = m_vertices[0].x();
        m_y = m_vertices[0].y();
    }

    void RoundedRectangle::writeToBatch(BatchRenderer &batchRenderer) const {
        const i32 vertexCount = static_cast<i32>(m_vertices.size());
        auto vertices = batchRenderer.allocateVertices(0, (vertexCount - 1) * 3);
        i32 i;
        for (i = 1; i < vertexCount - 1; ++i) {
            *vertices++ = m_vertices[0].getSDLVertex();
            *vertices++ = m_vertices[i].getSDLVertex();
            *vertices++ = m_vertices[i + 1].getSDLVertex();
        }
        *vertices++ = m_vertices[0].getSDLVertex();
        *vertices++ = m_vertices[i].getSDLVertex();
        *vertices++ = m_vertices[1].getSDLVertex();
    }

    void RoundedRectangle::updateVertices() {
        const f32 maxRadius = std::min(m_width, m_height) / 2.0f;
        const f32 radius = std::min(m_cornerRadius, maxRadius);

        const f32 right = m_x + m_width;
        const f32 bottom = m_y + m_height;

        const i32 cornerPointCount = m_cornerSegments + 1;
        const i32 totalVertices = 1 + 4 * cornerPointCount;
        m_vertices.resize(totalVertices);

        const f32 centerX = m_x + m_width / 2.0f;
        const f32 centerY = m_y + m_height / 2.0f;
        m_vertices[0] = Vertex(centerX, centerY, m_color);

        i32 vertexIndex = 1;
        const f32 angleStep = f32pi / 2.0f / static_cast<f32>(m_cornerSegments);

        // 左上角圆角
        const f32 lt_cx = m_x + radius;
        const f32 lt_cy = m_y + radius;
        for (i32 i = 0; i <= m_cornerSegments; ++i) {
            const f32 angle = f32pi + i * angleStep;
            m_vertices[vertexIndex++] = Vertex(
                lt_cx + radius * std::cos(angle),
                lt_cy + radius * std::sin(angle),
                m_color);
        }

        // 右上角圆角
        const f32 rt_cx = right - radius;
        const f32 rt_cy = m_y + radius;
        for (i32 i = 0; i <= m_cornerSegments; ++i) {
            const f32 angle = -f32pi / 2.0f + i * angleStep;
            m_vertices[vertexIndex++] = Vertex(
                rt_cx + radius * std::cos(angle),
                rt_cy + radius * std::sin(angle),
                m_color);
        }

        // 右下角圆角
        const f32 rb_cx = right - radius;
        const f32 rb_cy = bottom - radius;
        for (i32 i = 0; i <= m_cornerSegments; ++i) {
            const f32 angle = i * angleStep;
            m_vertices[vertexIndex++] = Vertex(
                rb_cx + radius * std::cos(angle),
                rb_cy + radius * std::sin(angle),
                m_color);
        }

        // 左下角圆角
        const f32 lb_cx = m_x + radius;
        const f32 lb_cy = bottom - radius;
        for (i32 i = 0; i <= m_cornerSegments; ++i) {
            const f32 angle = f32pi / 2.0f + i * angleStep;
            m_vertices[vertexIndex++] = Vertex(
                lb_cx + radius * std::cos(angle),
                lb_cy + radius * std::sin(angle),
                m_color);
        }
    }
}