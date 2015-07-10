#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include <vector>
#include <map>

#include "Entity.h"

class Game {
public:
    enum Resource
    {
        Player,
        GrassTile,
        WallTile
    };

    Game(HWND hwnd, HDC hdc);
    ~Game() {};

    void Initialize();
    void Update(const double deltaTime);
    void Render(const double interpolation) const;
    void End();

    std::vector<Entity*> entities;
    std::map<Game::Resource, HBITMAP> bitmaps;
    HWND hwnd;
    HDC hdc;
private:
    HBITMAP LoadBitmap(Game::Resource resource, wchar_t** filename);
};

#endif