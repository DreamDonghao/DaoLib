//
// Created by donghao on 25-12-7.
//
#include <core/tool/bounding_box.hpp>
namespace dao {
    BoundingBox::BoundingBox(const float left, const float up, const float right, const float down)
               : m_left(left), m_up(up), m_right(right), m_down(down) {
    }

    void BoundingBox::set(const float left, const float up, const float right, const float down) {
        m_left = left;
        m_up = up;
        m_right = right;
        m_down = down;
    }

    void BoundingBox::moveHorizontal(const float distance) {
        m_left += distance;
        m_right += distance;
    }

    void BoundingBox::moveVertical(const float distance) {
        m_up += distance;
        m_down += distance;
    }

    void BoundingBox::normalize() {
        if (m_left > m_right) std::swap(m_left, m_right);
        if (m_up > m_down) std::swap(m_up, m_down);
    }

    bool BoundingBox::isValid() const {
        return m_left <= m_right && m_up <= m_down;
    }

    bool BoundingBox::isInBoundingBox(const float x, const float y) const {
        return (x >= m_left && x <= m_right) && (y >= m_up && y <= m_down);
    }

    bool BoundingBox::isIntersects(const BoundingBox &other) const {
        return std::max(m_left, other.m_left) <= std::min(m_right, other.m_right) &&
               std::max(m_up, other.m_up) <= std::min(m_down, other.m_down);
    }
}
