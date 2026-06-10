#pragma once
#include <interface/IGeneralPage.hpp>

namespace dao {
    class VoidPage : public dao::ifc::IGeneralPage {
    public:
        VoidPage() : IGeneralPage("") {
        };

        ~VoidPage() override = default;

        [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override { return {}; }

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