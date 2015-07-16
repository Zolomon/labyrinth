#include "Utils.h"


std::shared_ptr<Level> Utils::loadLevel(std::string filename) {
    std::ifstream fileStream(filename);
    std::string line;
    int cols = 0;
    int rows = 0;
    std::vector<Resource> tiles;
    while (std::getline(fileStream, line)) {
        // 
        for (auto x : line) {
            if (x == '0') {
                tiles.push_back(Resource::GrassTile);
                continue;
            }
            if (x == '1') {
                tiles.push_back(Resource::WallTile);
                continue;
            }
            if (x == 's') {
                tiles.push_back(Resource::StartTile);
                continue;
            }
            if (x == 'e') {
                tiles.push_back(Resource::EndTile);
            }
        }

        cols = line.length();
        rows++;
    }
    std::shared_ptr<Level> level = std::make_shared<Level>(tiles, cols, rows);
    
    return  level;
}