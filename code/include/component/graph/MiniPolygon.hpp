#pragma once
#include <component/graph/IGraph.hpp>
#include <core/tool/Point.hpp>

namespace dao {
    /// @brief 多边形图形
    /// @details 由多个顶点组成的多边形，支持平移和旋转操作
    /// 内部将多边形分割为多个三角形进行渲染
    class Polygon : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param vertexes 顶点初始化列表
        explicit Polygon(const std::initializer_list<Vertex> &vertexes)
            : m_vertexes(vertexes) {
        }

        /// @brief 将多边形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            if (m_dirty) {
                updateRenderBase();
            }
            batchRenderer.addToBatch(m_triangles);
        }

        /// @brief 平移多边形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        void translate(const f32 dx, const f32 dy) override {
            for (auto &vertex: m_vertexes) {
                vertex.translate(dx, dy);
            }
            m_dirty = true;
        }

        /// @brief 旋转多边形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            for (auto &vertex: m_vertexes) {
                auto [rx,ry] = rotatePointRad(vertex.x(), vertex.y(), cx, cy, theta);
                vertex.setPosition(rx, ry);
            }
            m_dirty = true;
        }

    private:
        std::vector<Vertex> m_vertexes; ///< 顶点数组

        mutable bool m_dirty{true};                ///< 标记是否需要更新渲染基元
        mutable std::vector<Triangle> m_triangles; ///< 三角形数组

        /// @brief 更新渲染基元，将多边形分割为三角形
        void updateRenderBase() const {
            m_triangles.resize(m_vertexes.size() - 2);
            for (size_t i = 2; i < m_vertexes.size(); ++i) {
                m_triangles[i - 2] = {m_vertexes[0], m_vertexes[i - 1], m_vertexes[i]};
            }
            m_dirty = false;
        }
    };
}
