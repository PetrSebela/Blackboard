#include <algorithm>
#include "utils.hpp"
#include <vector>

void DenormalizeRGBA(float *RGBA, int *output)
{
    for (size_t i = 0; i < 4; i++)
        output[i] = std::clamp((int)(RGBA[i] * 255), 0, 255);
}

SDL_Color ArrayToColour(int *RGBA)
{
    SDL_Color color{
        (Uint8)RGBA[0],
        (Uint8)RGBA[1],
        (Uint8)RGBA[2],
        (Uint8)RGBA[3]
    };
    return color; 
}