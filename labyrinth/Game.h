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
#include "Bitmap.h"

class Game {
public:
    //Game(HWND hwnd, HDC hdc);
    Game() {};
    ~Game() {};

    void InitializeGraphics(void* window);
    bool LoadBitmapFromFile(const std::wstring filename, Bitmap& bitmap, Resource resource);
    void Render(const double interpolation);
    void BeginGraphics();
    void DrawBitmap(Bitmap bitmap, int x, int y);
    void DrawString(const std::wstring text, int x, int y);
    void EndGraphics();
    void FreeBitmap(Bitmap bitmap);
    void ShutdownGraphics();
        
    void Update(const double deltaTime);

    std::vector<std::shared_ptr<Entity>> entities;
    std::map<Resource, Bitmap> bitmapDictionary;
    std::map<Resource, std::wstring> fileDictionary;

    HWND window;
    int windowWidth;
    int windowHeight;

    HDC backbufferDC;
    HBITMAP backbufferBitmap;

    HDC bitmapDC;

    struct GDIBitmap
    {
        HBITMAP handle;
        int width;
        int height;

        GDIBitmap()
            : handle(0)
        {
        }
    };

    std::vector<GDIBitmap> bitmaps;

    Bitmap player;

   /* void Initialize();
    void LoadResources();
    void Update(const double deltaTime);
    void Render(const double interpolation) const;
    void Game::DrawBitmap(HBITMAP bitmap, int x, int y, int width, int height);
    void UnloadResources();
    void End();

    
    HWND hwnd;
    HDC hdc;
    HBITMAP bufferBitmap;
    HDC bufferHdc;
    RECT winRect;
    HGDIOBJ oldGdiObj;*/
private:
  /*  void SetupGDI();
    void TeardownGDI();
    HBITMAP LoadBitmap(Resource resource, std::wstring filename);
    void RenderEntity(const std::shared_ptr<Entity>& entity, const double interpolation) const;
    bool HasLoaded;*/
};

#endif