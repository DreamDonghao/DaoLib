//
// Created by donghao on 25-12-4.
//
#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <functional>
#include <string>
#include <SDL3/SDL.h>
#include <interface/page.hpp>

#include "app_controller.hpp"
#include "context.hpp"

namespace dao {
    /// @brief 窗口
    class Window {
        struct TextureDeleter {
            void operator()(SDL_Texture *texture) const {
                if (texture) SDL_DestroyTexture(texture);
            }
        };

    public:
        /// @param width 窗口默认宽度
        /// @param height 窗口默认高度
        /// @param hidden 隐藏
        /// @param isSubject 为应用主体窗口
        /// @param resizable 可重新设置大小
        /// @param transparent 支持透明
        /// @param onTop 置顶
        /// @param borderless 无边框
        Window(uint32 width, uint32 height, bool hidden = false, bool isSubject = false,
               bool resizable = false, bool transparent = false, bool
               onTop = false, bool borderless = false);

        ~Window();

        /// @brief 添加页面
        /// @param page 要添加页面的unique_ptr指针
        Window &addPage(std::unique_ptr<Page> &&page);

        /// @brief 加载纹理对应的纹理图集
        void registerTexture(const uint32 &textureId);

        /// @brief 根据当前窗口拥有的页面加载未加载的图集
        void registerPageTexture();

        /// @brief 获取 id
        [[nodiscard]] uint32 getId() const { return m_id; }

        /// @brief 更新一帧
        void update();

        /// @brief 处理消息
        void handleInputEvent(const SDL_Event &event);

        /// @brief 隐藏窗口
        void hide() const;

        /// @brief 展示窗口
        void show() const;

        /// @brief 销毁窗口
        void destroy();

        /// @brief 创建窗口
        void create();

        /// @brief 判断是否在运行
        [[nodiscard]] bool isRunning() const { return m_running; }

        /// @brief 渲染
        void render();

        /// @brief 获取 SDL_window 指针
        [[nodiscard]] const SDL_Window *getSDLWindow() const { return m_window; }

        /// @brief 切换页面
        void switchPage(std::string title);

        /// @brief 设置位置
        void setPosition(uint32 x, uint32 y) const;

        /// @brief 设置大小
        void setSize(uint32 width, uint32 height) const;

        /// @brief 设置标题
        void setTitle(const std::string &title) const;

        /// @brief 设置点击是否穿透
        void setClickThrough(bool enable) const;

        AppController &getAppController() { return m_appController; }

        void setContext(Context *context) {
            m_context = context;
        }

    private:
        uint32 m_id{0xFFFFFFFF};                               ///< ID
        bool m_running = true;                                 ///< 是否正在运行
        SDL_Window *m_window{nullptr};                         ///< SDL_Window 指针
        SDL_Renderer *m_renderer{nullptr};                     ///< SDL_Renderer 指针
        hash_map<uint32, SDL_Texture *> m_atlasTextures;       ///< 纹理图集
        std::string m_nowPageTitle;                            ///< 当前页面的标题
        hash_map<std::string, std::unique_ptr<Page> > m_pages; ///< 窗口拥有的页面
        uint32 m_width;                                        ///< 窗口宽度
        uint32 m_height;                                       ///< 窗口高度
        SDL_WindowFlags m_windowFlags = 0;                     ///< 窗口属性标记
        AppController m_appController;                         ///< 应用控制器

        Context *m_context = nullptr;

        /// @brie 执行窗口控制器的命令
        void executeCommand();

        std::function<void()> m_closeAction{
            [] {
            }
        };
    };
}
#endif //WINDOW_HPP
