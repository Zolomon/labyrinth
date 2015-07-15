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

HBITMAP Game::FindBitmap(Resource resource) const {

    auto bitmapSearch = bitmaps.find(resource);
    if (bitmapSearch != bitmaps.end()) {
        return bitmapSearch->second;  // load the image
    }

    MessageBox(this->hwnd, _T("Could not find correct image."), _T("Error"), MB_OK);
    exit(1);
}

void Game::RenderEntity(const std::shared_ptr<Entity>& entity, const double interpolation) const {
    // Create a temporary buffer to draw the bitmap on. 
    HDC bmpHdc = CreateCompatibleDC(bufferHdc); 

    HBITMAP image = FindBitmap(entity->resource);

    // Load image into memory buffer for the bitmap, save handle to previous one.
    HBITMAP hbmOld = (HBITMAP)SelectObject(bmpHdc, image); 
    
    // Read image and put information inside the bitmap.
    BITMAP bmp;
    GetObject(image, sizeof(BITMAP), &bmp); 

    // Render the entity by performing a bit block transfer.
    // Use the handle to the memory buffer containing the bitmap data (bmpHdc)
    // and transfer it onto temporary memory buffer used for double buffering
    // (pointed to by bufferHdc).
    entity->Render(this->bufferHdc, bmp, bmpHdc, interpolation); 

    // Bit block transfer the data inside the temporary memory buffer of the 
    // virtual device context (bufferHdc) into the main device context memory 
    // buffer (hdc).
    BitBlt(hdc, 0, 0, winRect.right, winRect.bottom, bufferHdc, 0, 0, SRCCOPY);

    // Restore old object
    SelectObject(bmpHdc, hbmOld);
}

void Game::Render(const double interpolation) const {
    // Get drawable entities
    std::vector<std::shared_ptr<Entity>> drawableEntities;
    auto it = std::copy_if(this->entities.begin(), this->entities.end(), 
        std::back_inserter(drawableEntities), 
        [](const std::shared_ptr<Entity> e) { return e->IsDrawable; });

    
    // Clear canvas area where the tile map will be drawn
    BitBlt(bufferHdc, WindowOption::MAP_CANVAS_RECT.left, 
        WindowOption::MAP_CANVAS_RECT.top, 
        WindowOption::MAP_CANVAS_RECT.right,
        WindowOption::MAP_CANVAS_RECT.bottom, NULL, 0, 0, WHITENESS);
        
    // Render entities
    for(auto& entity : drawableEntities) {
        this->RenderEntity(entity, interpolation);
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