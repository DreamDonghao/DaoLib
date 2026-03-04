#include <interface/general_page.hpp>
#include "core/frame/context.hpp"

dao::GeneralPage::GeneralPage(std::string title): m_title(std::move(title)) {
}

dao::WindowController & dao::GeneralPage::getWindowController() {
    return m_windowController;
}

const std::string & dao::GeneralPage::getTitle() const {
    return m_title;
}

void dao::GeneralPage::setContext(Context *context) {
    m_context = context;
}

dao::Context &dao::GeneralPage::getContext() const {
    if (!m_context) {
        DAO_ERROR_LOG("m_context指针为空");
        return *new Context;
    }
    return *m_context;
}