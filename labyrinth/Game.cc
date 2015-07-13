#include "Game.h"

Game::Game(HWND hwnd, HDC hdc) : hwnd(hwnd), hdc(hdc), entities(), resourceFiles() {
    this->LoadResources();
}

void Game::Initialize() {

    this->entities.clear();
    auto player = std::make_shared<Player>(0, 0);
    player->resource = Resource::PlayerTile;

    this->entities.push_back(player);
}

void Game::LoadResources() {
    LoadBitmap(Resource::PlayerTile, std::wstring(_T("player.bmp")));
}

void Game::Update(const double deltaTime) {
}

void Game::Render(const double interpolation) const {
    HBITMAP image;
    BITMAP bm;

    for(auto& entity : this->entities) {
        if (entity->IsDrawable) {
            auto bitmapSearch = bitmaps.find(entity->resource);
            if (bitmapSearch != bitmaps.end()) {
                image = bitmapSearch->second;
                GetObject(image, sizeof(BITMAP), &bm);
            }
            else {
                MessageBox(this->hwnd, _T("Could not find correct image."), _T("Error"), MB_OK);
                exit(1);
            }

            HDC hdcMem = CreateCompatibleDC(this->hdc);
            SelectObject(hdcMem, image);

            entity->Render(this->hdc, bm, hdcMem, interpolation);

            DeleteDC(hdcMem);
        }
    }

    BOOL succeededInvalidation = InvalidateRect(hwnd, 0, false);
    if (!succeededInvalidation) {
        exit(1);
    }
}

HBITMAP Game::LoadBitmap(Resource resource, std::wstring filename) {
    HBITMAP image = (HBITMAP)LoadImage(0, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (image == NULL) {

        std::wstring errorMsg(_T("Could not load '"));
        errorMsg += filename;
        errorMsg += std::wstring(_T("' into memory."));
        MessageBox(this->hwnd, errorMsg.c_str(), _T("Error"), MB_OK);
        exit(1);
    }

    resourceFiles[resource] = filename;
    bitmaps[resource] = image;
    return image;
}