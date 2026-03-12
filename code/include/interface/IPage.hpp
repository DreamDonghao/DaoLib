#pragma once
#include <core/tool/type.hpp>
#include <vector>
union SDL_Event;
namespace dao {
    class BatchRenderer;
    class WindowController;
    class Context;

    namespace ifc {
        /// @brief 页面接口
        /// @details 提供了所有页面的接口，所有页面最底层都应继承这个类
        class IPage {
        public:
            virtual ~IPage() = default;

            /// @brief 初始化页面
            /// @details 每次打开页面时执行
            virtual void open() = 0;

            /// @brief 关闭页面
            /// @details 每次关闭页面时执行
            virtual void close() = 0;

            /// @brief 获取要注册的纹理ID列表
            /// @details 用于预加载纹理
            [[nodiscard]] virtual std::vector<i32> getRegisterTextures() const = 0;

            /// @brief 更新
            virtual void update() = 0;

            /// @brief 处理消息
            virtual void handleInputEvent(const SDL_Event &event) = 0;

            /// @brief 获取窗口控制器
            virtual WindowController &getWindowController() = 0;

            /// @brief 获取全局上下文
            virtual void setContext(Context *context) = 0;

            /// @brief 设置渲染批处理器
            virtual void setVertexBatch(BatchRenderer *batchRenderer) = 0;

            /// @brief 获取页面标题
            [[nodiscard]] virtual const std::string &getTitle() const = 0;

            /// @brief 设置全局环境上下文
            [[nodiscard]] virtual Context &getContext() const = 0;
        };
    }
}
