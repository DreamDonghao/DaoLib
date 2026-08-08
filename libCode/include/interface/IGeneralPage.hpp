#pragma once
#include <core/frame/IPage.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/frame/WindowController.hpp>

namespace dao {
    template<typename T> concept BatchWritable =
            requires(T t, BatchRenderer &b) { t.writeToBatch(b); };

    namespace ifc {
        /// @brief 通用页面接口
        /// @details 提供的一个继承自 IPage 的通用页面接口，实现了一些页面通常应该具备的功能
        class IGeneralPage : public IPage {
        public:
            IGeneralPage(BatchRenderer *vertexBatch, Context *context, std::string_view title);

            ~IGeneralPage() override = default;

            void open() override = 0;

            void close() override = 0;

            /// @brief 获取要注册的纹理ID列表
            [[nodiscard]] std::vector<i32> getRegisterTextures() const override = 0;

            /// @brief 更新
            void update() override = 0;

            /// @brief 处理消息
            void handleInputEvent(const SDL_Event &event) override = 0;

            WindowController &getWindowController() override;

            [[nodiscard]] const std::string_view &getTitle() const override;

            template<BatchWritable... Args>
            void addToBatch(Args &&... args) {
                (args.writeToBatch(m_BatchRenderer), ...);
            }

            [[nodiscard]] Context &getContext();

            [[nodiscard]] BatchRenderer & getBatchRenderer();
        protected:
            WindowController m_windowController; ///< 窗口控制器
            [[nodiscard]] static f32 getGlyphAspectRatio(const utf32char charCode) {
                return GlyphAtlas::getGlyphAtlas().getGlyphAspectRatio(charCode);
            }

        private:
            std::string_view m_title;
            BatchRenderer &m_BatchRenderer;
            Context &m_context;
        };
    }
}
