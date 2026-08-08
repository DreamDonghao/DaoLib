#include <core/frame/Window.hpp>
#include <core/frame/WindowController.hpp>
#include <ranges>
#include <utility>
#include <core/frame/IPage.hpp>
#include "core/frame/App.hpp"

namespace dao {
    static SDL_WindowFlags createWindowFlags(
        const Window::WorkState workState, const bool resizable, const bool transparent, const bool onTop,
        const bool borderless) {
        SDL_WindowFlags windowFlags = 0;
        if (workState == Window::WorkState::Background || workState == Window::WorkState::Closed) {
            windowFlags |= SDL_WINDOW_HIDDEN;
        }
        if (resizable)
            windowFlags |= SDL_WINDOW_RESIZABLE;
        if (transparent)
            windowFlags |= SDL_WINDOW_TRANSPARENT;
        if (onTop)
            windowFlags |= SDL_WINDOW_ALWAYS_ON_TOP;
        if (borderless)
            windowFlags |= SDL_WINDOW_BORDERLESS;
        return windowFlags;
    }

    Window::Window(const i32 width, const i32 height, const WorkState workState, const bool isSubject,
                   const bool resizable, const bool transparent, const bool onTop,
                   const bool borderless)
        : m_window(SDL_CreateWindow("", width, height,
                                    createWindowFlags(workState, resizable, transparent, onTop, borderless))),
          m_id(static_cast<i32>(SDL_GetWindowID(m_window))),
          m_workState(workState), m_width(width), m_height(height),
          m_batchRenderer(m_id, SDL_CreateRenderer(m_window, nullptr)),
          m_isSubject(isSubject) {
    }

    Window::~Window() {
        if (m_window) {
            SDL_DestroyWindow(m_window);
        }
    }

    Window &Window::addPage(std::unique_ptr<ifc::IPage> page) {
        const std::string_view title = page->getTitle();
        if (m_pages.contains(title)) {
            ErrorLog(std::string("重复页面:") + title);
        }

        if (m_pages.empty()) {
            m_nowPageTitle = title;
            setTitle(m_nowPageTitle);
        }
        // 加载页面使用的纹理
        for (const auto textureId: page->getRegisterTextures()) {
            m_batchRenderer.loadAtlas(textureId);
        }
        m_pages[title] = std::move(page);
        return *this;
    }

    void Window::update() {
        m_pages[m_nowPageTitle]->update();
        executeCommand();
    }

    void Window::handleInputEvent(const SDL_Event &event) {
        m_pages[m_nowPageTitle]->handleInputEvent(event);
    }

    void Window::render() {
        if (m_workState == WorkState::Foreground) {
            m_batchRenderer.render();
        }
    }

    void Window::hide() const { SDL_HideWindow(m_window); }

    void Window::show() const { SDL_ShowWindow(m_window); }

    void Window::convertWorkState(const WorkState workState) {
        m_workState = workState;
        switch (m_workState) {
            case WorkState::Foreground: show();
                break;
            case WorkState::Background: hide();
                break;
            case WorkState::Closed:
                hide();
                if (m_isSubject) {
                    if (m_context->has<App>()) {
                        m_context->get<App>()->exit();
                    }
                }
                break;
            default:
                break;
        }
    }


    void Window::executeCommand() {
        m_pages[m_nowPageTitle]->getWindowController().executeCommand(*this);
    }

    void Window::switchPage(const std::string_view title) {
        if (!m_pages.contains(title)) {
            ErrorLog(std::string("不存在的页面") + m_nowPageTitle + "->" + title);
        }
        m_pages[m_nowPageTitle]->close();
        m_pages[title]->open();
        m_nowPageTitle = title;
        setTitle(m_nowPageTitle);
    }

    std::string_view Window::getNowPageTitle() const {
        return m_nowPageTitle;
    }

    void Window::setPosition(const i32 x, const i32 y) const { SDL_SetWindowPosition(m_window, x, y); }

    void Window::movePosition(const i32 x, const i32 y) const {
        int wx, wy;
        SDL_GetWindowPosition(m_window, &wx, &wy);
        SDL_SetWindowPosition(m_window, wx + x, wy + y);
    }

    void Window::setSize(const i32 width, const i32 height) const {
        SDL_SetWindowSize(m_window, width, height);
    }

    void Window::setTitle(const std::string_view title) const { SDL_SetWindowTitle(m_window, title.data()); }
}

void dao::Window::setContext(Context *context) { m_context = context; }

#ifdef _WIN32
#include <Windows.h>

void dao::Window::setClickThrough(const bool enable) const {
    if (!m_window)
        return;
    const SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
    const auto hwnd = static_cast<HWND>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
    if (!hwnd) {
        ErrorLog("HWND 获取失败");
        return;
    }

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    if (enable) {
        exStyle |= WS_EX_TRANSPARENT;
        exStyle |= WS_EX_LAYERED;
    } else {
        exStyle &= ~WS_EX_TRANSPARENT;
        exStyle &= ~WS_EX_LAYERED;
    }
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

#elifdef __linux__
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

void dao::Window::setClickThrough(bool enable) const {
    SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
    Display *display = (Display *) SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
    Window xwindow = (Window) SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    if (!display || !xwindow)
        return;
    if (enable) {
        XShapeCombineMask(display, xwindow, ShapeInput, 0, 0, None, ShapeSet);
    } else {
        XShapeCombineMask(display, xwindow, ShapeInput, 0, 0, None, ShapeInvert);
    }
    XFlush(display);
}
#elifdef __APPLE__
extern "C" void dao_window_set_click_through(void *nsWindowPtr, bool enable);

void dao::Window::setClickThrough(const bool enable) const {
    const SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
    void *nsWindow = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (!nsWindow)
        return;
    dao_window_set_click_through(nsWindow, enable);
}
#endif
