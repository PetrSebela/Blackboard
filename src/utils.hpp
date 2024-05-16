#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

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
SDL_Color ArrayToColour(int *RGBA);

#endif