//
// Created by donghao on 25-12-20.
//
#ifndef GENERAL_PAGE_HPP
#define GENERAL_PAGE_HPP
#include "page.hpp"
#include <utility>

namespace dao {
    /// @brief 通用页面接口
    /// @details 提供的一个继承自 Page 的通用页面接口，实现了一些页面通常应该具备的功能
    class GeneralPage : public Page {
    public:
        ~GeneralPage() override = default;

        void init() override = 0;

        void close() override = 0;

        /// @brief 获取加载图集
        [[nodiscard]] std::vector<uint32> getRegisterTexture() const override = 0;

        /// @brief 更新
        void update() override = 0;

        /// @brief 处理消息
        void handleInputEvent(const SDL_Event &event) override = 0;


        explicit GeneralPage(std::string title) : m_title(std::move(title)) {
        }

        [[nodiscard]] const std::vector<AtlasDrawBatch> &getDrawBatches() const override {
            return m_vertexBatch.getDrawBatches();
        }

        GlyphAtlas &getGlyphAtlas() override {
            return m_vertexBatch.getGlyphAtlas();
        }

        PageCmdQueue &getEvent() override {
            return m_event;
        }

        [[nodiscard]] const std::string &getTitle() const override {
            return m_title;
        }

    protected:
        std::string m_title;
        PageCmdQueue m_event;
        VertexBatchBuilder m_vertexBatch{"zh-cn.ttf", 64, 1024};
    };
}
#endif //GENERAL_PAGE_HPP
