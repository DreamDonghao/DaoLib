//
// Created by donghao on 2026/2/11.
//

#pragma once
#include "interface/general_page.hpp"

using namespace dao;

class PatsPage : public GeneralPage {
    AtlasTexture m_pats{texture::food_apple, 0, 0, 100, 100};
    // Text chatBox{0, 100, U"hello", 20, hexToRGBA("#FFD04C")};

public:
    ~PatsPage() override = default;

    PatsPage()
        : GeneralPage("pats") {

    }

    [[nodiscard]] std::vector<uint32> getRegisterTexture() const override {
        return {texture::food_apple};
    }

    void init() override {
    };

    void close() override {
    }


    void update() override {
        //chatBox.setContent(to_u32(currentDateTime()));
        m_vertexBatch.clearDrawBatches();
        m_vertexBatch.addToBatch(m_pats);
        // m_vertexBatch.addToBatch(chatBox);
    }


    void handleInputEvent(const SDL_Event &event) override {
        static bool m_dragging = false;
        static float m_dragOffsetX = 0.f;
        static float m_dragOffsetY = 0.f;
        switch (event.type) {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    m_windowController.setSize(200, 200);
                    m_pats = AtlasTexture(texture::food_apple, 0, 0, 200, 200);
                    m_dragging = true;
                    m_dragOffsetX = event.button.x;
                    m_dragOffsetY = event.button.y;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    m_windowController.setSize(100, 100);
                    m_pats = AtlasTexture(texture::food_apple, 0, 0, 100, 100);
                    m_dragging = false;
                }
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if (m_dragging) {
                    float gx, gy;
                    SDL_GetGlobalMouseState(&gx, &gy);
                    m_windowController.setPosition(
                        static_cast<int>(gx - m_dragOffsetX), static_cast<int>(gy - m_dragOffsetY)
                    );
                }
                break;
            default:
                break;
        }
    }
};
