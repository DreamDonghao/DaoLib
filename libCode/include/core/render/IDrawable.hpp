#pragma once
#include <core/render/BatchRenderer.hpp>

namespace dao::ifc {
    /// @brief 可绘制对象接口
    class IDrawable {
    public:
        virtual ~IDrawable() = default;

        /// @brief 写入批处理
        virtual void writeToBatch(BatchRenderer &batchRenderer) const = 0;
    };
}
