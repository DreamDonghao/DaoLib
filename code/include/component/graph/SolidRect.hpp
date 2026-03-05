#pragma once
#include <component/graph/Rect.hpp>

namespace dao {
    class SolidRect : public ifc::IGraph {
    public:
        SolidRect(const f32 x, const f32 y, const f32 width, const f32 height, const ColorRGBA &color)
            : m_rect(x, y, width, height, color, color, color, color) {
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            m_rect.writeToBatch(batchRenderer);
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            m_rect.rotate(cx, cy, theta);
        }

        void translate(const f32 dx, const f32 dy) override {
            m_rect.translate(dx, dy);
        }

    private:
        Rect m_rect;
    };
}
