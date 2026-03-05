//
// Created by donghao on 25-12-7.
//
#include <core/tool/BoundingBox.hpp>
namespace dao {
    BoundingBox::BoundingBox(const f32 left, const f32 top, const f32 right, const f32 bottom)
               : m_left(left), m_top(top), m_right(right), m_bottom(bottom) {
    }

    void BoundingBox::set(const f32 left, const f32 top, const f32 right, const f32 bottom) {
        m_left = left;
        m_top = top;
        m_right = right;
        m_bottom = bottom;
    }

    void BoundingBox::moveHorizontal(const f32 distance) {
        m_left += distance;
        m_right += distance;
    }

    void BoundingBox::moveVertical(const f32 distance) {
        m_top += distance;
        m_bottom += distance;
    }

    void BoundingBox::normalize() {
        if (m_left > m_right) std::swap(m_left, m_right);
        if (m_top > m_bottom) std::swap(m_top, m_bottom);
    }

    bool BoundingBox::isValid() const {
        return m_left <= m_right && m_top <= m_bottom;
    }

    bool BoundingBox::isInBoundingBox(const f32 x, const f32 y) const {
        return (x >= m_left && x <= m_right) && (y >= m_top && y <= m_bottom);
    }

    bool BoundingBox::isIntersects(const BoundingBox &other) const {
        return std::max(m_left, other.m_left) <= std::min(m_right, other.m_right) &&
               std::max(m_top, other.m_top) <= std::min(m_bottom, other.m_bottom);
    }
}