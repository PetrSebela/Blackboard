#ifndef CANVAS_OBJECT_WRAPPER_
#define CANVAS_OBJECT_WRAPPER_
#include "spline.hpp"
#include "image.hpp"
#include "canvas.hpp"
#include "canvas_object_type.hpp"

class Canvas;
class CanvasObjectWrapper
{
private:
    Image image;
    Spline spline;
    CanvasObjectType contained_type;

public:
    CanvasObjectWrapper(Image image);
    CanvasObjectWrapper(Spline Spline);
    CanvasObjectWrapper();
    ~CanvasObjectWrapper();

    CanvasObjectType GetContainedType();
    void Render(Canvas *canvas);
    SDL_FRect GetBoundingBox();
    void SetSelected(bool selected);
    bool IsSelected();
};

#endif