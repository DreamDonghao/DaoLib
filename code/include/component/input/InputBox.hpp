#pragma once
#include <SDL3/SDL_events.h>
#include <utf8cpp/utf8/unchecked.h>
#include <core/tool/BoundingBox.hpp>

namespace dao {
    enum class InputStatus {
        Disabled, ///< 禁用
        Normal,   ///< 自然
        Input,    ///< 输入
    };

    /// @brief 输入框
    /// @bug 使用 SDL3 的中文输入时，数字键选择候选词后偶发失效
    /// 可能是由于 SDL 事件循环与操作系统 IME 之间的"竞态条件"或事件冲突
    /// 暂未找到解决方案
    class InputBox {
    public:
        InputBox(const f32 x, const f32 y, const f32 w, const f32 h,
                 const InputStatus status = InputStatus::Normal)
            : m_status(status), m_boundingBox(x, y, x + w, y + h) {
        }

        /// @brief 处理消息，更新状态
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

        [[nodiscard]] BoundingBox getBoundingBox() const {
            return m_boundingBox;
        }

        [[nodiscard]] InputStatus getStatus() const {
            return m_status;
        }

        [[nodiscard]] int getCursorPos() const {
            return cursorPos;
        }

        [[nodiscard]] const std::u32string &getText() const {
            return m_text;
        }

    private:
        InputStatus m_status;
        BoundingBox m_boundingBox;
        utf32str m_text;
        int cursorPos = 0;
    };
}
