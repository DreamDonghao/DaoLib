#pragma once
#include <interface/IGeneralPage.hpp>
#include <core/tool/json.hpp>
#include <components/Image.hpp>
#include <components/TextBox.hpp>
#include <components/controls/InputBox.hpp>
#include <components/graphs/Rectangle.hpp>
#include <web/http/HttpsClient.hpp>
#include <texture_id.hpp>
#include <components/AnimatedSprite.hpp>

namespace test {
    class SettingsPage : public dao::ifc::IGeneralPage {
    public:
        SettingsPage() : IGeneralPage("SettingsPage") {
        }

        [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override {
            return {texture::image};
        }

        void open() override {
        }

        void close() override {
        }

        void update() override {
        }

        void handleInputEvent(const SDL_Event &event) override {
        }
    };
}
