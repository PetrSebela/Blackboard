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
    for (Spline s : this->splines)
        s.Render(this);


    if (this->render_select_box)
    {
        Vector2 origin = WorldToScreen(this->selectbox_origin);
        Vector2 destination = WorldToScreen(this->selectbox_destination);
        
        SDL_FRect screen_select_box = {
            origin.x,
            origin.y,
            destination.x - origin.x,
            destination.y - origin.y
        };
    
        SDL_SetRenderDrawColor(this->renderer, 0, 255, 255, 2);
        SDL_RenderFillRectF(renderer, &screen_select_box);
        SDL_SetRenderDrawColor(this->renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRectF(renderer, &screen_select_box);
    }
}

void Canvas::PerformSelection()
{
    SDL_FRect selectbox = GetNormalRect(this->selectbox_origin, this->selectbox_destination);
    for (int i = 0; i < this->splines.size(); i++)
    {
        SDL_FRect tested = GetNormalRect(this->splines[i].bb_origin, this->splines[i].bb_destination);
        this->splines[i].selected = RectContains(selectbox, tested);
    }      
}