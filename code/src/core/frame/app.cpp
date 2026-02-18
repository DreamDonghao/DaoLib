//
// Created by donghao on 25-12-6.
//
#include <core/frame/app.hpp>
#include <ranges>
#include <SDL3_ttf/SDL_ttf.h>

namespace dao {
    App::App(const uint32 fps, const bool clickThrough) : m_frameLimiter(fps) {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            DAO_ERROR_LOG(std::string("初始化 SDL 失败 ") + SDL_GetError());
        }
        if (!SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "1")) {
            DAO_ERROR_LOG(std::string("初始化 SDL_Hint 失败 ") + SDL_GetError());
        }
        if (!TTF_Init()) {
            DAO_ERROR_LOG(std::string("初始化 SDL_TTF 失败 ") + SDL_GetError());
        }
        if (!SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, clickThrough ? "1" : "0")) {
            DAO_ERROR_LOG("失焦点击生效设置失败");
        }
    }

    App::~App() {
        TTF_Quit();
        SDL_Quit();
    }

    void App::run() {
        m_running = true;
        while (m_running) {
            m_frameLimiter.wait();
            for (const auto &window: m_windows | std::views::values) {
                window->update();
                window->render();
                window->getAppController().executeCommand(*this);
            }
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                    uint32_t wid = event.window.windowID;
                    if (auto it = m_windows.find(wid); it != m_windows.end()) {
                        it->second->hide();
                    }
                }

                uint32_t wid = 0;
                if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
                    wid = event.window.windowID;
                } else if (event.type >= SDL_EVENT_KEY_DOWN && event.type <= SDL_EVENT_KEY_UP) {
                    wid = event.key.windowID;
                } else if (event.type >= SDL_EVENT_MOUSE_MOTION && event.type <= SDL_EVENT_MOUSE_WHEEL) {
                    wid = event.button.windowID;
                }

                if (wid != 0) {
                    if (auto it = m_windows.find(wid); it != m_windows.end()) {
                        it->second->handleInputEvent(event);
                    }
                }
            }
        }
    }

    void App::close() {
        for (const auto &window: m_windows | std::views::values) {
            window->destroy();
        }
        m_running = false;
    }

    Window &App::createWindow(
        uint32 width, uint32 height, const std::string_view tag,
        bool hidden, bool isSubject,
        bool resizable, bool transparent, bool onTop, bool borderless) {
        auto nowWindow = std::make_unique<Window>(
            width, height, hidden, isSubject,
            resizable, transparent, onTop, borderless);
        const uint32 windowId = nowWindow->getId();
        m_windowMap[tag] = windowId;
        m_windows[windowId] = std::move(nowWindow);
        m_windows[windowId]->setContext(&m_context);
        return *m_windows[windowId];
    }
}
