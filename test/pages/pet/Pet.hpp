#pragma once
#include <interface/IGeneralPage.hpp>
#include <core/tool/json.hpp>
#include <components/Image.hpp>
#include <components/TextBox.hpp>
#include <components/controls/InputBox.hpp>
#include <components/graphs/Rectangle.hpp>
#include <web/http/HttpsClient.hpp>
#include <texture_id.hpp>
#include <SDL3/SDL_mouse.h>

namespace test {
    class Pet : public dao::ifc::IGeneralPage {
        static constexpr dao::f32 PET_SIZE = 80.0f;
        static constexpr dao::f32 BUBBLE_WIDTH = 250.0f;
        static constexpr dao::f32 LINE_HEIGHT = 18.0f;
        static constexpr dao::f32 INPUT_HEIGHT = 25.0f;

        dao::AnimatedSprite m_pet{1, 0, 0, 100, 100, texture::sf_number_begin, texture::sf_number_end};

        dao::TextBox m_bubble{PET_SIZE + 10, 0, BUBBLE_WIDTH, 0, LINE_HEIGHT};
        dao::InputBox m_input{PET_SIZE + 10, 0, BUBBLE_WIDTH, INPUT_HEIGHT};
        dao::Rectangle m_inputBg{PET_SIZE + 10, 0, BUBBLE_WIDTH, INPUT_HEIGHT, dao::ColorRGBA(30, 30, 30, 200)};

        bool m_dragging = false;
        dao::i32 m_offsetX = 0, m_offsetY = 0;

        dao::web::HttpsClient m_client{"api.deepseek.com", 443};
        dao::i32 m_reqId = -1;
        dao::json m_history = dao::json::array();

    public:
        Pet() : IGeneralPage("pet") {
        }

        [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override {
            std::vector<dao::i32> ret;
            for (int i = texture::sf_test_begin; i < texture::sf_test_end; ++i) {
                ret.push_back(i);
            }
            ret.push_back(texture::image);
            return ret;
        }

        void open() override {
            m_pet.beginUse();
            // 加载历史记录
            if (auto j = dao::readJson("pet_history.json")) {
                if (j->contains("history")) {
                    m_history = (*j)["history"];
                }
            }
            // 添加系统提示
            if (m_history.empty()) {
                m_history.push_back({{"role", "system"}, {"content", "你是一个可爱的桌面宠物，回复简短有趣。"}});
            }
            m_client.setTimeout(10, 60);
            updateWindowSize();
        }

        void close() override {
            // 保存历史记录
            dao::writeJson("pet_history.json", {{"history", m_history}});
        }

        void update() override {
            // 检查异步响应
            if (m_reqId >= 0 && m_client.isReady(m_reqId)) {
                if (auto resp = m_client.takeResponse(m_reqId)) {
                    if (resp->success) {
                        try {
                            auto j = dao::json::parse(resp->body);
                            std::string content = j["choices"][0]["message"]["content"];
                            m_history.push_back({{"role", "assistant"}, {"content", content}});
                            m_bubble.setContent(dao::utf8ToUtf32(content));
                            m_bubble.adjustHeightToContent(getGlyphAtlas());
                            updateWindowSize();
                        } catch (...) {
                            m_bubble.setContent(U"解析响应失败");
                        }
                    } else {
                        m_bubble.setContent(dao::utf8ToUtf32("错误: " + resp->error));
                    }
                }
                m_reqId = -1;
            }

            clearBatch();
            addToBatch(m_pet);
            if (!m_bubble.getContent().empty()) {
                addToBatch(m_bubble);
            }
            addToBatch(m_inputBg, m_input);
        }

        void handleInputEvent(const SDL_Event &event) override {
            m_input.handleEvent(event);

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RETURN) {
                if (const auto &text = m_input.getText(); !text.empty()) {
                    sendMessage(dao::utf32ToUtf8(text));
                    m_input.clear();
                }
            }

            const auto &box = m_input.getBoundingBox();
            const bool inInput = box.isInBoundingBox(
                event.type == SDL_EVENT_MOUSE_MOTION ? event.motion.x : event.button.x,
                event.type == SDL_EVENT_MOUSE_MOTION ? event.motion.y : event.button.y
            );

            if (!inInput) {
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                    m_dragging = true;
                    m_offsetX = static_cast<dao::i32>(event.button.x);
                    m_offsetY = static_cast<dao::i32>(event.button.y);
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
                    m_dragging = false;
                } else if (event.type == SDL_EVENT_MOUSE_MOTION && m_dragging) {
                    float gx, gy;
                    SDL_GetGlobalMouseState(&gx, &gy);
                    m_windowController.setPosition(
                        static_cast<dao::i32>(gx - m_offsetX),
                        static_cast<dao::i32>(gy - m_offsetY)
                    );
                }
            }
        }

    private:
        void updateWindowSize() {
            const float bubbleHeight = m_bubble.getContentHeight(getGlyphAtlas());
            constexpr float width = PET_SIZE + 10 + BUBBLE_WIDTH;
            const float height = std::max(PET_SIZE, bubbleHeight) + 5 + INPUT_HEIGHT;
            m_input.setPosition(PET_SIZE + 10, std::max(PET_SIZE, bubbleHeight) + 5);
            m_inputBg.setPosition(PET_SIZE + 10, std::max(PET_SIZE, bubbleHeight) + 5);
            m_windowController.setSize(static_cast<dao::i32>(width), static_cast<dao::i32>(height));
        }

        void sendMessage(const std::string &msg) {
            m_history.push_back({{"role", "user"}, {"content", msg}});
            // 限制历史长度
            while (m_history.size() > 21) {
                // system + 10轮对话
                m_history.erase(m_history.begin() + 1);
            }

            const dao::web::Headers headers = {
                {"Content-Type", "application/json"},
                {"Authorization", "Bearer sk-5566b22b1cfd4bc2bdc55f88c29ff5ba"}
            };

            const dao::json body = {
                {"model", "deepseek-chat"},
                {"messages", m_history},
                {"stream", false}
            };

            m_bubble.setContent(U"思考中...");
            m_reqId = m_client.postAsync("/chat/completions", headers, body.dump());
        }
    };
}
