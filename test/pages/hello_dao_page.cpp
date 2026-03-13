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

std::vector<i32> HelloDaoPage::getRegisterTextures() const {
    return {};
}
i32 id;
void HelloDaoPage::open() {
    id = client.postAsync("/post",R"({"你好":"1"})");
}

void HelloDaoPage::close() {
}

void HelloDaoPage::update() {
    clearBatch();
    std::string st;
    if (client.isReady(id)) {
        st = client.getResponse(id)->body;
    }
    Text text{0, 0, 100, Yellow, utf8ToUtf32(st)};
    addToBatch(text);
    addToBatch();
}


void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
}
