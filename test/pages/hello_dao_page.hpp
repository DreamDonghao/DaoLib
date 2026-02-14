#pragma once
#include <daoui.hpp>
#include <texture_id.hpp>
#include <component/image.hpp>

class HelloDaoPage :public dao::GeneralPage {
    //Text text{0, 0, U"你好 Dao", 50, hexToRGBA("#D249FC")};
    dao::Image image{10,50,150,150,texture::image};
    dao::AtlasTexture img{texture::image,10,50,160,200};
public:
    HelloDaoPage() : GeneralPage("helloDaoPage") {
    };

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<dao::uint32> getRegisterTexture() const override;

    void init() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
