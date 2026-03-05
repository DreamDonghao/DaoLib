#pragma once
#include <core/frame/Tray.hpp>
#include <core/frame/Context.hpp>

class AppTray : public dao::Tray {
public:
    AppTray(const std::string_view iconPath, const std::string_view tooltip)
        : Tray(iconPath, tooltip) {
    }

    void createTrayMenu() override;

private:
    dao::TrayFunc example = [](SDL_TrayEntry *entry) {
        std::cout<<"托盘按钮"<<std::endl;
    };
};
