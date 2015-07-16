#include "Game.h"

void Game::InitializeGraphics(void* window)
{
    // spara undan fönsterhandtaget. anledningen att den ör void* ör för att
    // slippa #include <Windows.h> i headern
    this->window = (HWND)window;

    // skapa en DC för backbuffern som har samma format som fönstret
    HDC hdc = GetDC(this->window);
    backbufferDC = CreateCompatibleDC(hdc);

    // ta reda pö hur stort fönstret ör
    RECT windowSize = { 0 };
    windowSize = WindowOption::MAP_CANVAS_RECT;

    windowWidth = windowSize.right - windowSize.left;
    windowHeight = windowSize.bottom - windowSize.top;

    // skapa en minnesbuffer för dubbelbuffring
    backbufferBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
    SelectObject(backbufferDC, backbufferBitmap);

    // sött textbakgrunden genomskinlig
    SetBkMode(backbufferDC, TRANSPARENT);

    // skapa en DC för utritning av bilder
    bitmapDC = CreateCompatibleDC(hdc);

    LoadBitmapFromFile(std::wstring(_T("player.bmp")), this->player, Resource::PlayerTile);

    auto player = std::make_shared<Player>(0, 0);
    player->resource = Resource::PlayerTile;

    this->entities.push_back(player);
}


void Game::Render(const double interpolation) {
    BeginGraphics();

    for (auto& entity : entities) {
        DrawBitmap(bitmapDictionary.at(entity->resource), entity->x, entity->y);
    }

    DrawString(std::wstring(_T("Du har noll poäng. Din nolla!")), 0, 0);

    EndGraphics();
}

