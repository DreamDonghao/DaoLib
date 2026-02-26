#include <core/frame/window_controller.hpp>
#include <core/frame/window.hpp>

void dao::WindowController::executeCommand(Window &window) {
    for (const auto &[type, data]: m_windowCommands) {
        i32 x, y;
        switch (type) {
            case WindowCommandType::switchPage:
                window.switchPage(std::any_cast<std::string>(data));
                break;
            case WindowCommandType::setClickThrough:
                window.setClickThrough(std::any_cast<bool>(data));
                break;
            case WindowCommandType::setPosition:
                x = std::any_cast<std::pair<i32, i32> >(data).first;
                y = std::any_cast<std::pair<i32, i32> >(data).second;
                window.setPosition(x, y);
                break;
            case WindowCommandType::movePosition:
                x = std::any_cast<std::pair<i32, i32> >(data).first;
                y = std::any_cast<std::pair<i32, i32> >(data).second;
                window.movePosition(x, y);
                break;
            case WindowCommandType::setSize:
                x = std::any_cast<std::pair<i32, i32> >(data).first;
                y = std::any_cast<std::pair<i32, i32> >(data).second;
                window.setSize(x, y);
                break;
            default:
                break;
        }
    }
    m_windowCommands.clear();
}

void dao::WindowController::switchPage(const std::string &title) {
    m_windowCommands.push_back(WindowCommand{WindowCommandType::switchPage, std::any{title}});
}

void dao::WindowController::setClickThrough(const bool &clickThrough) {
    m_windowCommands.push_back(WindowCommand{WindowCommandType::setClickThrough, std::any{clickThrough}});
}

void dao::WindowController::movePosition(const i32 x, const i32 y) {
    m_windowCommands.push_back(WindowCommand{
        WindowCommandType::movePosition, std::any{std::make_pair(x, y)}
    });
}

void dao::WindowController::setPosition(i32 x, i32 y) {
    m_windowCommands.push_back(WindowCommand{
        WindowCommandType::setPosition, std::any{std::make_pair(x, y)}
    });
}

void dao::WindowController::setSize(i32 x, i32 y) {
    m_windowCommands.push_back(WindowCommand{
        WindowCommandType::setSize, std::any{std::make_pair(x, y)}
    });
}
