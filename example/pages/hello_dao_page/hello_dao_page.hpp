#pragma once
#include <daoui.hpp>
#include <texture_id.hpp>
#include <component/image.hpp>

namespace example {
    class HelloDaoPage : public dao::GeneralPage {
        dao::Image image{10, 50, 150, 150, texture::image};

    public:
        HelloDaoPage() : GeneralPage("helloDaoPage") {
        };

        ~HelloDaoPage() override = default;

        [[nodiscard]] std::vector<dao::u32> getRegisterTexture() const override;

        void init() override;

        void close() override;

        void update() override;

        void handleInputEvent(const SDL_Event &event) override;
    };
}