bool Game::LoadBitmapFromFile(const std::wstring filename, Bitmap& bitmap, Resource resource)
{
    // ladda bitmapen
    GDIBitmap gdi;
    gdi.handle = (HBITMAP)LoadImage(0, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (gdi.handle == 0)
    {
        return false;
    }

    // ta reda pö storleken
    BITMAP header;
    GetObject(gdi.handle, sizeof(header), &header);
    gdi.width = header.bmWidth;
    gdi.height = header.bmHeight;

    // spara undan bitmap-informationen och returnera ett index
    // det "skyddar" spelet/applikationen frön implementationen, vilket
    // gör det enkelt att byta till en annan API som OpenGL eller Direct3D
    bitmaps.push_back(gdi);

    bitmap.index = bitmaps.size() - 1;
    bitmapDictionary[resource] = bitmap;
    fileDictionary[resource] = filename;

    return true;
}


void Game::BeginGraphics()
{
    // töm innehöllet i minnes-buffern
    RECT rectangle = { 0, 0, windowWidth, windowHeight };
    FillRect(backbufferDC, &rectangle, (HBRUSH)COLOR_BACKGROUND);
}

void Game::DrawBitmap(Bitmap bitmap, int x, int y)
{
    // rita ut bitmappen pö backbuffern
    GDIBitmap& gdi = bitmaps[bitmap.index];
    SelectObject(bitmapDC, gdi.handle);
    BitBlt(backbufferDC, x, y, gdi.width, gdi.height, bitmapDC, 0, 0, SRCCOPY);
}

void Game::DrawString(const std::wstring text, int x, int y)
{
    // rita ut text pö backbuffern
    TextOut(backbufferDC, x, y, text.c_str(), text.size());
}

void Game::EndGraphics()
{
    // kopiera innehöllet i minnesbuffern till skörmen
    HDC windowDC = GetDC(window);
    BitBlt(windowDC, 0, 0, windowWidth, windowHeight, backbufferDC, 0, 0, SRCCOPY);
    ReleaseDC(window, windowDC);
}

void Game::FreeBitmap(Bitmap bitmap)
{
    auto findResult = std::find_if(std::begin(bitmapDictionary), std::end(bitmapDictionary), [&](const std::pair<Resource, Bitmap>& pair) {
        return pair.second.index == bitmap.index;
    });
    Resource foundKey;
    Bitmap foundValue;
    if (findResult != std::end(bitmapDictionary))
    {
        foundKey = findResult->first;
        foundValue = findResult->second;

        // Remove file and bitmap
        bitmapDictionary.erase(foundKey);
        fileDictionary.erase(foundKey);
    }

    // fria minnet som upptas av en bitmap
    if (bitmaps[bitmap.index].handle != 0)
    {
        DeleteObject(bitmaps[bitmap.index].handle);
        bitmaps[bitmap.index].handle = 0;
    }
}

void Game::ShutdownGraphics()
{
    FreeBitmap(this->player);

    // fria alla resurser som anvönds av grafiksystemet
    DeleteDC(this->bitmapDC);

    DeleteObject(this->backbufferBitmap);
    DeleteDC(this->backbufferDC);
}

void Game::Update(const double deltaTime) {
    for (auto& entity : entities) {
        entity->Update(deltaTime);
    }
}

//Game::Game(HWND hwnd, HDC hdc) : hwnd(hwnd), hdc(hdc), entities(), resourceFiles(), HasLoaded(false) {
//
//}
//
//void Game::Initialize() {
//    assert(this->HasLoaded);
//
//    this->entities.clear();
//    auto player = std::make_shared<Player>(0, 0);
//    player->resource = Resource::PlayerTile;
//
//    this->entities.push_back(player);
//}
//
//void Game::SetupGDI() {
//    this->bufferHdc = CreateCompatibleDC(this->hdc);
//    this->bufferBitmap = CreateCompatibleBitmap(this->hdc, WindowOption::MAP_CANVAS_RECT.right, WindowOption::MAP_CANVAS_RECT.bottom);
//
//    oldGdiObj = SelectObject(bufferHdc, bufferBitmap);
//    SetBkMode(bufferHdc, TRANSPARENT); // transparent text background
//}
//
//void Game::LoadResources() {
//    this->SetupGDI();
//    
//    // Load assets
//    LoadBitmap(Resource::PlayerTile, std::wstring(_T("player.bmp")));
//    this->HasLoaded = true;
//}
//
//void Game::TeardownGDI() {
//    // Restore old object
//    SelectObject(hdc, oldGdiObj);
//
//    // Clean GDI
//    DeleteDC(this->bufferHdc);
//    this->bufferHdc = NULL;
//
//    DeleteObject(this->bufferBitmap);
//    this->bufferBitmap = NULL;
//    
//    for (auto& kv : this->bitmaps) {
//        DeleteObject(kv.second);
//    }
//}
//
//void Game::UnloadResources() {
//    this->TeardownGDI();
//
//    // Clean assets
//    this->bitmaps.clear();
//    this->resourceFiles.clear();
//    this->HasLoaded = false;
//}
//
//void Game::Update(const double deltaTime) {
//    for (auto& entity : entities) {
//        entity->Update(deltaTime);
//    }
//}
//
//void Game::BeginGraphics() {
//
//}
//
//void Game::DrawBitmap(HBITMAP bitmap, int x, int y, int width, int height) {
//
//}
//
//void Game::DrawString(const char* text, int x, int y) {
//
//}
//
//void Game::EndGraphics() {
//
//}
//
//void FreeBitmaps() {
//}
//
//void Game::RenderEntity(const std::shared_ptr<Entity>& entity, const double interpolation) const {
//    // Create a temporary buffer to draw the bitmap on. 
//    HDC bitmapDC = CreateCompatibleDC(bufferHdc); 
//
//    HBITMAP image = bitmaps.at(entity->resource);
//
//    // Load image into memory buffer for the bitmap, save handle to previous one.
//    HBITMAP hbmOld = (HBITMAP)SelectObject(bitmapDC, image);
//    
//    // Read image and put information inside the bitmap.
//    BITMAP bmp;
//    GetObject(image, sizeof(BITMAP), &bmp); 
//
//    // Render the entity by performing a bit block transfer.
//    // Use the handle to the memory buffer containing the bitmap data (bmpHdc)
//    // and transfer it onto temporary memory buffer used for double buffering
//    // (pointed to by bufferHdc).
//    entity->Render(this->bufferHdc, bmp, bitmapDC, interpolation);
//
//    //// Bit block transfer the data inside the temporary memory buffer of the 
//    //// virtual device context (bufferHdc) into the main device context memory 
//    //// buffer (hdc).
//    //BitBlt(hdc, 0, 0, winRect.right, winRect.bottom, bufferHdc, 0, 0, SRCCOPY);
//
//    // Restore old object
//    SelectObject(bitmapDC, hbmOld);
//    DeleteDC(bitmapDC);
//}
//
//void Game::Render(const double interpolation) const {
//    // Get drawable entities
//    std::vector<std::shared_ptr<Entity>> drawableEntities;
//    auto it = std::copy_if(this->entities.begin(), this->entities.end(), 
//        std::back_inserter(drawableEntities), 
//        [](const std::shared_ptr<Entity> e) { return e->IsDrawable; });
//
//    
//    // Clear canvas area where the tile map will be drawn
//    BitBlt(bufferHdc, WindowOption::MAP_CANVAS_RECT.left, 
//        WindowOption::MAP_CANVAS_RECT.top, 
//        WindowOption::MAP_CANVAS_RECT.right,
//        WindowOption::MAP_CANVAS_RECT.bottom, NULL, 0, 0, WHITENESS);
//        
//    // Render entities
//    for(auto& entity : drawableEntities) {
//        this->RenderEntity(entity, interpolation);
//    }
//
//    // Bit block transfer the data inside the temporary memory buffer of the 
//    // virtual device context (bufferHdc) into the main device context memory 
//    // buffer (hdc).
//    BitBlt(hdc, 0, 0, winRect.right, winRect.bottom, bufferHdc, 0, 0, SRCCOPY);
//}
//
//HBITMAP Game::LoadBitmap(Resource resource, std::wstring filename) {
//    HBITMAP image = (HBITMAP)LoadImage(0, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//
//    if (image == NULL) {
//
//        std::wstring errorMsg(_T("Could not load '"));
//        errorMsg += filename;
//        errorMsg += std::wstring(_T("' into memory."));
//        MessageBox(this->hwnd, errorMsg.c_str(), _T("Error"), MB_OK);
//        exit(1);
//    }
//
//    resourceFiles[resource] = filename;
//    bitmaps[resource] = image;
//    return image;
//}