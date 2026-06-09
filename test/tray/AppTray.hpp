#pragma once
#include <core/frame/Tray.hpp>
#include <core/frame/Context.hpp>

#include "core/frame/App.hpp"
#include "core/frame/Window.hpp"

class AppTray : public dao::Tray {
public:
    AppTray(const std::string_view iconPath, const std::string_view tooltip)
        : Tray(iconPath, tooltip) {
    }

    void createTrayMenu() override;

private:
    dao::TrayFunc settings = [this](SDL_TrayEntry *entry) {
        getContext().get<dao::App>()->convertWindowWorkState("SettingsPage",dao::Window::WorkState::Foreground);
    };

    dao::TrayFunc quit = [this](SDL_TrayEntry *entry) {
        getContext().get<dao::App>()->exit();
    };
};
