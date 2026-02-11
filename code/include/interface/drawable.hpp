//
// Created by donghao on 25-12-8.
//
#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP
#include "core/frame/vertex_batch_builder.hpp"

namespace dao {
    /// @brief 可绘制对象接口
    class Drawable {
    public:
        virtual ~Drawable() = default;

        /// @brief 写入批处理
        virtual void writeToBatch(VertexBatchBuilder &builder) const = 0;
    };
}
#endif //DRAWABLE_HPP
