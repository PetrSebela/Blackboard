#include "canvas_object_wrapper.hpp"
#include "utils.hpp"

CanvasObjectWrapper::CanvasObjectWrapper(){};
CanvasObjectWrapper::~CanvasObjectWrapper(){};

CanvasObjectWrapper::CanvasObjectWrapper(Image image)
{
    this->image = image;
    this->contained_type = CanvasObjectType::IMAGE;
};

CanvasObjectWrapper::CanvasObjectWrapper(Spline spline)
{
    this->spline = spline;
    this->contained_type = CanvasObjectType::SPLINE;
};

CanvasObjectType CanvasObjectWrapper::GetContainedType()
{
    return this->contained_type;
}

void CanvasObjectWrapper::Render(Canvas *canvas)
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        return this->image.Render(canvas);
    case CanvasObjectType::SPLINE:
        return this->spline.Render(canvas);
    default:
        return;
    }
}

SDL_FRect CanvasObjectWrapper::GetBoundingBox()
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        return this->image.destination;
    case CanvasObjectType::SPLINE:
        return GetNormalRect(spline.bb_origin, spline.bb_destination);
    default:
        return {};
    }
}

void CanvasObjectWrapper::SetSelected(bool selected)
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        this->image.selected = selected;
        break;
    case CanvasObjectType::SPLINE:
        this->spline.selected = selected;
        break;
    default:
        return;
    }
}

bool CanvasObjectWrapper::IsSelected()
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        return this->image.selected;
    case CanvasObjectType::SPLINE:
        return this->spline.selected;
    default:
        return false;
    }
}