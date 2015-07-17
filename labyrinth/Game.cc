#include "Game.h"

bool Game::LoadBitmapFromFile(const std::wstring filename, Resource resource)
{
    Bitmap bitmap;
    GDIBitmap gdi;
    gdi.handle = (HBITMAP)LoadImage(0, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (gdi.handle == 0)
    {
        return false;
    }

    // Read size
    BITMAP header;
    GetObject(gdi.handle, sizeof(header), &header);
    gdi.width = header.bmWidth;
    gdi.height = header.bmHeight;

    // cache data
    gdiBitmaps.push_back(gdi);
    bitmap.index = gdiBitmaps.size() - 1;
    bitmaps.push_back(bitmap);
    bitmapDictionary[resource] = bitmap;
    fileDictionary[resource] = filename;

    return true;
}

void Game::InitializeGraphics(HWND window)
{
    // Double buffering, borrowed from GDI-provided wrapper in next assignment
    this->window = window;

    HDC hdc = GetDC(this->window);
    backbufferDC = CreateCompatibleDC(hdc);

    RECT windowSize = { 0 };
    windowSize = WindowOption::MAP_CANVAS_RECT;

    windowWidth = windowSize.right - windowSize.left;
    windowHeight = windowSize.bottom - windowSize.top;

    backbufferBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
    // Store old object so that we don't leak.
    oldObject = SelectObject(backbufferDC, backbufferBitmap);

    SetBkMode(backbufferDC, TRANSPARENT);

    bitmapDC = CreateCompatibleDC(hdc);

    // Load assets
    LoadBitmapFromFile(std::wstring(_T("wall.bmp")), Resource::WallTile);
    LoadBitmapFromFile(std::wstring(_T("grass.bmp")), Resource::GrassTile);
    LoadBitmapFromFile(std::wstring(_T("start.bmp")), Resource::StartTile);
    LoadBitmapFromFile(std::wstring(_T("end.bmp")), Resource::EndTile);
    LoadBitmapFromFile(std::wstring(_T("player.bmp")), Resource::PlayerTile);

    this->player = std::make_shared<Player>(0, 0);
    player->resource = Resource::PlayerTile;

    // Initialize entities
    this->entities.push_back(player);
}

void Game::BeginGraphics()
{
    RECT rectangle = { 0, 0, windowWidth, windowHeight };
    FillRect(backbufferDC, &rectangle, (HBRUSH)COLOR_BACKGROUND);
}

void Game::DrawBitmap(Bitmap bitmap, int x, int y) const
{
    const GDIBitmap& gdi = gdiBitmaps.at(bitmap.index);
    SelectObject(bitmapDC, gdi.handle);
    BitBlt(backbufferDC, x, y, gdi.width, gdi.height, bitmapDC, 0, 0, SRCCOPY);
}

void Game::DrawString(const std::wstring text, COLORREF color, int x, int y) const
{
    SetTextColor(backbufferDC, color);
    TextOut(backbufferDC, x, y, text.c_str(), text.size());
}

void Game::DrawLevel() const {
    for (int y = 0; y < currentLevel->TILES_PER_ROW; y++) {
        for (int x = 0; x < currentLevel->TILES_PER_COLUMN; x++) {
            auto resource = currentLevel->tiles[x + (y * currentLevel->TILES_PER_COLUMN)];
            DrawBitmap(bitmapDictionary.at(resource), x * WindowOption::TILE_WIDTH, y * WindowOption::TILE_HEIGHT);
        }
    }
}

void Game::Render(const double interpolation) {
    BeginGraphics();

    DrawLevel();

    // In case more entities are added.
    for (auto& entity : entities) {
        DrawBitmap(bitmapDictionary.at(entity->resource), entity->x, entity->y);
    }

    std::wstring scoreText(_T("Score: "));
    scoreText += std::to_wstring(score);
    DrawString(scoreText, RGB(0,0,0), 2, 2);
    DrawString(scoreText, RGB(255, 255, 255), 0, 0);

    EndGraphics();
}

void Game::EndGraphics()
{
    // Blit-block transfer to the main device context
    HDC windowDC = GetDC(window);
    BitBlt(windowDC, 0, 0, windowWidth, windowHeight, backbufferDC, 0, 0, SRCCOPY);
    ReleaseDC(window, windowDC);
}

void Game::FreeBitmap(Bitmap bitmap)
{
    // Find the resource, from the bitmap, to free
    auto findResult = std::find_if(std::begin(bitmapDictionary), std::end(bitmapDictionary), [&](const std::pair<Resource, Bitmap>& pair) {
        return pair.second.index == bitmap.index;
    });
    Resource foundKey;
    if (findResult != std::end(bitmapDictionary))
    {
        foundKey = findResult->first;

        // Remove file and bitmap
        bitmapDictionary.erase(foundKey);
        fileDictionary.erase(foundKey);
    }

    // fria minnet som upptas av en bitmap
    if (gdiBitmaps[bitmap.index].handle != 0)
    {
        DeleteObject(gdiBitmaps[bitmap.index].handle);
        gdiBitmaps[bitmap.index].handle = 0;
    }
}

void Game::ShutdownGraphics()
{
    for (auto& bm : bitmaps) {
        FreeBitmap(bm);
    }

    // fria alla resurser som anvönds av grafiksystemet
    DeleteDC(this->bitmapDC);

    DeleteObject(this->backbufferBitmap);
    SelectObject(this->backbufferDC, oldObject);
    DeleteDC(this->backbufferDC);
}

// Start game
void Game::Start() {
    int startPosition = find(currentLevel->tiles.begin(), currentLevel->tiles.end(), Resource::StartTile) - this->levels[0]->tiles.begin();
    int x = startPosition % currentLevel->TILES_PER_COLUMN;
    int y = startPosition / currentLevel->TILES_PER_COLUMN;
    player->x = x * WindowOption::TILE_WIDTH;
    player->y = y * WindowOption::TILE_HEIGHT;
    score = 0;
}

// Move player, assume level's contour is made out of walls,
// otherwise perform boundary checks.
void Game::MovePlayer(Command command) {
    int x = (int)player->x / WindowOption::TILE_WIDTH;
    int y = (int)player->y / WindowOption::TILE_HEIGHT;

    // TODO: Fix duplication
    if (command == Command::MoveNorth) {
        auto position = x + ((y - 1) * currentLevel->TILES_PER_COLUMN);
        auto tile = currentLevel->tiles[position];
        if (tile != Resource::WallTile) {
            player->y -= WindowOption::TILE_HEIGHT;
        }
    }
    if (command == Command::MoveEast) {
        auto position = 1 + x + (y * currentLevel->TILES_PER_COLUMN);
        auto tile = currentLevel->tiles[position];
        if (tile != Resource::WallTile) {
            player->x += WindowOption::TILE_WIDTH;
        }
    }
    if (command == Command::MoveSouth) {
        auto position = x + ((y + 1) * currentLevel->TILES_PER_COLUMN);
        auto tile = currentLevel->tiles[position];
        if (tile != Resource::WallTile) {
            player->y += WindowOption::TILE_HEIGHT;
        }
    }
    if (command == Command::MoveWest) {
        auto position = x - 1 + (y * currentLevel->TILES_PER_COLUMN);
        auto tile = currentLevel->tiles[position];
        if (tile != Resource::WallTile) {
            player->x -= WindowOption::TILE_WIDTH;
        }
    }

    score++;
}

void Game::ProcessInput(Command command) {
    switch (command) {
    case Command::MoveEast:
    case Command::MoveNorth:
    case Command::MoveSouth:
    case Command::MoveWest:
        MovePlayer(command);
        break;
    case Command::Restart:
        Start();
    }
}

void Game::CheckWinningCondition() {
    int x = (int)player->x / WindowOption::TILE_WIDTH;
    int y = (int)player->y / WindowOption::TILE_HEIGHT;
    auto position = x + (y * currentLevel->TILES_PER_COLUMN);
    auto tile = currentLevel->tiles[position];
    if (tile == Resource::EndTile) {
        std::wstring finishText(_T("You found the exit "));
        finishText += std::to_wstring(score);
        finishText += std::wstring(_T(" in steps!\r\n"));
        finishText += std::wstring(_T("Would you like to play again?"));
        if (MessageBox(this->window, finishText.c_str(), _T("Freedom at last"), MB_YESNO) == IDYES) {
            Start();
        }
        else {
            exit(1);
        }
    }
}

void Game::AddLevel(std::shared_ptr<Level> level) {
    this->levels.push_back(level);
    currentLevel = level;
}