#ifndef TOOL_MANAGER_H_
#define TOOL_MANAGER_H_

#include "tool_types.hpp"
#include <SDL2/SDL.h>
#include "canvas.hpp"

class ToolManager
{
private:
    ToolType default_tool = ToolType::Brush;
    bool toolbox_states[ToolType::TOOL_COUNT] = {false};
    ToolType active_tool;
    Canvas *canvas;

    void BrushTool(SDL_Event event);
    bool CannotDraw(int mouse_buttons);

public:
    ToolManager(Canvas *canvas);
    ToolManager();
    ~ToolManager();
    void SetTool(ToolType toolType);
    bool *GetToolboxStates();
    void ExecuteTool(SDL_Event event);
};
#endif