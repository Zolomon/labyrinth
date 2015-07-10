#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"

class Level : public Entity
{
public:
    Level();

    void Render(const double interpolation) const;
    void Update(const double deltaTime);
};
#endif