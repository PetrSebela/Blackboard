#include "input_manager.hpp"


KeyEvent GetMouseButtonEvent(int key_index, int last_mouse_buttons, int current_mouse_buttons)
{
    bool past_down = ((last_mouse_buttons >> key_index) & 1) == 1;
    bool current_down = ((current_mouse_buttons >> key_index) & 1) == 1;
   
    KeyEvent key_event = KeyEvent::UP;
    
    if (past_down && current_down)
        key_event = KeyEvent::DOWN;
    else if (past_down && !current_down)
        key_event = KeyEvent::RELEASED;
    else if (!past_down && current_down)
        key_event = KeyEvent::PRESSED;
    else if (!past_down && !current_down)
        key_event = KeyEvent::UP;
    return key_event;
}