#include "Game.h"

Game::Game(HWND hwnd, HDC hdc) : hwnd(hwnd), hdc(hdc), entities(), resourceFiles(), HasLoaded(false) {

}

void Game::Initialize() {
    assert(this->HasLoaded);

    this->entities.clear();
    auto player = std::make_shared<Player>(0, 0);
    player->resource = Resource::PlayerTile;

    this->entities.push_back(player);
}

void Game::SetupGDI() {
    GetClientRect(this->hwnd, &this->winRect);
    int winWidth = winRect.right;
    int winHeight = winRect.bottom;

    this->bufferHdc = CreateCompatibleDC(this->hdc);
    this->bufferBitmap = CreateCompatibleBitmap(this->hdc, winWidth, winHeight);

    oldGdiObj = SelectObject(bufferHdc, bufferBitmap);
}

void Game::LoadResources() {
    this->SetupGDI();
    
    // Load assets
    LoadBitmap(Resource::PlayerTile, std::wstring(_T("player.bmp")));
    this->HasLoaded = true;
}

void Game::TeardownGDI() {
    // Restore old object
    SelectObject(hdc, oldGdiObj);

    // Clean GDI
    DeleteDC(this->bufferHdc);
    DeleteObject(this->bufferBitmap);

    for (auto& kv : this->bitmaps) {
        DeleteObject(kv.second);
    }
}

void Game::UnloadResources() {
    this->TeardownGDI();

    // Clean assets
    this->bitmaps.clear();
    this->resourceFiles.clear();
    this->HasLoaded = false;
}

void Game::Update(const double deltaTime) {
    for (auto& entity : entities) {
        entity->Update(deltaTime);
    }
}

void Game::Render(const double interpolation) const {
    HBITMAP image;
    BITMAP bm;
    
    // Get drawable entities
    std::vector<std::shared_ptr<Entity>> drawableEntities;
    auto it = std::copy_if(this->entities.begin(), this->entities.end(), 
        std::back_inserter(drawableEntities), 
        [](const std::shared_ptr<Entity> e) { return e->IsDrawable; });

    
    BitBlt(bufferHdc, 0, 0, winRect.right, winRect.bottom, NULL, 0, 0, WHITENESS);
    
    //Rectangle(bufferHdc, 0, 0, winRect.right, winRect.bottom);
    
    // Render 'em
    for(auto& entity : drawableEntities) {
        HDC bmpHdc = CreateCompatibleDC(bufferHdc);
        

        auto bitmapSearch = bitmaps.find(entity->resource);
        if (bitmapSearch != bitmaps.end()) {
            image = bitmapSearch->second;    
        }
        else {
            MessageBox(this->hwnd, _T("Could not find correct image."), _T("Error"), MB_OK);
            exit(1);
        }

        HBITMAP hbmOld = (HBITMAP)SelectObject(bmpHdc, image);
        //HBITMAP hbmOld = (HBITMAP)SelectObject(bufferHdc, image);
        GetObject(image, sizeof(BITMAP), &bm);
       
        //SelectObject(bufferHdc, image);
        //entity->Render(this->bufferHdc, bm, image, interpolation);
        entity->Render(this->bufferHdc, bm, bmpHdc, interpolation);

        BitBlt(hdc, 0, 0, winRect.right, winRect.bottom, bufferHdc, 0, 0, SRCCOPY);

        
        //SelectObject(bufferHdc, hbmOld);
        SelectObject(bmpHdc, hbmOld);
    }

    //BitBlt(hdc, 0, 0, winRect.right, winRect.bottom, bufferHdc, 0, 0, SRCCOPY);

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