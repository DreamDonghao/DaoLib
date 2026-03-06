#pragma once
#include <component/graph/MiniPolygon.hpp>

#include "MaxPolygon.hpp"

namespace dao {
    /// @brief 矩形图形
    /// @details 由四个顶点组成的矩形，支持平移和旋转操作
    class Rect : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x 矩形左上角 x 坐标
        /// @param y 矩形左上角 y 坐标
        /// @param width 矩形宽度
        /// @param height 矩形高度
        /// @param colorA 左上角颜色
        /// @param colorB 右上角颜色
        /// @param colorC 右下角颜色
        /// @param colorD 左下角颜色
        Rect(const f32 x, const f32 y, const f32 width, const f32 height,
            const ColorRGBA &colorA,const ColorRGBA &colorB,const ColorRGBA &colorC,const ColorRGBA &colorD)
        :m_polygon({Vertex{x, y, colorA}, Vertex{x + width, y, colorB},
              Vertex{x + width, y + height, colorC}, Vertex{x, y + height, colorD}}) {
        }

        /// @brief 平移矩形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        void translate(const f32 dx, const f32 dy) override {
           m_polygon.translate(dx, dy);
        }

        /// @brief 旋转矩形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            m_polygon.rotate(cx,cy,theta);
        }

        /// @brief 将矩形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            m_polygon.writeToBatch(batchRenderer);
        }

    private:
       Polygon m_polygon; ///< 内部多边形
    };
}
