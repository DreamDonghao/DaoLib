#include <SDL3_ttf/SDL_ttf.h>
#include <core/frame/App.hpp>
#include <ranges>

namespace dao {
    App::App(const i32 fps, const bool clickThrough) : m_frameLimiter(fps) {
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
        m_windows.clear();  // 先销毁所有窗口，确保 TTF_CloseFont 在 TTF_Quit 之前调用
        TTF_Quit();
        SDL_Quit();
    }


    void App::run() {
        m_running = true;
        while (m_running) {
            m_frameLimiter.wait();
            for (const auto &window: m_windows | std::views::values) {
                if (window->workState() != Window::WorkState::Closed) {
                    window->update();
                    window->getAppController().executeCommand(*this);
                }
            }
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                // 处理窗口关闭按钮（左上角）点击事件
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                    const i32 wid = static_cast<i32>(event.window.windowID);
                    if (auto it = m_windows.find(wid); it != m_windows.end()) {
                        it->second->convertWorkState(Window::WorkState::Closed);
                    }
                }

                i32 wid = 0;
                if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
                    wid = static_cast<i32>(event.window.windowID);
                } else if (event.type >= SDL_EVENT_KEY_DOWN && event.type <= SDL_EVENT_KEY_UP) {
                    wid = static_cast<i32>(event.key.windowID);
                } else if (event.type >= SDL_EVENT_MOUSE_MOTION && event.type <= SDL_EVENT_MOUSE_WHEEL) {
                    wid = static_cast<i32>(event.button.windowID);
                }

                if (wid != 0) {
                    if (auto it = m_windows.find(wid); it != m_windows.end()) {
                        it->second->handleInputEvent(event);
                    }
                }
            }
        }
        for (const auto &window: m_windows | std::views::values) {
            window->convertWorkState(Window::WorkState::Closed);
        }
    }

    void App::exit() { m_running = false; }

    Window &App::createWindow(i32 width, i32 height, const std::string_view tag, Window::WorkState workState,
                              bool isSubject, bool resizable, bool transparent, bool onTop, bool borderless) {
        auto nowWindow = std::make_unique<Window>(width, height, workState, isSubject, resizable, transparent, onTop,
                                                  borderless);
        const i32 windowId = nowWindow->getId();
        m_windowMap[tag] = windowId;
        m_windows[windowId] = std::move(nowWindow);
        m_windows[windowId]->setContext(&m_context);
        return *m_windows[windowId];
    }
} // namespace dao
