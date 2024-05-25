#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include "vectors.hpp"
#include <vector>
#include "spline.hpp"
#include "image.hpp"
#include "canvas_object_wrapper.hpp"

class Spline;
class Image;
class CanvasObjectWrapper;
class Canvas
{
private:
    SDL_FRect selection_box;

public:
    Vector2 offset;
    float scale = 1;
    SDL_Renderer *renderer;

    std::vector<CanvasObjectWrapper> canvas_objects;

    bool render_select_box = false;
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

    // Converts rectangle from screen to world coordinates
    SDL_FRect ScreenRectToWorldRect(SDL_FRect screen);
    
    // Converts rectangle from world to screen coordinates
    SDL_FRect WorldRectToScreenRect(SDL_FRect world);

    // Renders all objects from `canvas_objects`
    void Render();

    // Performs selection on `canvas_objects` and sets appropriate flags
    
    void AddToSelection(SDL_FRect select_area);
    
    void ResetSelection();

    bool IsOverObject(Vector2 point);    
};

#endif