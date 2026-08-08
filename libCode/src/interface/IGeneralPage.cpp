#include <interface/IGeneralPage.hpp>
#include "core/frame/Context.hpp"

dao::ifc::IGeneralPage::IGeneralPage(BatchRenderer *vertexBatch, Context *context, const std::string_view title)
: m_title(title), m_BatchRenderer(*vertexBatch), m_context(*context) {
}

dao::WindowController &dao::ifc::IGeneralPage::getWindowController() {
    return m_windowController;
}

const std::string_view &dao::ifc::IGeneralPage::getTitle() const {
    return m_title;
}

dao::Context &dao::ifc::IGeneralPage::getContext(){
    return m_context;
}

dao::BatchRenderer & dao::ifc::IGeneralPage::getBatchRenderer() {
    return m_BatchRenderer;
}
