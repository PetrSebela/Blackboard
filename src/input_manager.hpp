#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <SDL2/SDL.h>

enum KeyEvent
{
    PRESSED,
    RELEASED,
    DOWN,
    UP
};

KeyEvent GetMouseButtonEvent(int key_index, int last_mouse_buttons, int current_mouse_buttons);

#endif