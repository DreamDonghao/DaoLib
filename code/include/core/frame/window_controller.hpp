//
// Created by donghao on 25-12-19.
//
#pragma once
#include <any>
#include <string>

#include "core/tool/better_stl.hpp"

namespace dao {
    class Window;

    /// @brief 窗口命令类型
    enum class WindowCommandType {
        switchPage,      ///< 切换页面
        setClickThrough, ///< 设置点击是否穿透
        setPosition,     ///< 设置窗口位置
        movePosition,    ///< 移动窗口位置
        setSize,         ////<设置窗口大小
    };

    /// @brief 窗口命令
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

        void switchPage(const std::string &title);

        void setClickThrough(const bool &clickThrough);

        void movePosition(const uint32 x, const uint32 y);

        void setPosition(uint32 x, uint32 y);

        void setSize(uint32 x, uint32 y);

    private:
        std::vector<WindowCommand> m_windowCommands;
    };
}
