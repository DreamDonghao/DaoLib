#pragma once
#include <component/graph/IGraph.hpp>
#include <core/tool/Point.hpp>

namespace dao {
    class Polygon : public ifc::IGraph {
    public:
        Polygon(const std::initializer_list<Vertex> &vertices)
            : m_vertexes(vertices) {
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            if (m_dirty) {
                updateRenderBase();
            }
            for (const auto &triangle: m_triangles) {
                batchRenderer.addToBatch(triangle);
            }
        }

        void translate(const f32 dx, const f32 dy) override {
            for (auto &vertex: m_vertexes) {
                vertex.translate(dx, dy);
            }
            m_dirty = true;
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            for (auto &vertex: m_vertexes) {
                auto [rx,ry] = rotatePointRad(vertex.x(), vertex.y(), cx, cy, theta);
                vertex.setPosition(rx, ry);
            }
            m_dirty = true;
        }

    private:
        std::vector<Vertex> m_vertexes;

        mutable bool m_dirty{true};
        mutable std::vector<Triangle> m_triangles;


        void updateRenderBase() const {
            m_triangles.resize(m_vertexes.size() - 2);
            for (size_t i = 2; i < m_vertexes.size(); ++i) {
                m_triangles[i - 2] = {m_vertexes[0], m_vertexes[i - 1], m_vertexes[i]};
            }
            m_dirty = false;
        }
    };
}
