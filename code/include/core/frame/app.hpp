//
// Created by donghao on 25-11-26.
//
#ifndef APP_HPP
#define APP_HPP
#include <core/frame/window.hpp>
#include <core/frame/tray.hpp>

#include "context.hpp"

namespace dao {
    /// @brief 应用
    /// @details 用来管理整个应用程序
    class App final {
    public:
        /// @param clickThrough 失焦点击生效
        explicit App(bool clickThrough = false);

        App(const App &) = delete;            // 禁止拷贝
        App &operator=(const App &) = delete; // 禁止拷贝
        App &operator=(App &&) = default;     // 支持移动
        ~App();

        /// @brief 创建窗口
        /// @param width 窗口默认宽度
        /// @param height 窗口默认高度
        /// @param display 默认显示
        /// @param isSubject 为应用主体窗口
        /// @param resizable 可重新设置大小
        /// @param transparent 支持透明
        /// @param onTop 置顶
        /// @param borderless 无边框
        /// @returns 窗口对象的引用
        Window &createWindow(uint32 width, uint32 height, bool display = false, bool isSubject = false,
                             bool resizable = false, bool transparent = false, bool onTop = false,
                             bool borderless = false);

        /// @brief 创建托盘
        Tray &createTray(std::string_view iconPath, std::string_view tooltip);

        /// @brief 获取窗口
        /// @param windowId 窗口 id
        /// @returns 窗口对象的引用
        Window &getWindow(const uint32 windowId) { return *m_windows[windowId]; }

        /// @brief 启动应用
        void run();

        /// @brief 关闭程序
        void close();

        Context &getContext() { return m_context; }
    private:
        bool m_running = false;
        hash_map<uint32, std::unique_ptr<Window> > m_windows; ///< 窗口映射表
        std::unique_ptr<Tray> m_tray = nullptr;               ///<托盘

        Context m_context;
    };
}
#endif //APP_HPP
