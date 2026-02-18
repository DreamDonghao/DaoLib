//
// Created by donghao on 25-11-26.
//
#ifndef APP_HPP
#define APP_HPP
#include <core/frame/window.hpp>
#include <core/frame/tray.hpp>

#include "context.hpp"
#include "frameLimiter.hpp"

namespace dao {
    /// @brief 应用
    /// @details 用来管理整个应用程序
    class App final {
    public:
        /// @param fps 帧率
        /// @param clickThrough 失焦点击生效
        explicit App(uint32 fps = 60,bool clickThrough = false);

        App(const App &) = delete;            // 禁止拷贝
        App &operator=(const App &) = delete; // 禁止拷贝
        App &operator=(App &&) = default;     // 支持移动
        ~App();

        /// @brief 创建窗口
        /// @param width 窗口默认宽度
        /// @param height 窗口默认高度
        /// @param tag 窗口标识
        /// @param hidden 隐藏
        /// @param isSubject 为应用主体窗口
        /// @param resizable 可重新设置大小
        /// @param transparent 支持透明
        /// @param onTop 置顶
        /// @param borderless 无边框
        /// @returns 窗口对象的引用
        Window &createWindow(uint32 width, uint32 height, std::string_view tag,
                             bool hidden = false, bool isSubject = false,
                             bool resizable = false, bool transparent = false, bool onTop = false,
                             bool borderless = false);

        /// @brief 创建托盘
        template<typename Type>
        Tray &createTray(const std::string_view iconPath, const std::string_view tooltip) {
            m_tray = std::make_unique<Type>(iconPath, tooltip);
            m_tray->setContext(m_context);
            return *m_tray;
        }

        /// @brief 获取窗口
        /// @param windowId 窗口 id
        /// @returns 窗口对象的引用
        Window &getWindow(const uint32 windowId) { return *m_windows[windowId]; }

        /// @brief 启动应用
        void run();

        /// @brief 关闭程序
        void close();

        Context &getContext() { return m_context; }

        void showWindow(const std::string_view windowTag) {
            m_windows[m_windowMap[windowTag]]->show();
        }

        void hideWindow(const std::string_view windowTag) {
            m_windows[m_windowMap[windowTag]]->hide();
        }

    private:
        bool m_running = false;                               ///< 是否运行
        FrameLimiter m_frameLimiter;                          ///< 帧率控制器
        hash_map<std::string_view, uint32> m_windowMap;       ///< 窗口标识 ID 映射表
        hash_map<uint32, std::unique_ptr<Window> > m_windows; ///< 窗口映射表
        std::unique_ptr<Tray> m_tray = nullptr;               ///< 托盘
        Context m_context;                                    ///< 全局环境数据
    };
}
#endif //APP_HPP
