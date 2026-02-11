//
// Created by donghao on 25-11-26.
//
#ifndef APP_HPP
#define APP_HPP
#include <core/frame/window.hpp>

namespace dao {
    /// @brief 应用
    /// @details 用来管理整个应用程序
    class App final {
    public:
        App(const App &) = delete;            // 禁止拷贝
        App &operator=(const App &) = delete; // 禁止拷贝
        App &operator=(App &&) = default;     // 支持移动
        ~App() {
            TTF_Quit();
            SDL_Quit();
        }

        /// @brief 创建应用
        /// @en
        /// @returns 一个应用实例
        [[nodiscard]] static App &getApp() {
            static App app{};
            return app;
        }

        /// @brief 创建窗口
        /// @param width 窗口默认宽度
        /// @param height 窗口默认高度
        /// @param resizable 可重新设置大小
        /// @param transparent 支持透明
        /// @param onTop 置顶
        /// @param borderless 无边框
        /// @returns 窗口对象的引用
        Window &createWindow(uint32 width, uint32 height,
                             bool resizable = false, bool transparent = false, bool onTop = false,
                             bool borderless = false);

        /// @brief 获取窗口
        /// @param windowId 窗口 id
        /// @returns 窗口对象的引用
        Window &getWindow(const uint32 windowId) { return *m_windows[windowId]; }

        /// @brief 启动应用
        void run();

    private:
        uint32 m_runWindowNum = 1;                            ///< 当前正在运行的窗口数量
        hash_map<uint32, std::unique_ptr<Window> > m_windows; ///< 窗口映射表

        /// 设置默认构造函数为私有成员，使 App 成为单例模式
        App();

        /// 渲染所有窗口内容
        void render();
    };
}
#endif //APP_HPP
