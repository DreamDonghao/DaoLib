#pragma once
#include <daoui.hpp>

namespace test {
    class PageA : public dao::ifc::IGeneralPage {
    public:
        PageA() : IGeneralPage("setPage") {
        };

        ~PageA() override = default;

        [[nodiscard]] std::vector<dao::TextureID> getRegisterTexture() const override;

        void open() override;

        void close() override;

        void update() override;

        void handleInputEvent(const SDL_Event &event) override;
    };
}