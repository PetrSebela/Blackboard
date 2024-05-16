#ifndef VECTORS_H
#define VECTORS_H

#include <SDL2/SDL.h>


class Vector2
{
public:
    float x = 0;
    float y = 0;

    Vector2();

    ~Vector2();
    
    static Vector2 GetZeroVector();

    float magnitude();

    void normalize();

    Vector2 normalized();

    Vector2 getNormal();

    void printVector();

    Vector2(float x, float y);

    SDL_FPoint ToSDLFpoint();

    Vector2 Reversed();
    
    bool operator != (const Vector2 &other);

    void operator += (const Vector2 &other);
    Vector2 operator + (const Vector2 &other);

    void operator -= (const Vector2 &other);
    Vector2 operator - (const Vector2 &other);

    void operator *= (const float &other);
    Vector2 operator * (const float &other);

    void operator /= (const float &other);
    Vector2 operator / (const float &other);
};

#endif