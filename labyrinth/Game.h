#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <tchar.h>
#include <cassert>
#include <vector>
#include <memory>
#include <map>

#include "Resource.h"
#include "Entity.h"
#include "Player.h"

class Game {
public:
    Game(HWND hwnd, HDC hdc);
    ~Game() {};

    void Initialize();
    void LoadResources();
    void Update(const double deltaTime);
    void Render(const double interpolation) const;
    void UnloadResources();
    void End();

    std::vector<std::shared_ptr<Entity>> entities;
    std::map<Resource, HBITMAP> bitmaps;
    std::map<Resource, std::wstring> resourceFiles;
    HWND hwnd;
    HDC hdc;
private:
    HBITMAP LoadBitmap(Resource resource, std::wstring filename);
    bool HasLoaded;
};

#endif