#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <chrono>
#include <memory>

#include "WindowOptions.h"
#include "Entity.h"
#include "Game.h"
#include "Level.h"
#include "Utils.h"

HWND btnNorth = NULL;
HWND btnEast = NULL;
HWND btnSouth = NULL;
HWND btnWest = NULL;

const int ID_OPEN = 1;
const int ID_QUIT = 2;
const int ID_ABOUT = 3;

const int BTN_NORTH_ID = 4;
const int BTN_EAST_ID = 5;
const int BTN_SOUTH_ID = 6;
const int BTN_WEST_ID = 7;

std::shared_ptr<Game> game;
std::weak_ptr<Game> wGame;

std::vector<int> WindowOption::ButtonIDs = []()->std::vector < int > {
    std::vector<int> v;
    v.push_back(BTN_NORTH_ID);
    v.push_back(BTN_EAST_ID);
    v.push_back(BTN_SOUTH_ID);
    v.push_back(BTN_WEST_ID);
    return v;
}();

std::vector<Entity*> entities;

HMENU CreateMainMenu()
{
    HMENU file = CreateMenu();
    AppendMenu(file, MF_STRING, ID_OPEN, _T("&New"));
    AppendMenu(file, MF_SEPARATOR, 0, 0);
    AppendMenu(file, MF_STRING, ID_QUIT, _T("&Quit"));

    HMENU help = CreateMenu();
    AppendMenu(help, MF_STRING, ID_ABOUT, _T("&About"));

    HMENU main = CreateMenu();
    AppendMenu(main, MF_POPUP, (UINT_PTR)file, _T("&File"));
    AppendMenu(main, MF_POPUP, (UINT_PTR)help, _T("&Help"));
    
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
        std::tie(x, y) = WindowOption::ButtonPositions[btnIndex];

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

    // TODO: fix duplication, casting char/ID to enum directly.
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_QUIT:
            PostQuitMessage(0);
            return 0;
            break;
        case ID_ABOUT: {
            std::wstring about(   _T("You are a lonely duck trying to get home. \r\n"));
            about += std::wstring(_T("Can you reach the exit in least amount of steps?\r\n"));
            about += std::wstring(_T("Use the buttons or WASD-keys to explore the maze!\r\n"));
            MessageBox(hwnd, about.c_str(), _T("About Labyrinth"), MB_OK);
        }
            break;
        case ID_OPEN:
            game->Start();
            break;
        case BTN_NORTH_ID:
            game->ProcessInput(Command::MoveNorth);
            break;
        case BTN_EAST_ID:
            game->ProcessInput(Command::MoveEast);
            break;
        case BTN_SOUTH_ID:
            game->ProcessInput(Command::MoveSouth);
            break;
        case BTN_WEST_ID:
            game->ProcessInput(Command::MoveWest);
            break;
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case 'W':
            game->ProcessInput(Command::MoveNorth);
            break;
        case 'D':
            game->ProcessInput(Command::MoveEast);
            break;
        case 'S':
            game->ProcessInput(Command::MoveSouth);
            break;
        case 'A':
            game->ProcessInput(Command::MoveWest);
            break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }
    break;
    case WM_ERASEBKGND:
        return 1;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Render(double interpolation) {
    game->Render(interpolation);
}

void processInput(const std::shared_ptr<Game>& game, MSG* msg) {
    while (PeekMessage(msg, NULL, NULL, NULL, PM_REMOVE)) {
        if (msg->message == WM_QUIT) {
            WindowOption::IsRunning = false;
        }

        TranslateMessage(msg);
        DispatchMessage(msg);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    wchar_t className[] = _T("LabyrinthClass");
    wchar_t windowName[] = _T("Labyrinth");

    WNDCLASS wc = { 0 };
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

    game = std::make_shared<Game>();
    game->InitializeGraphics(hwnd);

    std::shared_ptr<Level> level0 = Utils::loadLevel(std::string("levels/level0.txt"));
    game->AddLevel(level0);

    game->Start();

    MSG msg = { 0 };

    const double FRAMES_PER_SEC = 60.0;
    const double SEC_PER_UPDATE = 1.0 / FRAMES_PER_SEC;

    auto previousTime = WindowOption::clock.now();
    double lag = 0.0;
    while (WindowOption::IsRunning) {

        auto currentTime = WindowOption::clock.now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - previousTime).count();
        previousTime = currentTime;

        processInput(game, &msg);
        Render(1.0);
        game->CheckWinningCondition();

        if (SEC_PER_UPDATE - deltaTime > 0)
            Sleep(SEC_PER_UPDATE - deltaTime);
    }

    return msg.wParam;
}
