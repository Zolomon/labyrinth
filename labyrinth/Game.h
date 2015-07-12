#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <memory>
#include <map>

#include "Entity.h"
#include "Resource.h"
#include "Player.h"

class Game {
public:
    Game(HWND hwnd, HDC hdc);
    ~Game() {};

    void Initialize();
    void Update(const double deltaTime);
    void Render(const double interpolation) const;
    void End();

    std::vector<std::shared_ptr<Entity>> entities;
    std::map<Resource, HBITMAP> bitmaps;
    HWND hwnd;
    HDC hdc;
private:
    HBITMAP LoadBitmap(Resource resource, wchar_t** filename);
};

#endif