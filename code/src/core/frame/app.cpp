//
// Created by donghao on 25-12-6.
//
#include <core/frame/app.hpp>
#include <iostream>
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
                if (SDL_GetKeyboardFocus() == window->getSDLWindow()) {
                    const bool *key = SDL_GetKeyboardState(nullptr);
                }
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

    App::App() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "SDL_Init failed" << SDL_GetError() << std::endl;
        }
        SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "1");
        if (!TTF_Init()) {
            SDL_Log("1TTF_Init failed: %s", SDL_GetError());
        }
    }

    void App::render() {
        for (const auto &window: m_windows | std::views::values) {
            window->render();
        }
    }
}
