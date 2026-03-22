#include "hello_dao_page.hpp"
#include <components/graphs/Circle.hpp>
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


void HelloDaoPage::open() {
    client.setTimeout(100, 100);
}

void HelloDaoPage::close() {
}


void HelloDaoPage::update() {
    if (client.isReady(reqId)) {
        if (const auto resp = client.takeResponse(reqId); resp) {
            dao::Log{dao::LogLevel::INFO}("resp.body", resp.value().body);
            dao::json bodyJson = dao::json::parse(resp.value().body);
            textbox.setContent(dao::utf8ToUtf32(bodyJson["choices"][0]["message"]["content"].get<std::string>()));
        } else {
            std::cout << "Error: " << resp.value().error << resp.value().body << std::endl;
        }
    }


    clearBatch();
    addToBatch(inputBox, button, textbox);
}


void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
    inputBox.handleEvent(event);
    button.handleEvent(event);
}
