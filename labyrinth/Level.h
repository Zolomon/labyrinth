#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <memory>

#include "Entity.h"
#include "Resource.h"
#include "WindowOptions.h"

class Game;

class Level : public Entity
{
public:
    Level(std::vector<Resource> tiles, int width, int height);

    void Update(const double deltaTime);
    
    std::vector<Resource> tiles;

    const int TILES_PER_COLUMN;
    const int TILES_PER_ROW;
};
#endif