#pragma once
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_clipboard.h>
#include <utf8cpp/utf8/unchecked.h>
#include <core/tool/BoundingBox.hpp>
#include <core/render/IDrawable.hpp>
#include <components/Text.hpp>
#include <components/graphs/Rectangle.hpp>

namespace dao {
    enum class InputStatus {
        Disabled,
        Normal,
        Input,
    };

    /// @brief 输入框组件
    class InputBox : public ifc::IDrawable {
    public:
        InputBox(const f32 x, const f32 y, const f32 w, const f32 h,
                 const InputStatus status = InputStatus::Normal)
            : m_status(status), m_boundingBox(x, y, x + w, y + h),
              m_text(x + 4, y, w - 8, h - 4, Yellow),
              m_placeholder(x + 4, y, w - 8, h - 4, ColorRGBA(120, 120, 120, 1.0f)),
              m_rectBg(x, y, w, h, ColorRGBA(30, 30, 30, 1.0f)),
              m_rectBorder(x, y, w, h, ColorRGBA(100, 100, 100, 1.0f)) {
        }

        void handleEvent(const SDL_Event &event) {
            if (m_status == InputStatus::Disabled) return;

            f32 mx = 0, my = 0;
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                mx = event.motion.x; my = event.motion.y;
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                mx = event.button.x; my = event.button.y;
            }

            SDL_Window *win = SDL_GetWindowFromID(event.window.windowID);

            if (m_status == InputStatus::Input) {
                // 更新 IME 光标位置
                const SDL_Rect rect{
                    static_cast<int>(m_boundingBox.getLeft()),
                    static_cast<int>(m_boundingBox.getTop()),
                    static_cast<int>(m_boundingBox.getWidth()),
                    static_cast<int>(m_boundingBox.getHeight())
                };
                SDL_SetTextInputArea(win, &rect, m_cursorPos);

                if (event.type == SDL_EVENT_TEXT_INPUT) {
                    deleteSelection();
                    const char *utf8chars = event.text.text;
                    const auto begin = utf8::unchecked::iterator<const char *>(utf8chars);
                    const auto end = utf8::unchecked::iterator<const char *>(utf8chars + std::strlen(utf8chars));
                    for (auto it = begin; it != end; ++it) {
                        m_str.insert(m_str.begin() + m_cursorPos, static_cast<char32_t>(*it));
                        ++m_cursorPos;
                    }
                } else if (event.type == SDL_EVENT_KEY_DOWN) {
                    handleKeyDown(event.key, win);
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (!m_boundingBox.isInBoundingBox(mx, my)) {
                        m_status = InputStatus::Normal;
                        m_selectionStart = -1;
                        SDL_StopTextInput(win);
                    }
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (m_boundingBox.isInBoundingBox(mx, my)) {
                    m_status = InputStatus::Input;
                    m_selectionStart = -1;
                    m_cursorBlinkTime = 0;
                    m_cursorVisible = true;
                    SDL_StartTextInput(win);
                }
            }
        }

