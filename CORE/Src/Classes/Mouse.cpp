#include "Mouse.h"

void Mouse::HandleEvent(const SDL_Event& e)
{
    if (e.type == SDL_EVENT_MOUSE_MOTION)
    {
        m_mouse_x = e.motion.x;
        m_mouse_y = e.motion.y;

        if (m_first_mouse)
        {
            m_last_x = m_mouse_x;
            m_last_y = m_mouse_y;
            m_first_mouse = false;
        }

        m_dx = m_mouse_x - m_last_x;
        m_dy = m_last_y - m_mouse_y; // y coordinates are inverted
        m_last_x = m_mouse_x;
        m_last_y = m_mouse_y;
    }
    else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) 
    {
        bool isPressed = (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
        switch (e.button.button) 
        {
        case SDL_BUTTON_LEFT:
            m_left_button_pressed = isPressed;
            break;
        case SDL_BUTTON_RIGHT:
            m_right_button_pressed = isPressed;
            break;
        case SDL_BUTTON_MIDDLE:
            m_middle_button_pressed = isPressed;
            break;
        default:
            break;
        }
    }
    else if (e.type == SDL_EVENT_MOUSE_WHEEL) 
    {
        m_scroll_dy = e.wheel.y;
    }
}

void Mouse::GetMousePosition(int& x, int& y) const
{
    x = m_mouse_x;
    y = m_mouse_y;
}

double Mouse::GetDX()
{
    double _dx = m_dx;
    // set to 0 because change no longer new
    m_dx = 0;
    return _dx;
}

double Mouse::GetDY()
{
    double _dy = m_dy;
    // set to 0 because change no longer new
    m_dy = 0;
    return _dy;
}

double Mouse::GetScrollDX()
{
    double _scroll_dx = m_scroll_dx;
    // set to 0 because change no longer new
    m_scroll_dx = 0;
    return _scroll_dx;
}

double Mouse::GetScrollDY()
{
    double _scroll_dy = m_scroll_dy;
    // set to 0 because change no longer new
    m_scroll_dy = 0;
    return _scroll_dy;
}
