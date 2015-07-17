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


#include "WindowOptions.h"
#include "Resource.h"
#include "Command.h"
#include "Bitmap.h"
#include "Entity.h"
#include "Player.h"
#include "Level.h"

class Game {
public:
    Game() {};
    ~Game() {};

    bool LoadBitmapFromFile(const std::wstring filename, Resource resource);
    void InitializeGraphics(HWND window);    
    void BeginGraphics();
    void DrawBitmap(Bitmap bitmap, int x, int y)  const;
    void DrawString(const std::wstring text, COLORREF color, int x, int y) const;
    void DrawLevel() const;
    void Render(const double interpolation);
    void EndGraphics();
    void FreeBitmap(Bitmap bitmap);
    void ShutdownGraphics();

    void Start();
    void MovePlayer(Command command);
    void ProcessInput(Command command);    
    void CheckWinningCondition();
    void AddLevel(std::shared_ptr<Level> level);

    // GDI-related stuff
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
    // EOF GDI

    std::vector<std::shared_ptr<Level>> levels;
    std::vector<std::shared_ptr<Entity>> entities;
    std::map<Resource, Bitmap> bitmapDictionary;
    std::map<Resource, std::wstring> fileDictionary;
    std::shared_ptr<Level> currentLevel;
    std::vector<GDIBitmap> gdiBitmaps;
    std::vector<Bitmap> bitmaps;
    std::shared_ptr<Player> player;
    Bitmap playerBitmap;
    int score;
    HGDIOBJ oldObject;
private:    
};

#endif