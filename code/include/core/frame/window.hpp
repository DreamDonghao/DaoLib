#pragma once
#include <functional>
#include <string>
#include <SDL3/SDL.h>
#include <interface/page.hpp>
#include <core/frame/app_controller.hpp>
#include <core/frame/context.hpp>
#include <core/render/BatchRenderer.hpp>

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
        Window(i32 width, i32 height, bool hidden = false, bool isSubject = false,
               bool resizable = false, bool transparent = false, bool
               onTop = false, bool borderless = false);

        ~Window();

        /// @brief 添加页面
        /// @param page 要添加页面的unique_ptr指针
        Window &addPage(std::unique_ptr<Page> &&page);

        /// @brief 根据当前窗口拥有的页面加载未加载的图集
        void registerPageTexture();

        /// @brief 获取 id
        [[nodiscard]] i32 getId() const { return m_id; }

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

        /// @brief 获取 SDL_window 指针
        [[nodiscard]] const SDL_Window *getSDLWindow() const { return m_window; }

        /// @brief 切换页面
        void switchPage(std::string title);

        /// @brief 设置位置
        void setPosition(i32 x, i32 y) const;

        void movePosition(i32 x, i32 y) const;

        /// @brief 设置大小
        void setSize(i32 width, i32 height) const;

        /// @brief 设置标题
        void setTitle(const std::string &title) const;

        /// @brief 设置点击是否穿透
        void setClickThrough(bool enable) const;

        AppController &getAppController() { return m_appController; }

        void setContext(Context *context) {
            m_context = context;
        }

    private:
        i32 m_id{-1};                                          ///< ID
        bool m_running = true;                                 ///< 是否正在运行
        SDL_Window *m_window{nullptr};                         ///< SDL_Window 指针
        std::string m_nowPageTitle;                            ///< 当前页面的标题
        hash_map<std::string, std::unique_ptr<Page> > m_pages; ///< 窗口拥有的页面
        i32 m_width;                                           ///< 窗口宽度
        i32 m_height;                                          ///< 窗口高度
        SDL_WindowFlags m_windowFlags = 0;                     ///< 窗口属性标记
        AppController m_appController;                         ///< 应用控制器
        BatchRenderer m_batchRenderer{"./assets/ttf/zh-cn.ttf"};
        Context *m_context{nullptr};

        /// @brief 执行窗口控制器的命令
        void executeCommand();

        std::function<void()> m_closeAction{
            [] {
            }
        };
    };
}
