#include "canvas.hpp"
#include "vectors.hpp"
#include "utils.hpp"

Canvas::Canvas(SDL_Renderer *renderer)
{
    this->renderer = renderer;
};

Canvas::Canvas(){};
Canvas::~Canvas(){};

Vector2 Canvas::WorldToScreen(Vector2 world)
{
    return (world - this->offset) * this->scale;
}
Vector2 Canvas::ScreenToWorld(Vector2 screen)
{
    return (screen / this->scale) + this->offset;
}

void Canvas::Render()
{
    for (CanvasObjectWrapper wrapper : this->canvas_objects)
    {
        if (wrapper.IsSelected())
            wrapper.RenderSelection(this);
        wrapper.Render(this);
    }

    if (this->render_select_box)
    {
        SDL_FRect screen_rect = WorldRectToScreenRect(this->selection_box);
        SDL_SetRenderDrawColor(this->renderer, 0, 155, 255, 5);
        SDL_RenderFillRectF(renderer, &screen_rect);
        SDL_SetRenderDrawColor(this->renderer, 0, 155, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRectF(renderer, &screen_rect);
    }
}

SDL_FRect Canvas::ScreenRectToWorldRect(SDL_FRect screen)
{
    Vector2 origin = ScreenToWorld(Vector2(screen.x, screen.y));
    Vector2 destination = ScreenToWorld(Vector2(screen.x + screen.w, screen.y + screen.h));

    return {
        origin.x,
        origin.y,
        destination.x - origin.x,
        destination.y - origin.y};
}

SDL_FRect Canvas::WorldRectToScreenRect(SDL_FRect world)
{
    Vector2 origin = WorldToScreen(Vector2(world.x, world.y));
    Vector2 destination = WorldToScreen(Vector2(world.x + world.w, world.y + world.h));

    return {
        origin.x,
        origin.y,
        destination.x - origin.x,
        destination.y - origin.y};
}

// --- Selection ---

void Canvas::AddToSelection(SDL_FRect select_area)
{
    this->selection_box = select_area;

    for (CanvasObjectWrapper wrapper : this->canvas_objects)
    {
        bool intersects = wrapper.IntersectsRect(selection_box);
        if (intersects)
            wrapper.SetSelected(true);
    }
}

void Canvas::ResetSelection()
{
    for (CanvasObjectWrapper wrapper : this->canvas_objects)
        wrapper.SetSelected(false);
}

bool Canvas::IsOverObject(Vector2 point)
{
    for (CanvasObjectWrapper wrapper : this->canvas_objects)
        if (PointInRect(wrapper.GetBoundingBox(), point))
            return true;
    return false;
}
