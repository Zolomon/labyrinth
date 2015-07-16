#ifndef WINDOW_OPTIONS_H
#define WINDOW_OPTIONS_H

#include <vector>
#include <tuple>
#include <tchar.h>
#include <windows.h>
#include <chrono>

class WindowOption
{
public:
    static const int WIDTH;
    static const int HEIGHT;
    static const int StartPositionX;
    static const int StartPositionY;
    static const int TILE_WIDTH;
    static const int TILE_HEIGHT;

    static std::vector<std::tuple<int, int>> ButtonPositions;
    static std::vector<wchar_t*> ButtonLabels;
    static std::vector<int> ButtonIDs;

    static std::chrono::high_resolution_clock clock;

    static const int ButtonWidth;
    static const int ButtonHeight;

    static bool IsRunning;
    
    static const RECT MAP_CANVAS_RECT;

    WindowOption();
    ~WindowOption();

private:

};

#endif