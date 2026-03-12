#pragma once
#include <daolib.hpp>

namespace test {
    class PageA : public dao::ifc::IGeneralPage {
    public:
        PageA() : IGeneralPage("graph_test") {
        };

        ~PageA() override = default;

        [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override;

        void open() override;

        void close() override;

        void update() override;

        void handleInputEvent(const SDL_Event &event) override;

    private:
        float m_time = 0.0f;
    };
}