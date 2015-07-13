#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"

class Level : public Entity
{
public:
    Level();

    void Update(const double deltaTime);
    void Render(HDC hdcWindow, const BITMAP& bm, HDC hdcImage, const double interpolation) const;
};
#endif