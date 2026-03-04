#pragma once
#include "page.hpp"

namespace dao {
    class Context;
    template<typename T> concept BatchWritable =
            requires(T t, BatchRenderer &b) { t.writeToBatch(b); };

    /// @brief 通用页面接口
    /// @details 提供的一个继承自 Page 的通用页面接口，实现了一些页面通常应该具备的功能
    class GeneralPage : public Page {
    public:
        explicit GeneralPage(std::string title);

        ~GeneralPage() override = default;

        void open() override = 0;

        void close() override = 0;

        /// @brief 获取加载图集
        [[nodiscard]] std::vector<i32> getRegisterTexture() const override = 0;

        /// @brief 更新
        void update() override = 0;

        /// @brief 处理消息
        void handleInputEvent(const SDL_Event &event) override = 0;

        WindowController &getWindowController() override;

        [[nodiscard]] const std::string &getTitle() const override;

        template<BatchWritable... Args>
        void addToBatch(Args &&... args) {
            m_vertexBatch->clearDrawBatches();
            (args.writeToBatch(*m_vertexBatch), ...);
        }

        void setContext(Context *context) override;

        void setVertexBatch(BatchRenderer *vertexBatch) override {
            m_vertexBatch = vertexBatch;
        }

        [[nodiscard]] Context &getContext() const override;

    protected:
        WindowController m_windowController; ///< 窗口控制器
        [[nodiscard]] f32 getGlyphAspectRatio(const utf32char charCode) const {
            return m_vertexBatch->getGlyphAtlas().getGlyphAspectRatio(charCode);
        }

    private:
        std::string m_title;
        BatchRenderer *m_vertexBatch = nullptr;
        Context *m_context = nullptr;
    };
}
