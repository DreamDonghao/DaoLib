
#include <component/button/SimpleButton.hpp>

dao::SimpleButton::SimpleButton(
    const f32 x, const f32 y, const f32 width, const f32 height,
    std::move_only_function<void()> onClick, const ButtonStatus buttonStatus)
    : m_boundingBox(x, y, x + width, y + height),
      m_status(buttonStatus), m_onClick(std::move(onClick)) {
}

void dao::SimpleButton::handleEvent(const SDL_Event &event) {
    if (m_status == ButtonStatus::Disabled) {
        return;
    }
    float x = 0, y = 0;
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        x = event.motion.x;
        y = event.motion.y;
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
               event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        x = event.button.x;
        y = event.button.y;
    } else {
        return;
    }
    if (!m_boundingBox.isInBoundingBox(x, y)) {
        m_status = ButtonStatus::Normal;
        return;
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
        m_status = ButtonStatus::Pressed;
        return;
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
        event.button.button == SDL_BUTTON_LEFT &&
        m_status == ButtonStatus::Pressed) {
        m_status = ButtonStatus::Hover;
        m_onClick();
        return;
    }
    m_status = ButtonStatus::Hover;
}
