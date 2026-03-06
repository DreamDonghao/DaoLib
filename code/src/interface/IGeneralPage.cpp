#include <interface/IGeneralPage.hpp>
#include "core/frame/Context.hpp"

dao::ifc::IGeneralPage::IGeneralPage(std::string title) : m_title(std::move(title)) {
}

dao::WindowController &dao::ifc::IGeneralPage::getWindowController() {
    return m_windowController;
}

const std::string &dao::ifc::IGeneralPage::getTitle() const {
    return m_title;
}

void dao::ifc::IGeneralPage::clearBatch() const {
    m_vertexBatch->clearDrawBatches();
}

void dao::ifc::IGeneralPage::setContext(Context *context) {
    m_context = context;
}

dao::Context &dao::ifc::IGeneralPage::getContext() const {
    if (!m_context) {
        DAO_ERROR_LOG("m_context指针为空");
        return *new Context;
    }
    return *m_context;
}
