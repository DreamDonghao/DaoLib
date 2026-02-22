//
// Created by donghao on 25-12-20.
//
#ifndef INPUT_BOX_STYLE_HPP
#define INPUT_BOX_STYLE_HPP
#include "input_box.hpp"
#include "core/basic_drawing_elements/text.hpp"
#include "core/frame/vertex_batch_builder.hpp"
#include "interface/drawable.hpp"

namespace dao {
    class VertexBatchBuilder;

    class InputBoxStyle : public Drawable {
    public:
        explicit InputBoxStyle(InputBox &inputBox, const int flickerInterval = 3000)
            : m_inputBox(&inputBox), m_rectangle(inputBox.getBoundingBox(), hexToRGBA("#FFFFFF")),
              m_text(inputBox.getBoundingBox().getLeft(), inputBox.getBoundingBox().getTop(), 50,
                     hexToRGBA("#66E656"),
                     U""),
              m_flickerInterval(flickerInterval) {
        }

        void writeToBatch(VertexBatchBuilder &builder) const override {
            static int sustained = 0;
            static bool isShown = false;
            m_rectangle.writeToBatch(builder);
            m_text.setContent(m_inputBox->getText());
            if (m_inputBox->getStatus() == InputStatus::Input) {
                if (++sustained == m_flickerInterval) {
                    isShown = !isShown;
                    sustained = 0;
                }
                if (isShown) {
                    m_text.insert(m_inputBox->getCursorPos(), '|');
                }
            }
            builder.addToBatch(m_text);
        }

    private:
        InputBox *m_inputBox;
        Rectangle m_rectangle;
        mutable Text m_text;
        int m_flickerInterval;
    };
}
#endif //INPUT_BOX_STYLE_HPP
