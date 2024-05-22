#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include "vectors.hpp"
#include <vector>
#include "spline.hpp"
#include "image.hpp"

class Spline;
class Image;
class Canvas
{
public:
    Vector2 offset;
    float scale = 1;
    SDL_Renderer *renderer;

    // objects
    std::vector<Spline> splines;
    std::vector<Image> images;

    bool render_select_box = false;
    Vector2 selectbox_origin;
    Vector2 selectbox_destination;

    Canvas(SDL_Renderer *renderer);
    Canvas();
    ~Canvas();
    
    /**
     * Translates world space coordinate to screen space
     * @param world coordinate in world space 
     */
    Vector2 WorldToScreen(Vector2 world);

    /**
     * Translates screen space coordinate to world space
     * @param screen coordinate in screen space 
     */
    Vector2 ScreenToWorld(Vector2 screen);

    SDL_FRect ScreenRectToWorldRect(SDL_FRect screen);
    SDL_FRect WorldRectToScreenRect(SDL_FRect world);

     /**
     * Renderes all objects on canvas to screen
     */
    void Render();

    void PerformSelection();

};

#endif