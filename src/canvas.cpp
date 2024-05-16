#include "canvas.hpp"
#include "vectors.hpp"

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
