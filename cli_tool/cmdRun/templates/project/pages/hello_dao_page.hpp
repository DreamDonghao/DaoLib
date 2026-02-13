#pragma once
#include <daoui.hpp>
#include <texture_id.hpp>
using namespace dao;

class HelloDaoPage :public GeneralPage {
    Text text{0, 0, U"你好 Dao", 50, hexToRGBA("#D249FC")};
    AtlasTexture img{texture::image,10,50,160,200};
public:
    HelloDaoPage() : GeneralPage("helloDaoPage") {
    };

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<uint32> getRegisterTexture() const override;

    void init() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
