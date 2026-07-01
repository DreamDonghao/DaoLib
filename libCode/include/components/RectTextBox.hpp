#pragma once
#include <components/Text.hpp>
#include <components/graphs/Rectangle.hpp>

namespace dao {
    class RectTextBox : public dao::ifc::IDrawable {
    public:
        RectTextBox(f32 x, f32 y, f32 width, f32 height,f32 fontHeigh, ColorRGBA color, ColorRGBA backgroundColor, utf32str content)
            : m_rectangle(x, y, width, height, backgroundColor),
              m_text(x,y,width,height,fontHeigh, color, std::move(content)) {
        }


        void writeToBatch(BatchRenderer &batchRenderer) const override {
            m_rectangle.writeToBatch(batchRenderer);
            m_text.writeToBatch(batchRenderer);
        }

    private:
        Rectangle m_rectangle;
        Text m_text;
    };
}
