#ifndef ENTITY_H
#define ENTITY_H

#include <Windows.h>
#include "Resource.h"
#include "Bitmap.h"

class Entity
{
public:
    Entity(bool isDrawable, double x, double y);
    virtual ~Entity();

    virtual void Update(const double deltaTime) = 0;
    //virtual void Render(HDC hdcWindow, const BITMAP& bm, HDC hdcImage, const double interpolation) const = 0;
    //virtual void Render(const Game& game, const double interpolation) const = 0;
    

    double x;
    double y;

    double vx;
    double vy;

    Resource resource;

    const bool IsDrawable;
};
#endif