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
#include "WindowOptions.h"

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
    HBITMAP bufferBitmap;
    HDC bufferHdc;
    RECT winRect;
    HGDIOBJ oldGdiObj;
private:
    void SetupGDI();
    void TeardownGDI();
    HBITMAP LoadBitmap(Resource resource, std::wstring filename);
    HBITMAP FindBitmap(Resource resource) const;
    void RenderEntity(const std::shared_ptr<Entity>& entity, const double interpolation) const;
    bool HasLoaded;
};

#endif