        void tickBlink(const f32 dt) {
            if (m_status != InputStatus::Input) return;
            m_cursorBlinkTime += dt;
            if (m_cursorBlinkTime >= 0.5f) {
                m_cursorBlinkTime -= 0.5f;
                m_cursorVisible = !m_cursorVisible;
            }
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            // 背景和边框
            if (m_status == InputStatus::Input) {
                m_rectBorder.setColor(ColorRGBA(100, 180, 255, 1.0f));
            } else {
                m_rectBorder.setColor(ColorRGBA(100, 100, 100, 1.0f));
            }
            m_rectBg.writeToBatch(batchRenderer);
            m_rectBorder.writeToBatch(batchRenderer);

            const f32 pad = 4;
            const f32 visibleLeft = m_boundingBox.getLeft() + pad;
            const f32 visibleRight = m_boundingBox.getRight() - pad;
            const f32 visibleWidth = visibleRight - visibleLeft;

            const auto &atlas = GlyphAtlas::getGlyphAtlas();
            const f32 fontScale = m_text.lineHeight() / atlas.getFontSize();

            // 计算光标在文本空间中的 x 坐标（从文本基线起算）
            f32 cursorLocalX = 0;
            for (i32 i = 0; i < m_cursorPos && i < static_cast<i32>(m_str.size()); ++i) {
                cursorLocalX += atlas.getGlyphAdvance(m_str[i]) * fontScale;
            }

            // 调整滚动，保持光标可见
            const f32 cursorScreenX = visibleLeft - m_scrollX + cursorLocalX;
            if (cursorScreenX > visibleRight) {
                m_scrollX = cursorLocalX - (visibleWidth - 1);
            } else if (cursorScreenX < visibleLeft) {
                m_scrollX = cursorLocalX;
            }
            m_scrollX = std::max(0.0f, m_scrollX);

            m_cursorPixelX = visibleLeft - m_scrollX + cursorLocalX;

            // 设置文本位置和裁剪参数
            m_text.setPosition(visibleLeft, m_text.y());
            m_text.setWidth(visibleWidth);
            m_text.setScrollX(m_scrollX);

            m_placeholder.setPosition(visibleLeft, m_placeholder.y());
            m_placeholder.setWidth(visibleWidth);

            // 文本或 placeholder
            if (!m_str.empty()) {
                m_text.setContent(m_str);
                m_text.writeToBatch(batchRenderer);
            } else if (m_status != InputStatus::Input) {
                m_placeholder.setContent(m_placeholderText);
                m_placeholder.writeToBatch(batchRenderer);
            }

            // 选区高亮
            if (m_selectionStart >= 0 && m_selectionStart != m_cursorPos) {
                const i32 selStart = std::min(m_selectionStart, m_cursorPos);
                const i32 selEnd = std::max(m_selectionStart, m_cursorPos);

                f32 selLocalX = 0, endLocalX = 0;
                for (i32 i = 0; i < selEnd; ++i) {
                    if (i == selStart) selLocalX = endLocalX;
                    if (i < static_cast<i32>(m_str.size())) {
                        endLocalX += atlas.getGlyphAdvance(m_str[i]) * fontScale;
                    }
                }

                const f32 selX = visibleLeft - m_scrollX + selLocalX;
                const f32 selW = visibleLeft - m_scrollX + endLocalX - selX;
                if (selW > 0) {
                    const SDL_FColor selColor{0.2f, 0.4f, 0.8f, 0.5f};
                    SDL_Vertex *v = batchRenderer.allocateVertices(0, 6);
                    v[0] = {{selX, m_text.y()}, selColor, {0, 0}};
                    v[1] = {{selX + selW, m_text.y()}, selColor, {1, 0}};
                    v[2] = {{selX + selW, m_text.y() + m_text.lineHeight()}, selColor, {1, 1}};
                    v[3] = v[2];
                    v[4] = {{selX, m_text.y() + m_text.lineHeight()}, selColor, {0, 1}};
                    v[5] = v[0];
                }
            }

            // 光标线
            if (m_status == InputStatus::Input && m_cursorVisible) {
                const f32 cy = m_text.y() + 2;
                const f32 ch = m_text.lineHeight() - 4;
                const SDL_FColor cColor{1, 1, 1, 1};
                SDL_Vertex *v = batchRenderer.allocateVertices(0, 6);
                const f32 cw = 1.5f;
                v[0] = {{m_cursorPixelX, cy}, cColor, {0, 0}};
                v[1] = {{m_cursorPixelX + cw, cy}, cColor, {1, 0}};
                v[2] = {{m_cursorPixelX + cw, cy + ch}, cColor, {1, 1}};
                v[3] = v[2];
                v[4] = {{m_cursorPixelX, cy + ch}, cColor, {0, 1}};
                v[5] = v[0];
            }
        }

        // === 新增 API ===
        void setText(const utf32str &str) { m_str = str; m_cursorPos = static_cast<i32>(str.size()); m_selectionStart = -1; m_scrollX = 0; }
        void setPlaceholder(const utf32str &str) { m_placeholderText = str; }
        void setCursorPos(const i32 pos) { m_cursorPos = std::clamp(pos, 0, static_cast<i32>(m_str.size())); }
        void selectAll() { m_selectionStart = 0; m_cursorPos = static_cast<i32>(m_str.size()); }
        void clearSelection() { m_selectionStart = -1; }

        // === 已有 API ===
        [[nodiscard]] BoundingBox getBoundingBox() const { return m_boundingBox; }
        [[nodiscard]] InputStatus getStatus() const { return m_status; }
        [[nodiscard]] i32 getCursorPos() const { return m_cursorPos; }
        [[nodiscard]] const utf32str &getText() const { return m_str; }

