#pragma once
#include <any>
#include <string>
#include <vector>

#include <core/tool/better_stl.hpp>

namespace dao {
    class Window;

    /// @brief 窗口级命令类型
    enum class WindowCommandType {
        switchPage,      ///< 切换页面
        setClickThrough, ///< 设置点击是否穿透
        setPosition,     ///< 设置窗口位置
        movePosition,    ///< 移动窗口位置
        setSize,         ///< 设置窗口大小
    };

    /// @brief 窗口级命令
    struct WindowCommand {
        WindowCommandType type; ///< 页面命令类型
        std::any data;          ///< 数据
    };

    /// @brief 窗口控制器
    class WindowController {
    public:
        WindowController() = default;

        WindowController(const WindowController &) = delete;

        /// @brief 执行命令
        void executeCommand(Window &window);

        /// @brief 切换页面
        void switchPage(const std::string &title);

        /// @brief 设置是否点击穿透
        void setClickThrough(const bool &clickThrough);

        /// @brief 移动窗口位置
        void movePosition(i32 x, i32 y);

        /// @brief 设置窗口位置
        void setPosition(i32 x, i32 y);

        /// @brief 设置窗口大小
        void setSize(i32 x, i32 y);

    private:
        std::vector<WindowCommand> m_windowCommands;
    };
}
