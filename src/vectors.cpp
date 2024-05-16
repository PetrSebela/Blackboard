#include "vectors.hpp"
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>


Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2() {}

Vector2::~Vector2() {}

float Vector2::magnitude()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

void Vector2::normalize(){
    float magnitude = this->magnitude();
    this->x /= magnitude;
    this->y /= magnitude;
}

Vector2 Vector2::normalized(){
    float magnitude = this->magnitude();
    return Vector2(this->x / magnitude, this->y /= magnitude);
}


Vector2 Vector2::getNormal()
{
    Vector2 normalized = this->normalized();
    return Vector2(normalized.y, -normalized.x);
}

void Vector2::printVector()
{
    fprintf(stdout, "Vector2 : %f, %f\n", this->x, this->y);
}

SDL_FPoint Vector2::ToSDLFpoint(){
    SDL_FPoint point{
        this->x, 
        this->y
    };
    return point;
}


Vector2 Vector2::Reversed(){
    return Vector2(-this->x, - this->y);
}



bool Vector2::operator!=(const Vector2 &other) { return this->x != other.x || this->y != other.y; }

void Vector2::operator+=(const Vector2 &other)
{
    this->x += other.x;
    this->y += other.y;
}

Vector2 Vector2::operator+(const Vector2 &other)
{
    return Vector2(this->x + other.x, this->y + other.y);
}

void Vector2::operator-=(const Vector2 &other)
{
    this->x -= other.x;
    this->y -= other.y;
}

Vector2 Vector2::operator-(const Vector2 &other)
{
    return Vector2(this->x - other.x, this->y - other.y);
}

void Vector2::operator*=(const float &other)
{
    this->x *= other;
    this->y *= other;
}

Vector2 Vector2::operator*(const float &other)
{
    return Vector2(this->x * other, this->y * other);
}

void Vector2::operator/=(const float &other)
{
    this->x /= other;
    this->y /= other;
}

Vector2 Vector2::operator/(const float &other)
{
    return Vector2(this->x / other, this->y / other);
}

Vector2 Vector2::GetZeroVector() { return Vector2(); }