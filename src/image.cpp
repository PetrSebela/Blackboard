#include "image.hpp"
#include "debug.hpp"

Image::Image(SDL_Surface *image, Canvas *canvas)
{
    this->image = image;
    this->texture = SDL_CreateTextureFromSurface(canvas->renderer, image);

    int w, h;
    SDL_GetRendererOutputSize(canvas->renderer, &w, &h);
    float scale_factor = (float)(w / 2) / image->w;

    int scaled_width = ((float)image->w * scale_factor);
    int scaled_height = ((float)image->h * scale_factor);
    int origin_x = w / 2 - scaled_width / 2;
    int origin_y = h / 2 - scaled_height / 2;
    
    SDL_FRect destination = {
        (float)origin_x,
        (float)origin_y,
        (float)scaled_width,
        (float)scaled_height
    };

    fprintf(stdout, "Original:(%d %d) | Scaled: (%d %d) | Factor: %f\n", image->w, image->h, scaled_width, scaled_height, scale_factor);

    this->destination = canvas->ScreenRectToWorldRect(destination);
}

Image::Image(){}
Image::~Image(){}

void Image::Render(Canvas *canvas)
{
    SDL_FRect screen_rect = canvas->WorldRectToScreenRect(this->destination);
 
    if (selected && DRAW_DEBUG)
    {
        SDL_FRect debug_rect = {
            screen_rect.x - 1,
            screen_rect.y - 1,
            screen_rect.w + 2,
            screen_rect.h + 2,
        };
        SDL_SetRenderDrawColor(canvas->renderer, 10, 255, 10, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRectF(canvas->renderer, &debug_rect);
    }
    
    SDL_RenderCopyF(canvas->renderer, this->texture, NULL, &screen_rect);
}

void Image::Free()
{
    SDL_FreeSurface(this->image);
}