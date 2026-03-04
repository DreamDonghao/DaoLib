#pragma once
#include <core/render/vertex_batch_builder.hpp>

namespace dao {
    /// @brief 可绘制对象接口
    class Drawable {
    public:
        virtual ~Drawable() = default;

        /// @brief 写入批处理
        virtual void writeToBatch(VertexBatchBuilder &builder) const = 0;
    };
}
