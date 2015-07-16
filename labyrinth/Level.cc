#include "Level.h"

Level::Level(std::vector<Resource> tiles, int tilesPerCol, int tilesPerRow) : Entity(true, 0, 0), tiles(tiles), TILES_PER_COLUMN(tilesPerCol), TILES_PER_ROW(tilesPerRow) {
}

void Level::Update(const double deltaTime) {

}