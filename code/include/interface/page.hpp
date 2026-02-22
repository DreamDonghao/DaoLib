//
// Created by donghao on 25-11-26.
//
#ifndef PAGE_HPP
#define PAGE_HPP
#include <core/frame/vertex_batch_builder.hpp>
#include <core/frame/window_controller.hpp>

namespace dao {
    class Context;

    /// @brief 页面接口
    /// @details 提供了所有页面的接口，所有页面最底层都应继承这个类
    class Page {
    public:
        virtual ~Page() = default;

        virtual void init() = 0;

        virtual void close() = 0;

        /// @brief 获取加载图集
        [[nodiscard]] virtual std::vector<u32> getRegisterTexture() const = 0;

        /// @brief 更新
        virtual void update() = 0;

        /// @brief 处理消息
        virtual void handleInputEvent(const SDL_Event &event) = 0;

        [[nodiscard]] virtual const std::vector<AtlasDrawBatch> &getDrawBatches() const = 0;

        [[nodiscard]] virtual  GlyphAtlas &getGlyphAtlas()= 0;

        /// @brief 获取窗口控制器
        virtual WindowController &getWindowController() = 0;

        /// @brief 获取页面标题
        [[nodiscard]] virtual const std::string &getTitle() const = 0;

        virtual void setContext(Context *context) = 0;

        virtual Context &getContext() const= 0;
    };
}
#endif //PAGE_HPP
