#pragma once
#include <interface/IDrawable.hpp>

namespace dao::ifc {
    class IGraph : public IDrawable {
    public:
        virtual void translate(f32 dx, f32 dy) = 0;

        virtual void rotate(f32 cx,f32 cy,f32 theta) = 0;

        void writeToBatch(BatchRenderer &batchRenderer) const override = 0;

        ~IGraph() override = default;
    };
} // dao
