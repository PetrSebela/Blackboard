#ifndef IMAGE_H_
#define IMAGE_H_

#include <SDL2/SDL.h>
#include "canvas.hpp"

class Canvas;
class Image
{
private:
    SDL_Texture *texture;
    SDL_Surface *image;

public:
    SDL_FRect destination;
    bool selected = false;
    Image(SDL_Surface *image, Canvas *canvas);
    Image();
    ~Image();
    void Render(Canvas *canvas);
    void Free();
};

#endif