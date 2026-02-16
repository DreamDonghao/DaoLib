#pragma once
#include <string_view>
#include "core/frame/tray.hpp"

class myTray : public dao::Tray {
public:
    myTray(const std::string_view iconPath, const std::string_view tooltip)
        : Tray(iconPath, tooltip) {
    }

    void createTrayMenu() override {
        button("设置", set);
        button("退出", quit);
    }

private:
    dao::TrayFunc set = [this](SDL_TrayEntry *entry) {
        getContext().service<dao::App *>()->showWindow("设置");
    };

    dao::TrayFunc quit = [this](SDL_TrayEntry *entry) {
        getContext().service<dao::App *>()->close();
    };
};
