#pragma once
class TestStaticDraw {
public:
    explicit TestStaticDraw(dao::BatchRenderer &batchRenderer)
        : batchGroup(batchRenderer.allocateBatchGroup(10)) {
        const auto v = batchGroup.allocateVertices(0, 3);
        v[0] = dao::Vertex(0, 0, dao::White).getSDLVertex();
        v[1] = dao::Vertex(100, 100, dao::White).getSDLVertex();
        v[2] = dao::Vertex(200, 300, dao::White).getSDLVertex();
    }

    ~TestStaticDraw() {
        batchGroup.erase();
    }

private:
    dao::BatchGroup &batchGroup;
};
