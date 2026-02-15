//
// Created by donghao on 25-12-6.
//
#include <core/frame/app.hpp>
#include <ranges>
#include <SDL3_ttf/SDL_ttf.h>

namespace dao {
    void App::run() {
        while (m_runWindowNum) {
            m_runWindowNum = 0;
            for (const auto &window: m_windows | std::views::values) {
                render();
                window->update();
                m_runWindowNum += window->isRunning();
            }
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                    uint32 wid = event.window.windowID;
                    if (auto it = m_windows.find(wid); it != m_windows.end()) {
                        it->second->requestClose();
                    }
                }
                SDL_Window *focus = SDL_GetMouseFocus();
                if (!focus) {
                    continue;
                }
                auto wid = SDL_GetWindowID(focus);
                m_windows[wid]->handleInputEvent(event);
            }
        }
        SDL_Quit();
    }

    Window &App::createWindow(uint32 width, uint32 height,
                              bool resizable, bool transparent, bool onTop, bool borderless) {
        auto nowWindow = std::make_unique<Window>(width, height, resizable, transparent, onTop, borderless);
        const uint32 windowId = nowWindow->getId();
        m_windows[windowId] = std::move(nowWindow);
        return *m_windows[windowId];
    }

    Tray &App::createTray(const std::string_view iconPath, const std::string_view tooltip) {
        m_tray = std::make_unique<Tray>(iconPath, tooltip);
        return *m_tray;
    }

    App::App() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            DAO_ERROR_LOG(std::string("初始化 SDL 失败 ") + SDL_GetError());
        }
        if (!SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "1")) {
            DAO_ERROR_LOG(std::string("初始化 SDL_Hint 失败 ") + SDL_GetError());
        }
        if (!TTF_Init()) {
            DAO_ERROR_LOG(std::string("初始化 SDL_TTF 失败 ") + SDL_GetError());
        }
    }

    void App::render() {
        for (const auto &window: m_windows | std::views::values) {
            window->render();
        }
    }
}
