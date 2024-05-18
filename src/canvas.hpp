#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include "vectors.hpp"
#include <vector>
#include "spline.hpp"

class Spline;
class Canvas
{
public:
    Vector2 offset;
    float scale = 1;
    SDL_Renderer *renderer;

    std::vector<Spline> splines;

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

     /**
     * Renderes all objects on canvas to screen
     */
    void Render();
};

#endif