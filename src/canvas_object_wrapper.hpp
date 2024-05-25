#ifndef CANVAS_OBJECT_WRAPPER_
#define CANVAS_OBJECT_WRAPPER_
#include "spline.hpp"
#include "image.hpp"
#include "canvas.hpp"
#include "canvas_object_type.hpp"

class Canvas;
class Image;
class Spline;
class CanvasObjectWrapper
{
private:
    Image *image;
    Spline *spline;
    CanvasObjectType contained_type;

public:
    CanvasObjectWrapper(Image *image);
    CanvasObjectWrapper(Spline *spline);
    CanvasObjectWrapper();
    ~CanvasObjectWrapper();


    Image *Image();
    Spline *Spline();

    CanvasObjectType GetContainedType();
    void Render(Canvas *canvas);
    void RenderSelection(Canvas *canvas);
    SDL_FRect GetBoundingBox();
    void SetSelected(bool selected);
    bool IsSelected();
    bool IntersectsRect(SDL_FRect rect);
};

#endif