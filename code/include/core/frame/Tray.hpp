#pragma once
#include <core/tool/better_stl.hpp>
#include <functional>
#include <SDL3_image/SDL_image.h>
#include <vector>

namespace dao {
    class Context;

    using TrayFunc = std::move_only_function<void(SDL_TrayEntry *entry)>;

    /// @brief 托盘回调函数适配器
    /// @details 将 SDL 的 C 风格回调转换为 C++ 函数对象调用
    /// @param userdata 用户数据（TrayFunc 指针）
    /// @param entry 被点击的托盘菜单项
    inline void callback(void *userdata, SDL_TrayEntry *entry) {
        (*static_cast<TrayFunc *>(userdata))(entry);
    }

    /// @brief 托盘
    /// @details 用于管理应用程序的系统托盘功能，包括托盘图标、托盘菜单和菜单项回调
    class Tray {
    public:
        /// @param iconPath 托盘图标文件路径
        /// @param tooltip 托盘悬停提示文本
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

        /// @brief 添加托盘菜单按钮（引用版本）
        /// @param label 菜单项显示文本
        /// @param trayFunc 回调函数引用
        /// @warning 使用此版本时需确保 trayFunc 生命周期长于托盘对象
        void button(const std::string_view label, TrayFunc &trayFunc) const {
            SDL_TrayEntry *entry = SDL_InsertTrayEntryAt(m_menu, -1, label.data(), SDL_TRAYENTRY_BUTTON);
            SDL_SetTrayEntryCallback(entry, callback, &trayFunc);
        }

        /// @brief 添加托盘菜单按钮（右值引用版本）
        /// @param label 菜单项显示文本
        /// @param trayFunc 回调函数（将被移动存储）
        void button(const std::string_view label, TrayFunc &&trayFunc) {
            m_callbacks.push_back(std::move(trayFunc));
            SDL_TrayEntry *entry = SDL_InsertTrayEntryAt(m_menu, -1, label.data(), SDL_TRAYENTRY_BUTTON);
            SDL_SetTrayEntryCallback(entry, callback, &m_callbacks.back());
        }

        virtual void createTrayMenu() = 0;

        /// @brief 获取 SDL 托盘对象
        [[nodiscard]] SDL_Tray *getSDLTray() const { return m_tray; }

        /// @brief 获取 SDL 托盘菜单对象
        [[nodiscard]] SDL_TrayMenu *getSDLTrayMenu() const { return m_menu; }

        /// @brief 获取上下文对象
        [[nodiscard]] Context &getContext() const { return *m_context; }

        /// @brief 设置上下文对象
        /// @param context 上下文对象引用
        void setContext(Context &context) { m_context = &context; }

        virtual ~Tray() {
            SDL_DestroySurface(m_icon);
            SDL_DestroyTray(m_tray);
        }

    private:
        SDL_Surface *m_icon{nullptr};      ///< 托盘图标
        SDL_Tray *m_tray{nullptr};         ///< SDL 托盘对象
        SDL_TrayMenu *m_menu{nullptr};     ///< SDL 托盘菜单对象
        Context *m_context{nullptr};       ///< 上下文对象指针
        std::vector<TrayFunc> m_callbacks; ///< 存储回调函数
    };
}