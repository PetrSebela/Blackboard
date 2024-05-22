#ifndef TOOL_MANAGER_H_
#define TOOL_MANAGER_H_

#include "tool_types.hpp"
#include <SDL2/SDL.h>
#include "canvas.hpp"
#include "input_manager.hpp"

class ToolManager
{
private:
    ToolType default_tool = ToolType::Brush;
    bool toolbox_states[ToolType::TOOL_COUNT] = {false};
    int last_mouse_buttons = 0;

    ToolType active_tool;
    Canvas *canvas;

    int mouse_x, mouse_y;
    Vector2 mouse_position;
    KeyEvent key_event;

    void BrushTool(SDL_Event event);
    void SelectTool(SDL_Event event);

public:
    float brush_size = 1;
    SDL_Color brush_color = {255, 255, 255, 255};

    ToolManager(Canvas *canvas);
    ToolManager();
    ~ToolManager();
    void SetTool(ToolType toolType);
    bool *GetToolboxStates();
    void ExecuteTool(SDL_Event event);
    void InsertImage(std::string image_path);
};
#endif