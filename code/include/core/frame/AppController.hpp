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
    struct AppCommand {
        AppCommandType type;
        std::any data;
    };

    /// @brief 应用控制器
    class AppController {
    public:
        AppController() = default;

        AppController(const AppController &) = delete;

        /// @brief 执行命令
        void executeCommand(App &app);

        /// @brief 关闭应用
        void close();

    private:
        std::vector<AppCommand> m_appCommands;
    };
}
