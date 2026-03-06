#pragma once
#include <functional>
#include <string>
#include <SDL3/SDL.h>
#include <interface/IPage.hpp>
#include <core/frame/AppController.hpp>
#include <core/frame/Context.hpp>
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
        Window &addPage(std::unique_ptr<ifc::IPage> &&page);

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

        /// @brief 移动窗口位置
        /// @param x 新的 x 坐标
        /// @param y 新的 y 坐标
        void movePosition(i32 x, i32 y) const;

        /// @brief 设置大小
        void setSize(i32 width, i32 height) const;

        /// @brief 设置标题
        void setTitle(const std::string &title) const;

        /// @brief 设置点击是否穿透
        void setClickThrough(bool enable) const;

        /// @brief 获取应用控制器
        /// @return 应用控制器引用
        AppController &getAppController() { return m_appController; }

        /// @brief 设置上下文
        /// @param context 上下文指针
        void setContext(Context *context) {
            m_context = context;
        }

    private:
        i32 m_id{-1};                                          ///< ID
        bool m_running = true;                                 ///< 是否正在运行
        SDL_Window *m_window{nullptr};                         ///< SDL_Window 指针
        std::string m_nowPageTitle;                            ///< 当前页面的标题
        hash_map<std::string, std::unique_ptr<ifc::IPage> > m_pages; ///< 窗口拥有的页面
        i32 m_width;                                           ///< 窗口宽度
        i32 m_height;                                          ///< 窗口高度
        SDL_WindowFlags m_windowFlags = 0;                     ///< 窗口属性标记
        AppController m_appController;                         ///< 应用控制器
        BatchRenderer m_batchRenderer{"./assets/ttf/zh-cn.ttf"}; ///< 批处理渲染器
        Context *m_context{nullptr};                           ///< 上下文

        /// @brief 执行窗口控制器的命令
        void executeCommand();

        std::function<void()> m_closeAction{                   ///< 窗口关闭时执行的操作
            [] {
            }
        };
    };
}