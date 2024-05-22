#include "tool_manager.hpp"
#include "canvas.hpp"
#include "input_manager.hpp"
#include <SDL2/SDL_image.h>
#include "image.hpp"
#include <boost/algorithm/string.hpp>

ToolManager::ToolManager(Canvas *canvas)
{
    this->SetTool(this->default_tool);
    this->canvas = canvas;
}

ToolManager::ToolManager() {}
ToolManager::~ToolManager() {}

void ToolManager::SetTool(ToolType tool_type)
{
    for (size_t i = 0; i < ToolType::TOOL_COUNT; i++)
        this->toolbox_states[i] = false;
    this->toolbox_states[tool_type] = true;
    this->active_tool = tool_type;
}

bool *ToolManager::GetToolboxStates()
{
    return this->toolbox_states;
}

void ToolManager::ExecuteTool(SDL_Event event)
{
    // rework input management, the way sdl hndeles it is terrible
    if (event.type != SDL_MOUSEMOTION && event.type != SDL_MOUSEBUTTONDOWN && event.type != SDL_MOUSEBUTTONUP)
        return;      

    int keys = SDL_GetMouseState(&this->mouse_x, &this->mouse_y);
    this->mouse_position = Vector2(this->mouse_x, this->mouse_y);
    this->key_event = GetMouseButtonEvent(0, last_mouse_buttons, keys);
    this->last_mouse_buttons = keys;

    switch (this->active_tool)
    {
    case ToolType::Brush:
        return this->BrushTool(event);
    case ToolType::Select:
        return this->SelectTool(event);
    default:
        return;
    }
};

// --- Brush tool ---

void ToolManager::BrushTool(SDL_Event event)
{
    if (key_event == KeyEvent::PRESSED)
    {
        Spline s;
        s.color = brush_color;
        s.thickness = brush_size;
        this->canvas->splines.push_back(s);
    }

    else if (key_event == KeyEvent::RELEASED && canvas->splines.size() != 0)
        this->canvas->splines.back().finished = true;

    else if (key_event == KeyEvent::DOWN && this->canvas->splines.size() != 0 && !this->canvas->splines.back().finished)
        this->canvas->splines.back().AddPoint(this->canvas->ScreenToWorld(mouse_position));
};

// --- Select tool ---

void ToolManager::SelectTool(SDL_Event event)
{   
    if (key_event == KeyEvent::RELEASED)
        canvas->render_select_box = false;

    else if (key_event == KeyEvent::PRESSED)
    {
        canvas->selectbox_origin = canvas->ScreenToWorld(mouse_position);
        canvas->selectbox_destination = canvas->ScreenToWorld(mouse_position);
        canvas->render_select_box = true;
        canvas->PerformSelection();
    }

    else if (key_event == KeyEvent::DOWN)
    {
        canvas->selectbox_destination = canvas->ScreenToWorld(mouse_position);
        canvas->PerformSelection();
    }
}

void ToolManager::InsertImage(std::string image_path)
{
    boost::trim(image_path);
    fprintf(stdout, "Loading image : %s\n", image_path.c_str());
    SDL_Surface *image = IMG_Load(image_path.c_str());
    
    if (!image)
    {
        fprintf(stderr, "Error occured while loading a image\n");
        return;
    }  

    Image canvas_image(image, this->canvas);
    this->canvas->images.push_back(canvas_image);
}
    