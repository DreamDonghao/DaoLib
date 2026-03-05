#pragma once
#include <component/input/InputBox.hpp>
#include <core/render/Text.hpp>
#include <core/render/BatchRenderer.hpp>
#include <interface/IDrawable.hpp>
#include <core/tool/TickTimer.hpp>

#include "component/graph/SolidRect.hpp"

namespace dao {
    class BatchRenderer;

    class InputBoxStyle : public ifc::IDrawable {
    public:
        explicit InputBoxStyle(InputBox &inputBox, const int flickerInterval = 3000)
            : m_inputBox(&inputBox),
              m_rectangle(inputBox.getBoundingBox().getLeft(), inputBox.getBoundingBox().getTop(),
                          inputBox.getBoundingBox().getWidth(), inputBox.getBoundingBox().getHeight(),
                          ColorRGBA("#FFFFFF")),
              m_text(inputBox.getBoundingBox().getLeft(), inputBox.getBoundingBox().getTop(), 50,
                     ColorRGBA("#66E656"),
                     U""),
              m_flickerInterval(flickerInterval),
              m_flickerTimer(std::chrono::milliseconds(flickerInterval)) {
        }

        void writeToBatch(BatchRenderer &builder) const override {
            m_rectangle.writeToBatch(builder);
            m_text.setContent(m_inputBox->getText());
            if (m_inputBox->getStatus() == InputStatus::Input) {
                if (static_cast<bool>(m_flickerTimer)) {
                    m_cursorShown = !m_cursorShown;
                }
                if (m_cursorShown) {
                    m_text.insert(m_inputBox->getCursorPos(), '|');
                }
            }
            builder.addToBatch(m_text);
        }

    private:
        InputBox *m_inputBox;
        SolidRect m_rectangle;
        mutable Text m_text;
        int m_flickerInterval;
        mutable TickTimer m_flickerTimer;
        mutable bool m_cursorShown{false};
    };
}
