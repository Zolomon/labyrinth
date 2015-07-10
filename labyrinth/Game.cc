#include "Game.h"

Game::Game(HWND hwnd, HDC hdc) : hwnd(hwnd), hdc(hdc) {

}

void Game::Initialize() {

}

void Game::Update(const double deltaTime) {
}

void Game::Render(const double interpolation) const {
    for(auto entity : entities) {
        //LoadImage
    }
}

HBITMAP Game::LoadBitmap(Resource resource, wchar_t** filename) {
    HBITMAP image = (HBITMAP)LoadImage(0, *filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //std::map<Resource, HBITMAP> bitmaps;
    //bitmaps.emplace(std::make_pair(resource, image));
    bitmaps[resource] = image;
}