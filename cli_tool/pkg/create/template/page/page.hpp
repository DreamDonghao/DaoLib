#pragma once
#include <interface/IGeneralPage.hpp>

namespace {{.Namespace}} {
    class {{.PageName}} : public dao::ifc::IGeneralPage {
    public:
        {{.PageName}}() : GeneralPage("{{.PageName}}") {
        };

        ~{{.PageName}}() override = default;

        [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override;

        void open() override;

        void close() override;

        void update() override;

        void handleInputEvent(const SDL_Event &event) override;
    };
}