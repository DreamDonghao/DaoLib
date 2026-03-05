#include <core/frame/Window.hpp>
#include <core/frame/WindowController.hpp>
#include <SDL3_image/SDL_image.h>
#include <utility>
#include <ranges>

namespace dao {
    Window::Window(
        const i32 width, const i32 height, const bool hidden, const bool isSubject,
        const bool resizable, const bool transparent, const bool onTop, const bool borderless)
        : m_width(width), m_height(height) {
        if (hidden) m_windowFlags |= SDL_WINDOW_HIDDEN;
        if (resizable) m_windowFlags |= SDL_WINDOW_RESIZABLE;
        if (transparent) m_windowFlags |= SDL_WINDOW_TRANSPARENT;
        if (onTop) m_windowFlags |= SDL_WINDOW_ALWAYS_ON_TOP;
        if (borderless) m_windowFlags |= SDL_WINDOW_BORDERLESS;
        create();
        if (isSubject) {
            m_closeAction = [this]() {
                m_appController.close();
            };
        }
    }

    Window::~Window() {
        if (m_window) { SDL_DestroyWindow(m_window); }
    }

    Window &Window::addPage(std::unique_ptr<ifc::IPage> &&page) {
        const std::string title = page->getTitle();
        detectionError(!m_pages.contains(title), std::string("重复页面:") + title);

        if (m_pages.empty()) {
            m_nowPageTitle = title;
            setTitle(m_nowPageTitle);
        }
        m_pages[title] = std::move(page);
        m_pages[title]->setContext(m_context);
        m_pages[title]->setVertexBatch(&m_batchRenderer);
        registerPageTexture();
        m_pages[title]->open();

        return *this;
    }

    void Window::registerPageTexture() {
        for (const auto &page: m_pages | std::views::values) {
            for (auto textureId: page->getRegisterTexture()) {
                m_batchRenderer.registerTexture(textureId);
            }
        }
    }

    void Window::update() {
        m_pages[m_nowPageTitle]->update();
        m_batchRenderer.render();
        executeCommand();
    }

    void Window::handleInputEvent(const SDL_Event &event) {
        m_pages[m_nowPageTitle]->handleInputEvent(event);
    }

    void Window::hide() const {
        SDL_HideWindow(m_window);
        m_closeAction();
    }

    void Window::show() const {
        SDL_ShowWindow(m_window);
    }

    void Window::destroy() {
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    void Window::create() {
        if (m_window) {
            DAO_ERROR_LOG("重复创建窗口");
            return;
        }
        m_window = SDL_CreateWindow(
            m_nowPageTitle.data(), m_width, m_height,
            m_windowFlags);
        m_batchRenderer.init(SDL_CreateRenderer(m_window, nullptr));
        m_id = static_cast<i32>(SDL_GetWindowID(m_window));
    }


    void Window::executeCommand() {
        m_pages[m_nowPageTitle]->getWindowController().executeCommand(*this);
    }

    void Window::switchPage(std::string title) {
        detectionError(m_pages.contains(title),
                       std::string("不存在的页面") + m_nowPageTitle + "->" + title);
        m_pages[m_nowPageTitle]->close();
        m_nowPageTitle = std::move(title);
        setTitle(m_nowPageTitle);
        m_pages[m_nowPageTitle]->open();
    }

    void Window::setPosition(const i32 x, const i32 y) const {
        SDL_SetWindowPosition(m_window, x, y);
    }

    void Window::movePosition(const i32 x, const i32 y) const {
        int wx, wy;
        SDL_GetWindowPosition(m_window, &wx, &wy);
        SDL_SetWindowPosition(m_window, wx + x, wy + y);
    }

    void Window::setSize(const i32 width, const i32 height) const {
        SDL_SetWindowSize(m_window, static_cast<int>(width), static_cast<int>(height));
    }

    void Window::setTitle(const std::string &title) const {
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}


#ifdef _WIN32
#include <Windows.h>

void dao::Window::setClickThrough(const bool enable) const {
    if (!m_window) return;
    const SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
    const auto hwnd = static_cast<HWND>(SDL_GetPointerProperty(
        props,SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr
    ));
    if (!hwnd) {
        DAO_ERROR_LOG("HWND 获取失败");
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

    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

#elifdef __linux__
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

void Window::setClickThrough(bool enable) {
    SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
    Display *display = (Display *) SDL_GetPointerProperty(
        props,
        SDL_PROP_WINDOW_X11_DISPLAY_POINTER,
        NULL
    );
    Window xwindow = (Window) SDL_GetNumberProperty(
        props,
        SDL_PROP_WINDOW_X11_WINDOW_NUMBER,
        0
    );
    if (!display || !xwindow) return;
    if (enable) {
        XShapeCombineMask(display, xwindow, ShapeInput, 0, 0, None, ShapeSet);
    } else {
        XShapeCombineMask(display, xwindow, ShapeInput, 0, 0, None, ShapeInvert);
    }
    XFlush(display);
}
#elifdef __APPLE__
#import <Cocoa/Cocoa.h>
void Window::setClickThrough(bool enable) {
    SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
    NSWindow *nsWindow = (__bridge
    NSWindow *
    )
    SDL_GetPointerProperty(
        props,
        SDL_PROP_WINDOW_COCOA_WINDOW_POINTER,
        NULL
    );
    if (!nsWindow) return;
    [nsWindow setIgnoresMouseEvents: enable];
}
#endif
