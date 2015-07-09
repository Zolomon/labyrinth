#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <chrono>

#include "WindowOptions.h"

HWND btnNorth = NULL;
HWND btnEast  = NULL;
HWND btnSouth = NULL;
HWND btnWest  = NULL;

const int ID_OPEN = 1;
const int ID_QUIT = 2;
const int ID_ABOUT = 3;

const int BTN_NORTH_ID = 4;
const int BTN_EAST_ID = 5;
const int BTN_SOUTH_ID = 6;
const int BTN_WEST_ID = 7;

std::vector<int> WindowOption::ButtonIDs = []()->std::vector<int>{
    std::vector<int> v;
    v.push_back(BTN_NORTH_ID);
    v.push_back(BTN_EAST_ID);
    v.push_back(BTN_SOUTH_ID);
    v.push_back(BTN_WEST_ID);
    return v;
}();

HMENU CreateMainMenu()
{
    HMENU file = CreateMenu();
    AppendMenu(file, MF_STRING, ID_OPEN, _T("&Open"));
    AppendMenu(file, MF_SEPARATOR, 0, 0);
    AppendMenu(file, MF_STRING, ID_QUIT, _T("&Quit"));

    HMENU help = CreateMenu();
    AppendMenu(help, MF_STRING, ID_ABOUT, _T("&About"));

    HMENU main = CreateMenu();
    AppendMenu(main, MF_POPUP, (UINT_PTR)file, _T("&File"));
    AppendMenu(main, MF_POPUP, (UINT_PTR)help, _T("&Help"));

    EnableMenuItem(main, ID_OPEN, MF_BYCOMMAND | MF_GRAYED);

    return main;
}

void SetupButtons(HWND hwnd, HINSTANCE hInstance) {
    HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    std::vector<HWND*> buttons;
    buttons.push_back(&btnNorth);
    buttons.push_back(&btnEast);
    buttons.push_back(&btnSouth);
    buttons.push_back(&btnWest);

    size_t btnIndex = 0;
    for (HWND* btn : buttons) {
        int x, y;
        std::tie(x,y) = WindowOption::ButtonPositions[btnIndex];

        wchar_t* name = WindowOption::ButtonLabels[btnIndex];

        *btn = CreateWindow(
            _T("BUTTON"),
            name,
            WS_CHILD | WS_VISIBLE,
            x, y,
            WindowOption::ButtonWidth, WindowOption::ButtonHeight,
            hwnd,
            (HMENU)WindowOption::ButtonIDs[btnIndex],
            hInstance, NULL);

        SendMessage(*btn, WM_SETFONT, (WPARAM)font, NULL);

        btnIndex++;
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_QUIT:
            PostQuitMessage(0);
            return 0;
            break;
        case ID_ABOUT:
            MessageBox(hwnd, _T("About this program!"), _T("About"), MB_OK);
            break;
        case BTN_NORTH_ID:
            MessageBox(hwnd, _T("You clicked north!"), _T("Direction"), MB_OK);
            break;
        case BTN_EAST_ID:
            MessageBox(hwnd, _T("You clicked east!"), _T("Direction"), MB_OK);
            break;
        case BTN_SOUTH_ID:
            MessageBox(hwnd, _T("You clicked south!"), _T("Direction"), MB_OK);
            break;
        case BTN_WEST_ID:
            MessageBox(hwnd, _T("You clicked west!"), _T("Direction"), MB_OK);
            break;
        }
        break;
    case WM_KEYDOWN:
        if (wParam == 'W')
            MessageBox(hwnd, _T("You pressed 'W'"), _T("Keydown"), MB_OK);
        if (wParam == 'D')
            MessageBox(hwnd, _T("You pressed 'D'"), _T("Keydown"), MB_OK);
        if (wParam == 'S')
            MessageBox(hwnd, _T("You pressed 'S'"), _T("Keydown"), MB_OK);
        if (wParam == 'A')
            MessageBox(hwnd, _T("You pressed 'A'"), _T("Keydown"), MB_OK);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Update(double deltaTime) {

}

void Render(HWND hwnd) {

    BOOL succeededInvalidation = InvalidateRect(hwnd, 0, false);
    if (!succeededInvalidation) {
        exit(1);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    wchar_t className[] = _T("LabyrinthClass");
    wchar_t windowName[] = _T("Labyrinth");

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        className,
        windowName,
        WS_OVERLAPPEDWINDOW,
        WindowOption::StartPositionX, WindowOption::StartPositionY,
        WindowOption::WIDTH, WindowOption::HEIGHT,
        NULL,
        CreateMainMenu(),
        hInstance, NULL);

    SetupButtons(hwnd, hInstance);

    UpdateWindow(hwnd);
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};

    const int FRAMES_PER_SEC = 60;
    const int FRAMES_TO_SKIP = 1000 / FRAMES_PER_SEC;
    const int MAX_FRAMES_TO_SKIP = 5;

    auto previousTime = WindowOption::clock.now();
    int loops = 0;
    while(WindowOption::IsRunning) {

        auto currentTime = WindowOption::clock.now();

        while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                WindowOption::IsRunning = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
        
        loops = 0;
        while (deltaTime <= FRAMES_TO_SKIP && loops < MAX_FRAMES_TO_SKIP) {
            Update(deltaTime);

            currentTime = WindowOption::clock.now();
            deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
            loops++;
        }

        
        Render(hwnd);

        auto next_frame_occurs_at = currentTime + std::chrono::milliseconds(FRAMES_TO_SKIP);
        auto time_to_sleep = next_frame_occurs_at - 

        previousTime = currentTime;
        
    }

    return 0;
}