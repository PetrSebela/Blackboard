#include "canvas_object_wrapper.hpp"
#include "utils.hpp"

CanvasObjectWrapper::CanvasObjectWrapper(){};
CanvasObjectWrapper::~CanvasObjectWrapper(){};

CanvasObjectWrapper::CanvasObjectWrapper(class Image *image)
{
    this->image = image;
    this->contained_type = CanvasObjectType::IMAGE;
};

CanvasObjectWrapper::CanvasObjectWrapper(class Spline *spline)
{
    this->spline = spline;
    this->contained_type = CanvasObjectType::SPLINE;
};

CanvasObjectType CanvasObjectWrapper::GetContainedType()
{
    return this->contained_type;
}

Image *CanvasObjectWrapper::Image()
{
    return this->image;
}

Spline *CanvasObjectWrapper::Spline()
{
    return this->spline;
}

void CanvasObjectWrapper::Render(Canvas *canvas)
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        return this->image->Render(canvas);
    case CanvasObjectType::SPLINE:
        return this->spline->Render(canvas);
    default:
        return;
    }
}

void CanvasObjectWrapper::RenderSelection(Canvas *canvas)
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        return this->image->RenderSelection(canvas);
    case CanvasObjectType::SPLINE:
        return this->spline->RenderSelection(canvas);
    default:
        return;
    }
}

SDL_FRect CanvasObjectWrapper::GetBoundingBox()
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        return this->image->destination;
    case CanvasObjectType::SPLINE:
        return GetNormalRect(spline->bb_origin, spline->bb_destination);
    default:
        return {};
    }
}

void CanvasObjectWrapper::SetSelected(bool selected)
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        this->image->selected = selected;
        break;
    case CanvasObjectType::SPLINE:
        this->spline->selected = selected;
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
        return this->image->selected;
    case CanvasObjectType::SPLINE:
        return this->spline->selected;
    default:
        return false;
    }
}

bool CanvasObjectWrapper::IntersectsRect(SDL_FRect rect)
{
    switch (this->contained_type)
    {
    case CanvasObjectType::IMAGE:
        return this->image->IntersectsRect(rect);
    case CanvasObjectType::SPLINE:
        return this->spline->IntersectsRect(rect);
    default:
        return false;
    }
}

