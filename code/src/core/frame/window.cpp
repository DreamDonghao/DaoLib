//
// Created by donghao on 25-12-6.
//
#include <core/frame/window.hpp>
#include <core/frame/window_event.hpp>
#include <core/basic_drawing_elements/atlas_region.hpp>
#include <SDL3_image/SDL_image.h>
#include <print>
#include <utility>
#include <chrono>

namespace dao {
    Window::Window(const uint32 width, const uint32 height,
        const bool resizable, const bool transparent, const bool onTop, const bool borderless) {
        SDL_WindowFlags flags = 0;
        if (resizable)flags |= SDL_WINDOW_RESIZABLE;
        if (transparent)flags |= SDL_WINDOW_TRANSPARENT;
        if (onTop)flags |= SDL_WINDOW_ALWAYS_ON_TOP;
        if (borderless)flags |= SDL_WINDOW_BORDERLESS;
        m_window = SDL_CreateWindow("", static_cast<int>(width), static_cast<int>(height), flags);

        m_renderer = SDL_CreateRenderer(m_window, "direct3d11");
        SDL_SetRenderVSync(m_renderer, 0);
        m_id = SDL_GetWindowID(m_window);
        /// 构建纯白纹理
        SDL_Texture *tex = SDL_CreateTexture(
            m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 1, 1
        );
        constexpr Uint32 whitePixel = 0xFFFFFFFF;
        SDL_UpdateTexture(tex, nullptr, &whitePixel, 4);
        m_atlasTextures[0] = tex;
    }

    Window::~Window() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    Window &Window::addPage(std::unique_ptr<Page> &&page) {
        const std::string title = page->getTitle();
        detectionError(!m_pages.contains(title), std::string("重复页面:") + title);
        for (auto textureId: page->getRegisterTexture()) {
            registerTexture(textureId);
        }
        if (m_pages.empty()) {
            m_nowPageTitle = title;
        }
        m_pages[title] = std::move(page);
        m_pages[title]->init();
        m_atlasTextures[1] = SDL_CreateTextureFromSurface(
            m_renderer, &m_pages[title]->getGlyphAtlas().getAtlasSurface());
        return *this;
    }

    void Window::registerTexture(const uint32 &textureId) {
        const AtlasRegion atlasRegion = getAtlasRegion(textureId);
        // 加载新纹理图集
        if (const uint32 atlasId = atlasRegion.atlasId; !m_atlasTextures.contains(atlasId)) {
            const char *texturePath = atlasRegion.atlasPath;
            m_atlasTextures[atlasId] = IMG_LoadTexture(m_renderer, texturePath);
            SDL_SetTextureScaleMode(m_atlasTextures[atlasId], SDL_SCALEMODE_NEAREST);
            detectionError(m_atlasTextures[atlasId],
                           std::string("纹理图集加载失败") + SDL_GetError() + ":" + texturePath);
        }
    }

    void Window::update() {
        m_pages[m_nowPageTitle]->update();
        if (m_pages[m_nowPageTitle]->getGlyphAtlas().isUpdated()) {
            SDL_UpdateTexture(
                m_atlasTextures[1], nullptr,
                m_pages[m_nowPageTitle]->getGlyphAtlas().getAtlasSurface().pixels,
                m_pages[m_nowPageTitle]->getGlyphAtlas().getAtlasSurface().pitch
            );
            SDL_SetTextureScaleMode(m_atlasTextures[1], SDL_SCALEMODE_NEAREST);
            m_pages[m_nowPageTitle]->getGlyphAtlas().clearUpdateFlag();
        }

        if (m_pages[m_nowPageTitle]->getEvent().isPresenceMessage()) {
            switch (const auto event = m_pages[m_nowPageTitle]->getEvent().popEvent(); event.type) {
                case PageCmdType::switchPage:
                    switchPage(std::any_cast<std::string>(event.data));
                    break;
                default:
                    break;
            }
        }
    }

    void Window::handleInputEvent(const SDL_Event &event) {
        m_pages[m_nowPageTitle]->handleInputEvent(event);
    }

    // void Window::handleInputState(const bool *keyboardState) {
    //     m_pages[m_nowPageTitle]->handleInputState(keyboardState);
    // }

    void Window::requestClose() {
        m_running = false;
    }

    void Window::render() {
        SDL_RenderClear(m_renderer);
        for (const auto &[atlasId, vertices, indices,indicesCount]:
             m_pages[m_nowPageTitle]->getDrawBatches()) {
            SDL_RenderGeometry(
                m_renderer, m_atlasTextures[atlasId],
                vertices.data(), static_cast<int>(vertices.size()),
                indices->data(), indicesCount
            );
        }

        SDL_RenderPresent(m_renderer);
    }

    void Window::switchPage(std::string title) {
        detectionError(m_pages.contains(title),
                       std::string("不存在的页面") + m_nowPageTitle + "->" + title);
        m_pages[m_nowPageTitle]->close();
        m_nowPageTitle = std::move(title);
        m_pages[m_nowPageTitle]->init();
        m_atlasTextures[1] = SDL_CreateTextureFromSurface(
            m_renderer, &m_pages[m_nowPageTitle]->getGlyphAtlas().getAtlasSurface()
        );
    }
}
