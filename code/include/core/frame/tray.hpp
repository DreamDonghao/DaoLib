//
// Created by donghao on 2026/2/14.
//
#pragma once
#include <SDL3_image/SDL_image.h>

namespace dao {
    class Tray {
    public:
        Tray(const std::string_view iconPath, const std::string_view tooltip) {
            m_icon = IMG_Load(iconPath.data());
            if (!m_icon) {
                DAO_ERROR_LOG(std::string("托盘图片失败:") + iconPath.data());
            }
            m_tray = SDL_CreateTray(m_icon, tooltip.data());
            if (!m_tray) {
                DAO_ERROR_LOG("托盘创建失败");
            }
        }

        [[nodiscard]] SDL_Tray *getSDLTray() const {
            return m_tray;
        }

        ~Tray() {
            SDL_DestroySurface(m_icon);
            SDL_DestroyTray(m_tray);
        }

    private:
        SDL_Surface *m_icon = nullptr;
        SDL_Tray *m_tray = nullptr;
    };
}
