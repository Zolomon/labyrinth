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

    //virtual void Update(const double deltaTime) = 0;
    //virtual void Render(const double interpolation) = 0;
    
    double x;
    double y;
    
    Resource resource;
    const bool IsDrawable;
};
#endif