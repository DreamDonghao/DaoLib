#pragma once
#include <any>
#include <vector>

namespace dao {
    class App;

    /// @brief 应用级命令类型
    enum class AppCommandType {
        close, ///< 关闭应用
    };

    /// @brief 应用级命令
    /// @details 包含命令类型和附加数据的结构体
    struct AppCommand {
        AppCommandType type;  ///< 命令类型
        std::any data;        ///< 命令附带的数据
    };

    /// @brief 应用控制器
    /// @details 负责处理应用级别的命令，例如关闭应用等
    class AppController {
    public:
        /// @brief 默认构造函数
        AppController() = default;

        /// @brief 禁止拷贝构造
        AppController(const AppController &) = delete;

        /// @brief 执行命令
        /// @param app 要执行命令的应用实例
        void executeCommand(App &app);

        /// @brief 发送关闭应用命令
        void close();

    private:
        std::vector<AppCommand> m_appCommands;  ///< 待执行的应用命令队列
    };
}