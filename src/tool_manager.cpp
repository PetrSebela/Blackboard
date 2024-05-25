#include "tool_manager.hpp"
#include "canvas.hpp"
#include "input_manager.hpp"
#include <SDL2/SDL_image.h>
#include "image.hpp"
#include <boost/algorithm/string.hpp>
#include "utils.hpp"

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
    this->mouse_screen_position = Vector2(this->mouse_x, this->mouse_y);
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
        void *memory = malloc(sizeof(Spline));
        Spline *s = new(memory)Spline;
        s->color = brush_color;
        s->thickness = brush_size;

        CanvasObjectWrapper wrapper(s);
        this->canvas->canvas_objects.push_back(wrapper);
    }
    else if (key_event == KeyEvent::RELEASED && canvas->canvas_objects.size() != 0 && canvas->canvas_objects.back().GetContainedType() == CanvasObjectType::SPLINE)
    {
        Spline *s = canvas->canvas_objects.back().Spline();
        s->finished = true;
    }

    else if (key_event == KeyEvent::DOWN && this->canvas->canvas_objects.size() != 0 && !canvas->canvas_objects.back().Spline()->finished)
    {
        Spline *s = canvas->canvas_objects.back().Spline();
        s->AddPoint(this->canvas->ScreenToWorld(mouse_screen_position));
    }
};

// --- Select tool ---

void ToolManager::SelectTool(SDL_Event event)
{   
    Vector2 world_position = canvas->ScreenToWorld(mouse_screen_position);

    if (canvas->IsOverObject(world_position))
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    else
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

    if (key_event == KeyEvent::RELEASED)
        canvas->render_select_box = false;

    else if (key_event == KeyEvent::PRESSED)
    {
        canvas->ResetSelection();

        this->selectbox_origin = canvas->ScreenToWorld(mouse_screen_position);
        this->selectbox_destination = canvas->ScreenToWorld(mouse_screen_position);
        
        canvas->render_select_box = true;
        
        SDL_FRect rect = GetNormalRect(this->selectbox_origin, this->selectbox_destination);
        canvas->AddToSelection(rect);

    }

    else if (key_event == KeyEvent::DOWN)
    {       
        canvas->ResetSelection();
        this->selectbox_destination = canvas->ScreenToWorld(mouse_screen_position);
        SDL_FRect rect = GetNormalRect(this->selectbox_origin, this->selectbox_destination);
        canvas->AddToSelection(rect);
    }
}

void ToolManager::InsertImage(std::string image_path)
{
    boost::trim(image_path);
    fprintf(stdout, "Loading image : %s\n", image_path.c_str());
    SDL_Surface *image_surface = IMG_Load(image_path.c_str());
    
    if (!image_surface)
    {
        fprintf(stderr, "Error occured while loading a image\n");
        return;
    }

    void *memory = malloc(sizeof(Image));
    Image *image = new(memory)Image(image_surface, canvas);

    CanvasObjectWrapper wrapper(image);
    this->canvas->canvas_objects.push_back(wrapper);
}
    