        [[nodiscard]] utf32str getSelectedText() const {
            if (m_selectionStart < 0) return U"";
            const i32 s = std::min(m_selectionStart, m_cursorPos);
            const i32 e = std::max(m_selectionStart, m_cursorPos);
            return m_str.substr(s, e - s);
        }

    private:
        InputStatus m_status;
        BoundingBox m_boundingBox;
        utf32str m_str;
        utf32str m_placeholderText = U"";
        i32 m_cursorPos = 0;
        i32 m_selectionStart = -1;

        mutable f32 m_cursorPixelX = 0;
        mutable Text m_text;
        mutable Text m_placeholder;
        mutable Rectangle m_rectBg;
        mutable Rectangle m_rectBorder;

        mutable f32 m_scrollX = 0;
        mutable f32 m_cursorBlinkTime = 0;
        mutable bool m_cursorVisible = true;

        void deleteSelection() {
            if (m_selectionStart < 0) return;
            const i32 s = std::min(m_selectionStart, m_cursorPos);
            const i32 e = std::max(m_selectionStart, m_cursorPos);
            m_str.erase(s, e - s);
            m_cursorPos = s;
            m_selectionStart = -1;
        }

        void

        copyToClipboard() const {
            if (const auto sel = getSelectedText(); !sel.empty()) {
                const std::string utf8 = utf32ToUtf8(sel);
                SDL_SetClipboardText(utf8.c_str());
            }
        }

        void pasteFromClipboard() {
            if (!SDL_HasClipboardText()) return;
            char *text = SDL_GetClipboardText();
            if (!text) return;
            deleteSelection();
            const auto begin = utf8::unchecked::iterator<const char *>(text);
            const auto end = utf8::unchecked::iterator<const char *>(text + std::strlen(text));
            for (auto it = begin; it != end; ++it) {
                m_str.insert(m_str.begin() + m_cursorPos, *it);
                ++m_cursorPos;
            }
            SDL_free(text);
        }

        void handleKeyDown(const SDL_KeyboardEvent &key, SDL_Window *win) {
            const bool ctrl = (key.mod & SDL_KMOD_CTRL) != 0;
            const bool shift = (key.mod & SDL_KMOD_SHIFT) != 0;

            // Ctrl shortcuts
            if (ctrl && key.key == SDLK_A) { selectAll(); return; }
            if (ctrl && key.key == SDLK_C) { copyToClipboard(); return; }
            if (ctrl && key.key == SDLK_X) { copyToClipboard(); deleteSelection(); return; }
            if (ctrl && key.key == SDLK_V) { pasteFromClipboard(); return; }

            if (key.key == SDLK_BACKSPACE && !m_str.empty()) {
                if (m_selectionStart >= 0) { deleteSelection(); return; }
                if (m_cursorPos > 0) { m_str.erase(m_cursorPos - 1, 1); --m_cursorPos; }
            } else if (key.key == SDLK_DELETE) {
                if (m_selectionStart >= 0) { deleteSelection(); return; }
                if (m_cursorPos < static_cast<i32>(m_str.size())) { m_str.erase(m_cursorPos, 1); }
            } else if (key.key == SDLK_LEFT) {
                if (m_selectionStart < 0 && shift) m_selectionStart = m_cursorPos;
                m_cursorPos = std::max(0, m_cursorPos - 1);
                if (!shift) m_selectionStart = -1;
            } else if (key.key == SDLK_RIGHT) {
                if (m_selectionStart < 0 && shift) m_selectionStart = m_cursorPos;
                m_cursorPos = std::min(static_cast<i32>(m_str.size()), m_cursorPos + 1);
                if (!shift) m_selectionStart = -1;
            } else if (key.key == SDLK_HOME) {
                if (m_selectionStart < 0 && shift) m_selectionStart = m_cursorPos;
                m_cursorPos = 0;
                if (!shift) m_selectionStart = -1;
            } else if (key.key == SDLK_END) {
                if (m_selectionStart < 0 && shift) m_selectionStart = m_cursorPos;
                m_cursorPos = static_cast<i32>(m_str.size());
                if (!shift) m_selectionStart = -1;
            } else if (key.key == SDLK_RETURN || key.key == SDLK_ESCAPE) {
                m_status = InputStatus::Normal;
                m_selectionStart = -1;
                SDL_StopTextInput(win);
            }
        }
    };
}