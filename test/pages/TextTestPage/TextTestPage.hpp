#pragma once
#include <interface/IGeneralPage.hpp>
#include <components/Text.hpp>

#include "components/RectTextBox.hpp"

namespace test {
    class TextTestPage : public dao::ifc::IGeneralPage {
        dao::RectTextBox pageName {0, 0, 200, 24,24,dao::Red, dao::White, U"文本测试页面"};
    public:
        TextTestPage() : IGeneralPage("TextTestPage") {
        }

        void open() override {
        }

        void close() override {}

        [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override {
            return {};
        }

        void update() override {
            clearBatch();
            addToBatch(pageName);
        }

        void handleInputEvent(const SDL_Event &event) override {
        }
    };
}
