#include "tool_manager.hpp"
#include "canvas.hpp"

ToolManager::ToolManager(Canvas *canvas)
{
    this->SetTool(this->default_tool);
    this->canvas = canvas;
}

ToolManager::ToolManager(){}
ToolManager::~ToolManager(){}

void ToolManager::SetTool(ToolType tool_type)
{
    for (size_t i = 0; i < ToolType::TOOL_COUNT; i++)
        this->toolbox_states[i] = false;
    this->toolbox_states[tool_type] = true;
    this->active_tool = tool_type;
}

bool* ToolManager::GetToolboxStates()
{
    return this->toolbox_states;
}

void ToolManager::ExecuteTool(SDL_Event event)
{
    switch (this->active_tool)
    {
    case ToolType::Brush:
        this->BrushTool(event);
        break;
    
    default:
        break;
    }
};


// --- Brush tool ---

void ToolManager::BrushTool(SDL_Event event)
{
    // get mouse position
    int mouse_x, mouse_y;
    int mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    Vector2 mouse_position(mouse_x, mouse_y);
    
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (mouse_buttons != 1)
            break;        
        printf("Starting new spline\n");        
        this->canvas->splines.push_back(Spline());
        break;

    case SDL_MOUSEBUTTONUP:
        if (this->canvas->splines.size() == 0)
            break;
        printf("Ending spline\n");        
        this->canvas->splines.back().finished = true;
        break;
              
    case SDL_MOUSEMOTION:
        if (this->CannotDraw(mouse_buttons))
            break;
        this->canvas->splines.back().AddPoint(this->canvas->ScreenToWorld(mouse_position));
        break;
    
    default:
        break;
    }
};

bool ToolManager::CannotDraw(int mouse_buttons)
{
    return  mouse_buttons != 1 || 
            this->canvas->splines.size() == 0 || 
            this->canvas->splines.back().finished;
}