#pragma once
#include <SDL3/SDL.h>


#include <iostream>



class Mouse 
{
public:
    Mouse() = default;
  

    // Update mouse state from SDL_Event
    void HandleEvent(const SDL_Event& e);
 

    // Get mouse position
    void GetMousePosition(int& x, int& y) const;

    double GetDX();
    double GetDY();

    double GetScrollDX();
    double GetScrollDY();


    // Check button states
    bool IsLeftButtonPressed() const { return m_left_button_pressed; }
    bool IsRightButtonPressed() const { return m_right_button_pressed; }
    bool IsMiddleButtonPressed() const { return m_middle_button_pressed; }

private:
    int m_mouse_x = 0; 
    int m_mouse_y = 0;             // Current mouse position
    bool m_left_button_pressed = false;          // State of the left mouse button
    bool m_right_button_pressed = false;         // State of the right mouse button
    bool m_middle_button_pressed = false;        // State of the middle mouse button

    double m_dx;
    double m_dy;
    double m_scroll_dx;
    double m_scroll_dy;

    double m_last_x;
    double m_last_y;

    bool m_first_mouse = true;
};

