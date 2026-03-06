#pragma once
#include <interface/IDrawable.hpp>

namespace dao::ifc {
    /// @brief 图形接口
    /// @details 所有几何图形类需要实现的接口，支持平移、旋转和批处理渲染
    class IGraph : public IDrawable {
    public:
        /// @brief 平移图形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        virtual void translate(f32 dx, f32 dy) = 0;

        /// @brief 旋转图形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        virtual void rotate(f32 cx, f32 cy, f32 theta) = 0;

        /// @brief 将图形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override = 0;

        /// @brief 析构函数
        ~IGraph() override = default;
    };
}