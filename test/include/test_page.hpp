//
// Created by donghao on 25-12-8.
//
#ifndef TEST_PAGE_HPP
#define TEST_PAGE_HPP
#include <core/frame/vertex_batch_builder.hpp>
#include <string>
#include <texture_id.hpp>
#include "component/button/simple_button.hpp"
#include "component/rectangle.hpp"
#include "component/button/simple_text_button_style.hpp"
#include "interface/page.hpp"

class TestPage final : public dao::Page {
    dao::AtlasTexture appleImg{texture::food_apple, 0, 0, 100, 100};
    dao::AtlasTexture bananaImg{texture::food_banana, 100, 0, 200, 100};
    dao::Rectangle rectangle{200, 0, 100, 100, dao::hexToRGBA("#25B1F3")};
    dao::Rectangle rectangle2{0, 100, 200, 100, {0, 255, 0, 1.0f}};
    dao::Rectangle rectangle3{0, 200, 200, 100, {0, 0, 255, 1.0f}};
    dao::SimpleButton button{
        200, 200, 100, 100,
        [this]() {
            m_windowEvent.addSwitchPage("testPage1");
        }
    };
    dao::SimpleTextButtonStyle buttonStyle{
        200, 200, 100, 100, U"按钮", 100, dao::hexToRGBA("#0C81CF"), &button
    };

public:
    void init() override {
    }
    void close() override {}
    TestPage() = default;

    explicit TestPage(std::string title): m_title(std::move(title)) {
    }

    [[nodiscard]] std::vector<dao::uint32> getRegisterTexture() const override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;

    dao::PageCmdQueue &getEvent() override { return m_windowEvent; };

    [[nodiscard]] const std::vector<dao::AtlasDrawBatch> &getDrawBatches() const override;

    dao::GlyphAtlas &getGlyphAtlas() override {
        return m_vertexBatch.getGlyphAtlas();
    }

    [[nodiscard]] const std::string &getTitle() const override { return m_title; }

private:
    std::string m_title{"testPage"};
    dao::PageCmdQueue m_windowEvent;
    dao::VertexBatchBuilder m_vertexBatch{"zh-cn.ttf", 64, 1024};
};


#endif //TEST_PAGE_HPP
