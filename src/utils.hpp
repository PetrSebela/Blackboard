#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include "vectors.hpp"

/**
 * Clamps RGBA value between 0 - 1 
 * @param RGBA source array of 4 floats 
 * @param output pointer to array of 4 float representing RGBA 
 */
void DenormalizeRGBA(float *RGBA, int *output);

/**
 * Converts 0 - 255 values in array to SDL color 
 * @param RGBA source array of 4 int in range from 0 to 255 
 */
SDL_Color ArrayToColor(int *RGBA);


SDL_FRect GetNormalRect(Vector2 origin, Vector2 destination);

bool RectIntersection(SDL_FRect a, SDL_FRect b);
bool PointInRect(SDL_FRect container, Vector2 point);
#endif