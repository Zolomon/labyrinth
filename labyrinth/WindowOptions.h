#ifndef WINDOW_OPTIONS_H
#define WINDOW_OPTIONS_H

#include <vector>
#include <tuple>
#include <tchar.h>
#include <chrono>

class WindowOption
{
public:
    static const int WIDTH;
    static const int HEIGHT;
    static const int StartPositionX;
    static const int StartPositionY;

    static std::vector<std::tuple<int, int>> ButtonPositions;
    static std::vector<wchar_t*> ButtonLabels;
    static std::vector<int> ButtonIDs;

    static std::chrono::high_resolution_clock clock;

    static const int ButtonWidth;
    static const int ButtonHeight;

    static bool IsRunning;
    
    WindowOption();
    ~WindowOption();

private:

};

#endif