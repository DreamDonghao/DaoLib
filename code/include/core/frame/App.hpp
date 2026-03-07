#pragma once
#include <core/frame/Window.hpp>
#include <core/frame/Tray.hpp>
#include <core/frame/Context.hpp>
#include <core/frame/FrameLimiter.hpp>

namespace dao {
    /// @brief 应用管理类
    /// @details 用来管理整个应用程序，包括窗口、托盘、上下文等组件
    class App final {
    public:
        /// @brief 构造函数
        /// @param fps 应用运行帧率，默认为60
        /// @param clickThrough 失焦点击是否生效，默认为false
        explicit App(i32 fps = 60, bool clickThrough = false);

        /// @brief 禁止拷贝构造
        App(const App &) = delete;

        /// @brief 禁止移动构造
        App(App &&) = delete;

        /// @brief 禁止拷贝赋值
        App &operator=(const App &) = delete;

        /// @brief 禁止移动赋值
        App &operator=(App &&) = delete;

        /// @brief 析构函数
        ~App();

        /// @brief 创建窗口
        /// @param width 窗口默认宽度
        /// @param height 窗口默认高度
        /// @param tag 窗口标识
        /// @param workState 窗口默认工作状态 （前台运行，后台运行，关闭）
        /// @param isSubject 是否为应用主体窗口，默认为false
        /// @param resizable 窗口是否可重新设置大小，默认为false
        /// @param transparent 是否支持透明，默认为false
        /// @param onTop 是否置顶显示，默认为false
        /// @param borderless 是否无边框，默认为false
        /// @returns 窗口对象的引用
        Window &createWindow(i32 width, i32 height, std::string_view tag,
                             Window::WorkState workState, bool isSubject = false,
                             bool resizable = false, bool transparent = false, bool onTop = false,
                             bool borderless = false);

        /// @brief 创建托盘
        /// @tparam Type 托盘的具体类型
        /// @param iconPath 托盘图标路径
        /// @param tooltip 托盘提示文本
        /// @returns 托盘对象的引用
        template<typename Type>
        Tray &createTray(const std::string_view iconPath, const std::string_view tooltip) {
            m_tray = std::make_unique<Type>(iconPath, tooltip);
            m_tray->setContext(m_context);
            return *m_tray;
        }

        /// @brief 根据窗口ID获取窗口对象
        /// @param windowId 窗口ID
        /// @returns 窗口对象的引用
        Window &getWindow(const i32 windowId) { return *m_windows[windowId]; }

        /// @brief 启动应用程序主循环
        void run();

        /// @brief 关闭退出应用程序
        void exit();

        /// @brief 获取全局上下文
        /// @return 全局上下文对象的引用
        Context &getContext() { return m_context; }

        /// @brief 显示指定标签的窗口
        /// @param windowTag 窗口标签
        void showWindow(const std::string_view windowTag) {
            m_windows[m_windowMap[windowTag]]->show();
        }

        /// @brief 隐藏指定标签的窗口
        /// @param windowTag 窗口标签
        void hideWindow(const std::string_view windowTag) {
            m_windows[m_windowMap[windowTag]]->hide();
        }

    private:
        bool m_running = false;                            ///< 应用是否正在运行
        FrameLimiter m_frameLimiter;                       ///< 帧率控制器
        hash_map<std::string_view, i32> m_windowMap;       ///< 窗口标识到ID的映射表
        hash_map<i32, std::unique_ptr<Window> > m_windows; ///< 窗口ID到窗口对象的映射表
        std::unique_ptr<Tray> m_tray = nullptr;            ///< 托盘对象
        Context m_context;                                 ///< 全局环境上下文
    };
}
