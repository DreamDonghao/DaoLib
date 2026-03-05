#include <core/frame/AppController.hpp>
#include "core/frame/App.hpp"

void dao::AppController::executeCommand(App &app) {
    for (const auto &[type, data]: m_appCommands) {
        switch (type) {
            case AppCommandType::close:
                app.close();
            default:
                break;
        }
    }
    m_appCommands.clear();
}

void dao::AppController::close() {
    m_appCommands.push_back(AppCommand{AppCommandType::close, std::any{}});
}
