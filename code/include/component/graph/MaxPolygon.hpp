#pragma once
#include <component/graph/IGraph.hpp>
#include <core/tool/Point.hpp>

namespace dao {
    /// @brief 多边形图形
    /// @details 由多个顶点组成的多边形，支持平移和旋转操作
    /// 内部将多边形分割为多个三角形进行渲染
    class MaxPolygon : public ifc::IGraph {
    public:
        /// @brief 默认构造函数
        MaxPolygon() = default;

        /// @brief 构造函数
        /// @param vertexes 顶点初始化列表
        explicit MaxPolygon(const std::initializer_list<Vertex> &vertexes)
            : m_vertexes(vertexes) {
        }

        /// @brief 设置顶点数组
        /// @param vertexes 顶点数组
        void setVertexes(std::vector<Vertex> vertexes) {
            m_vertexes = std::move(vertexes);
            m_dirty = true;
        }

        /// @brief 将多边形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            if (m_dirty) {
                updateRenderBase();
            }
            batchRenderer.addToBatch(m_sdl_vertexes, m_indexes);
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

        mutable bool m_dirty{true}; ///< 标记是否需要更新渲染基元
        mutable std::vector<SDL_Vertex> m_sdl_vertexes;
        mutable std::vector<int> m_indexes;
        /// @brief 更新渲染基元，将多边形分割为三角形
        /// @details 使用扇形三角化（Fan Triangulation），以第一个顶点为公共顶点
        void updateRenderBase() const {
            const size_t vertexCount = m_vertexes.size();
            if (vertexCount < 3) {
                m_dirty = false;
                return;
            }

            // 转换顶点格式（原地更新或扩容）
            m_sdl_vertexes.resize(vertexCount);
            for (size_t i = 0; i < vertexCount; ++i) {
                m_sdl_vertexes[i] = static_cast<SDL_Vertex>(m_vertexes[i]);
            }

            // 索引模式固定，仅在顶点数变化时重新生成
            const size_t triangleCount = vertexCount - 2;
            if (const size_t indexCount = triangleCount * 3; m_indexes.size() != indexCount) {
                m_indexes.resize(indexCount);
                for (size_t i = 0; i < triangleCount; ++i) {
                    m_indexes[i * 3 + 0] = 0;
                    m_indexes[i * 3 + 1] = static_cast<int>(i + 1);
                    m_indexes[i * 3 + 2] = static_cast<int>(i + 2);
                }
            }

            m_dirty = false;
        }
    };
}
