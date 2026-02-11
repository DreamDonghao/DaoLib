//
// Created by donghao on 25-12-19.
//
#pragma once
#include "component/input/input_box.hpp"
#include "component/input/input_box_style.hpp"
#include "interface/page.hpp"
#include "component/button/simple_texture_button_style.hpp"
#include "component/button/simple_button.hpp"
#include "interface/general_page.hpp"
using namespace dao;

class TestPage1 : public GeneralPage {
    SimpleButton m_button;
    SimpleTextureButtonStyle m_style;
    InputBox inputBox;
    InputBoxStyle inputBoxStyle;

public:
    ~TestPage1() override = default;

    TestPage1()
        : GeneralPage("testPage1"),
          m_button(100, 100, 300, 200, [this]() {
              m_event.addSwitchPage("testPage");
          }),
          m_style(m_button, 2, 3, 3),
          inputBox(500, 500, 300, 100),
          inputBoxStyle(inputBox) {
    }

    void init() override {
    };

    void close() override {
    }

    [[nodiscard]] std::vector<uint32> getRegisterTexture() const override {
        return {};
    }

    void update() override {
        m_vertexBatch.clearDrawBatches();
        m_style.writeToBatch(m_vertexBatch);
        inputBoxStyle.writeToBatch(m_vertexBatch);
    }

    void handleInputEvent(const SDL_Event &event) override {
        m_button.handleEvent(event);
        inputBox.handleEvent(event);
    }
};
