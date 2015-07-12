#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"

class Level : public Entity
{
public:
    Level();

    void Update(const double deltaTime);
    void Render(HWND hwnd, HDC hdc, const double interpolation) const;
};
#endif