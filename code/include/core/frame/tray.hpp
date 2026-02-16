#pragma once
#include <core/tool/better_stl.hpp>
#include <functional>
#include <SDL3_image/SDL_image.h>
#include <vector>
namespace dao {
    class Context;
    using TrayFunc = std::move_only_function<void(SDL_TrayEntry *entry)>;

    inline void callback(void *userdata, SDL_TrayEntry *entry) {
        (*static_cast<TrayFunc *>(userdata))(entry);
    }

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
            m_menu = SDL_CreateTrayMenu(m_tray);
            if (!m_menu) {
                DAO_ERROR_LOG("托盘菜单创建失败");
            }
        }

        void button(const std::string_view label, TrayFunc &trayFunc) const {
            SDL_TrayEntry *entry = SDL_InsertTrayEntryAt(m_menu, -1, label.data(), SDL_TRAYENTRY_BUTTON);
            SDL_SetTrayEntryCallback(entry, callback, &trayFunc);
        }

        void button(const std::string_view label, TrayFunc &&trayFunc) {
            m_callbacks.push_back(std::move(trayFunc));
            SDL_TrayEntry *entry = SDL_InsertTrayEntryAt(m_menu, -1, label.data(), SDL_TRAYENTRY_BUTTON);
            SDL_SetTrayEntryCallback(entry, callback, &m_callbacks.back());
        }

        virtual void createTrayMenu() = 0;

        [[nodiscard]] SDL_Tray *getSDLTray() const { return m_tray; }
        [[nodiscard]] SDL_TrayMenu *getSDLTrayMenu() const { return m_menu; }
        [[nodiscard]] Context &getContext() const { return *m_context; }

        void setContext(Context &context) { m_context = &context; }

        virtual ~Tray() {
            SDL_DestroySurface(m_icon);
            SDL_DestroyTray(m_tray);
        }

    private:
        SDL_Surface *m_icon = nullptr;
        SDL_Tray *m_tray = nullptr;
        SDL_TrayMenu *m_menu;
        Context *m_context = nullptr;
        std::vector<TrayFunc> m_callbacks;
    };
}
