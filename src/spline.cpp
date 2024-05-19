#include "spline.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include "canvas.hpp"
#include "utils.hpp"
#include "debug.hpp"

void Spline::AddPoint(Vector2 point)
{
    if (this->points.size() != 0 && this->points[this->points.size() - 1] == point)
        return;

    this->points.push_back(point);
    if (this->points.size() == 1)
    {
        this->bb_origin = point;
        this->bb_destination = point;
        return;
    }

    // doesnt account for line thickness, but should be solvable with some vector math
    if (point.x < this->bb_origin.x)
        bb_origin.x = point.x;

    if (point.y > this->bb_origin.y)
        bb_origin.y = point.y;

    if (point.x > this->bb_destination.x)
        bb_destination.x = point.x;

    if (point.y < this->bb_destination.y)
        bb_destination.y = point.y;
}

void Spline::Render(Canvas *canvas)
{
    int point_count = this->points.size();
    SDL_FPoint points[point_count];

    int screen_size_x = 0;
    int screen_size_y = 0;
    SDL_GetRendererOutputSize(canvas->renderer, &screen_size_x, &screen_size_y);

    for (int i = 0; i < point_count; i++)
    {
        Vector2 screen_point = canvas->WorldToScreen(this->points[i]);
        points[i] = {screen_point.x, screen_point.y}; // SDL_Point and Vector2 are literarly the same thing
    }

    // skip zero length lines
    if (point_count <= 1)
        return;

    // IMPORTANT : construction of line mesh is buggy AF, rewrite it in the future (or leave it as is and never touch it again)
    // i will probably rewrite it in the future to handle sharp corners and smoothing in zoom ( probably shaders )

    Vector2 origin = this->points[0];
    Vector2 next = this->points[1];
    SDL_Color line_color = this->color;
    float thickness = this->thickness;
    std::vector<int> triangles;
    int vertex_count = 4;

    std::vector<SDL_Vertex> vertices = {
        {
            canvas->WorldToScreen(origin + (next - origin).getNormal().Reversed() * (thickness / 2)).ToSDLFpoint(),
            line_color,
            SDL_FPoint{0},
        },
        {
            canvas->WorldToScreen(origin + (next - origin).getNormal() * (thickness / 2)).ToSDLFpoint(),
            line_color,
            SDL_FPoint{0},
        },
        {
            canvas->WorldToScreen(next + (next - origin).getNormal() * (thickness / 2)).ToSDLFpoint(),
            line_color,
            SDL_FPoint{0},
        },
        {
            canvas->WorldToScreen(next + (next - origin).getNormal().Reversed() * (thickness / 2)).ToSDLFpoint(),
            line_color,
            SDL_FPoint{0},
        }};

    int vertex_offsets[6] = {0, 2, 3, 0, 3, 1};
    for (size_t i = 0; i < 6; i++)
        triangles.push_back(vertex_count - vertex_offsets[i]);

    for (int i = 1; i <= point_count; i++)
    {
        Vector2 origin = this->points[i];
        Vector2 past = this->points[i - 1];
        vertices.push_back({canvas->WorldToScreen(origin + (past - origin).getNormal() * (thickness / 2)).ToSDLFpoint(), line_color, SDL_FPoint{0}});
        vertices.push_back({canvas->WorldToScreen(origin + (past - origin).getNormal().Reversed() * (thickness / 2)).ToSDLFpoint(), line_color, SDL_FPoint{0}});

        for (int offset : vertex_offsets)
            triangles.push_back(vertex_count - offset - 1);
        vertex_count += 2;
    }


    // debug render of bounding box
    Vector2 bb_origin = canvas->WorldToScreen(this->bb_origin);
    Vector2 bb_destination = canvas->WorldToScreen(this->bb_destination);
    SDL_FRect bounding_box = GetNormalRect(bb_origin, bb_destination);

    if (DRAW_DEBUG)
    {
        if (this->selected)
        {
            SDL_SetRenderDrawColor(canvas->renderer, 10, 255, 10, 5);
            SDL_RenderFillRectF(canvas->renderer, &bounding_box);
            SDL_SetRenderDrawColor(canvas->renderer, 10, 255, 10, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRectF(canvas->renderer, &bounding_box);
        }
        else
        {
            SDL_SetRenderDrawColor(canvas->renderer, 255, 10, 10, 5);
            SDL_RenderFillRectF(canvas->renderer, &bounding_box);
            SDL_SetRenderDrawColor(canvas->renderer, 255, 10, 10, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRectF(canvas->renderer, &bounding_box);
        }
    }

    SDL_RenderGeometry(canvas->renderer, nullptr, vertices.data(), vertices.size(), triangles.data(), triangles.size());
}

void Spline::SetColor(int *colors_rgba)
{
    SDL_Color color = {
        (Uint8)std::clamp((int)(colors_rgba[0]), 0, 255),
        (Uint8)std::clamp((int)(colors_rgba[1]), 0, 255),
        (Uint8)std::clamp((int)(colors_rgba[2]), 0, 255),
        (Uint8)std::clamp((int)(colors_rgba[3]), 0, 255),
    };
    this->color = color;
}