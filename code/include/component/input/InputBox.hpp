#pragma once
#include <SDL3/SDL_events.h>
#include <utf8cpp/utf8/unchecked.h>
#include <core/tool/BoundingBox.hpp>

namespace dao {
    /// @brief 输入框状态枚举
    enum class InputStatus {
        Disabled, ///< 禁用
        Normal,   ///< 自然
        Input,    ///< 输入
    };

    /// @brief 输入框组件
    /// @details 实现了基本的文本输入功能，支持中文输入
    /// @bug 使用 SDL3 的中文输入时，数字键选择候选词后偶发失效
    /// 可能是由于 SDL 事件循环与操作系统 IME 之间的"竞态条件"或事件冲突
    /// 暂未找到解决方案
    class InputBox {
    public:
        /// @brief 构造函数
        /// @param x 输入框左上角 x 坐标
        /// @param y 输入框左上角 y 坐标
        /// @param w 输入框宽度
        /// @param h 输入框高度
        /// @param status 输入框初始状态，默认为Normal
        InputBox(const f32 x, const f32 y, const f32 w, const f32 h,
                 const InputStatus status = InputStatus::Normal)
            : m_status(status), m_boundingBox(x, y, x + w, y + h) {
        }

        /// @brief 处理鼠标和键盘事件
        /// @param event SDL 事件对象，处理鼠标点击、键盘输入等事件
        void handleEvent(const SDL_Event &event) {
            if (m_status == InputStatus::Disabled) {
                return;
            }
            float x = 0, y = 0;
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                x = event.motion.x;
                y = event.motion.y;
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                x = event.button.x;
                y = event.button.y;
            }

            SDL_Window *win = SDL_GetWindowFromID(event.window.windowID);
            if (m_status == InputStatus::Input) {
                const SDL_Rect rect{
                    static_cast<int>(m_boundingBox.getLeft()),
                    static_cast<int>(m_boundingBox.getTop()),   // 顶部
                    static_cast<int>(m_boundingBox.getWidth()), // 宽度
                    static_cast<int>(m_boundingBox.getHeight()) // 高度
                };
                SDL_SetTextInputArea(win, &rect, cursorPos);
                if (event.type == SDL_EVENT_TEXT_INPUT) {
                    const char *utf8chars = event.text.text;
                    const auto begin = utf8::unchecked::iterator(utf8chars);
                    const auto end = utf8::unchecked::iterator(utf8chars + std::strlen(utf8chars));

                    for (auto it = begin; it != end; ++it) {
                        m_text.insert(m_text.begin() + cursorPos, static_cast<char32_t>(*it));
                        cursorPos += 1;
                    }
                } else if (event.type == SDL_EVENT_KEY_DOWN) {
                    if (event.key.key == SDLK_BACKSPACE && !m_text.empty()) {
                        if (cursorPos != 0) {
                            m_text.erase(cursorPos - 1, 1);
                            --cursorPos;
                        }
                    } else if (event.key.key == SDLK_DELETE) {
                        if (cursorPos != m_text.size()) {
                            m_text.erase(cursorPos, 1);
                        }
                    } else if (event.key.key == SDLK_LEFT) {
                        cursorPos = std::max(cursorPos - 1, 0);
                    } else if (event.key.key == SDLK_RIGHT) {
                        cursorPos = std::min(cursorPos + 1, static_cast<int>(m_text.size()));
                    }
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (!m_boundingBox.isInBoundingBox(x, y)) {
                        m_status = InputStatus::Normal;
                        SDL_StopTextInput(win);
                    }
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (m_boundingBox.isInBoundingBox(x, y)) {
                    m_status = InputStatus::Input;
                    SDL_StartTextInput(win);
                }
            }
        }

        /// @brief 获取边界框
        /// @return 输入框的边界框
        [[nodiscard]] BoundingBox getBoundingBox() const {
            return m_boundingBox;
        }

        /// @brief 获取当前状态
        /// @return 输入框当前的状态
        [[nodiscard]] InputStatus getStatus() const {
            return m_status;
        }

        /// @brief 获取光标位置
        /// @return 光标的当前位置
        [[nodiscard]] int getCursorPos() const {
            return cursorPos;
        }

        /// @brief 获取输入文本
        /// @return 输入框中存储的UTF-32字符串
        [[nodiscard]] const std::u32string &getText() const {
            return m_text;
        }

    private:
        InputStatus m_status;      ///< 输入框当前状态
        BoundingBox m_boundingBox; ///< 输入框边界框
        utf32str m_text;           ///< 输入的文本内容
        int cursorPos = 0;         ///< 光标位置
    };
}