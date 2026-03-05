#pragma once
#include <component/graph/Polygon.hpp>

namespace dao {
    class Rect : public ifc::IGraph {
    public:
        Rect(const f32 x, const f32 y, const f32 width, const f32 height,
            const ColorRGBA &colorA,const ColorRGBA &colorB,const ColorRGBA &colorC,const ColorRGBA &colorD)
        :m_polygon({Vertex{x, y, colorA}, Vertex{x + width, y, colorB},
              Vertex{x + width, y + height, colorC}, Vertex{x, y + height, colorD}}) {
        }

        void translate(const f32 dx, const f32 dy) override {
           m_polygon.translate(dx, dy);
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            m_polygon.rotate(cx,cy,theta);
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            m_polygon.writeToBatch(batchRenderer);
        }

    private:
       Polygon m_polygon;
    };
}
