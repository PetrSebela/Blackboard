#include <algorithm>
#include "utils.hpp"
#include <vector>

void DenormalizeRGBA(float *RGBA, int *output)
{
    for (size_t i = 0; i < 4; i++)
        output[i] = std::clamp((int)(RGBA[i] * 255), 0, 255);
}

SDL_Color ArrayToColor(int *RGBA)
{
    SDL_Color color{
        (Uint8)RGBA[0],
        (Uint8)RGBA[1],
        (Uint8)RGBA[2],
        (Uint8)RGBA[3]};
    return color;
}

SDL_FRect GetNormalRect(Vector2 origin, Vector2 destination)
{
    float ox = origin.x < destination.x ? origin.x : destination.x;
    float oy = origin.y < destination.y ? origin.y : destination.y;
    float dx = origin.x > destination.x ? origin.x : destination.x;
    float dy = origin.y > destination.y ? origin.y : destination.y;

    return {ox, oy, dx - ox, dy - oy};
}

bool RectIntersection(SDL_FRect a, SDL_FRect b)
{
    return !(a.x > b.x + b.w ||
             a.x + a.w < b.x ||
             a.y > b.y + b.h ||
             a.y + a.h < b.y);
};

bool PointInRect(SDL_FRect container, Vector2 point)
{
    return point.x >= container.x &&
           point.x <= container.x + container.w &&
           point.y >= container.y &&
           point.y <= container.y + container.h;
}