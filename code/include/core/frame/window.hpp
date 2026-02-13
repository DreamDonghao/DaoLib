//
// Created by donghao on 25-12-4.
//
#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <string>
#include <SDL3/SDL.h>
#include <interface/page.hpp>

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
        /// @param resizable 可重新设置大小
        /// @param transparent 支持透明
        /// @param onTop 置顶
        /// @param borderless 无边框
        Window(uint32 width, uint32 height,
               bool resizable = false, bool transparent = false, bool onTop = false, bool borderless = false);

        ~Window();

        /// @brief 添加页面
        /// @param page 要添加页面的unique_ptr指针
        Window &addPage(std::unique_ptr<Page> &&page);

        /// @brief 加载纹理图集
        void registerTexture(const uint32 &textureId);

        /// @brief 获取 id
        [[nodiscard]] uint32 getId() const { return m_id; }

        /// @brief 更新一帧
        void update();

        /// @brief 处理消息
        void handleInputEvent(const SDL_Event &event);

        /// @brief 窗口关闭请求
        void requestClose();

        /// @brief 判断是否在运行
        [[nodiscard]] bool isRunning() const { return m_running; }

        /// @brief 渲染
        void render();

        [[nodiscard]] const SDL_Window *getSDLWindow() const {
            return m_window;
        }

        /// @brief 切换页面
        void switchPage(std::string title);

        void setPosition(uint32 x, uint32 y) const;

        void setSize(uint32 width, uint32 height) const;

        void setTitle(const std::string& title) const;

        void setClickThrough(bool enable) const;
    private:
        uint32 m_id;           ///< ID
        bool m_running = true; ///< 是否正在运行
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        hash_map<uint32, SDL_Texture *> m_atlasTextures;
        std::string m_nowPageTitle;
        hash_map<std::string, std::unique_ptr<Page> > m_pages;

        /// @brie 执行窗口控制器的命令
        void executeCommand();
    };
}
#endif //WINDOW_HPP
