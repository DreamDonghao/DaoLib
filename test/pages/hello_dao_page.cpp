#include "hello_dao_page.hpp"
#include <components/graphs/Circle.hpp>
#include <components/graphs/Polygon.hpp>
#include <components/Text.hpp>
using namespace dao;

long long getTimeInterval() {
    static auto start_time = std::chrono::high_resolution_clock::now(); // 记录第一次调用的时间
    const auto now = std::chrono::high_resolution_clock::now();

    // 计算从第一次调用到现在的时间差，单位为毫秒
    const auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

    // 更新开始时间为当前时间，以便下一次调用时计算新时间间隔
    start_time = now;

    return interval;
}

std::vector<i32> HelloDaoPage::getRegisterTexture() const {
    return {};
}

void HelloDaoPage::open() {
}

void HelloDaoPage::close() {
}

Circle circle(100,100,100);
Polygon<4> polygon(
    Vertex(0, 0, Red),
    Vertex(100, 0, Red),
    Vertex(100, 100, Red),
    Vertex(0, 100, Red)
);
dao::Text text(100,100,100,ColorRGBA(10,200,100,0.02),U"你好");
void HelloDaoPage::update() {
    clearBatch();
    circle.rotate(200,200,0.1);
    addToBatch(circle,text);
    for (int i=0;i<100;i++) {
        addToBatch(polygon,text);
    }
}


void HelloDaoPage::handleInputEvent(const SDL_Event &event) {

}
