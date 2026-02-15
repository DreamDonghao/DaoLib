#pragma once
#include <any>
#include <vector>

namespace dao {
    class App;
    enum class AppCommandType {
        close,
    };

    struct AppCommand {
        AppCommandType type;
        std::any data;
    };

    class AppController {
    public:
        AppController() = default;

        AppController(const AppController &) = delete;

        void executeCommand(App &app);

        void close();

    private:
        std::vector<AppCommand> m_appCommands;
    };
}
