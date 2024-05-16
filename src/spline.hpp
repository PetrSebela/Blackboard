#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <SDL2/SDL.h>
#include "vectors.hpp"
#include "canvas.hpp"

/**
 * Class defining spline object
 */
class Spline
{
public:
    // current brush color
    int color[4] = {255, 255, 255, 255};
    float thickness = 1;

    // list of all sline points
    std::vector<Vector2> points;

    /**
     * Sets color of current spline
     * @param colors_rgba expects array of 4 floats, throws segfault if smalled size is passed 
     */
    void setColor(int *colors_rgba);

    /**
     * Adds point to spline
     * @param point point to be added to spline 
     */
    void addPoint(Vector2 point);

    /**
     * Renders spline to the screen
     * @param renderer pointer to SDL2 renderer 
     * @param offset canvas origin scale 
     * @param scale canvas scale (zoom) 
     */
    void render(Canvas *canvas);
};

#endif
