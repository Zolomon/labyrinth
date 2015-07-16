#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <tchar.h>
#include <cassert>
#include <sstream>
#include <vector>
#include <memory>
#include <map>


#include "Resource.h"
#include "Entity.h"
#include "Player.h"
#include "WindowOptions.h"
#include "Bitmap.h"
#include "Level.h"

class Game {
public:
    //Game(HWND hwnd, HDC hdc);
    Game() {};
    ~Game() {};

    void InitializeGraphics(HWND window);
    bool LoadBitmapFromFile(const std::wstring filename, Bitmap& bitmap, Resource resource);
    void Render(const double interpolation);
    void BeginGraphics();
    void DrawBitmap(Bitmap bitmap, int x, int y);
    void DrawString(const std::wstring text, int x, int y);
    void EndGraphics();
    void FreeBitmap(Bitmap bitmap);
    void ShutdownGraphics();

    void Restart();
        
    void Update(const double deltaTime);

    void AddLevel(std::shared_ptr<Level> level);

    std::vector<std::shared_ptr<Level>> levels;
    std::vector<std::shared_ptr<Entity>> entities;
    std::map<Resource, Bitmap> bitmapDictionary;
    std::map<Resource, std::wstring> fileDictionary;

    std::shared_ptr<Level> currentLevel;

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

    std::shared_ptr<Player> player;
    Bitmap playerBitmap;
    int score;

 
private:
    void DrawLevel();
};

#endif