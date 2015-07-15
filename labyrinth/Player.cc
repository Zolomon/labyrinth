#include "Player.h"

Player::Player(double x, double y) : Entity(true, x, y) {

}

Player::~Player() { }

void Player::Render(HDC hdcWindow, const BITMAP& bm, HDC bufferHdc, const double interpolation) const {
    BitBlt(hdcWindow, this->x + (3 * interpolation), this->y, bm.bmWidth, bm.bmHeight, bufferHdc, 0, 0, SRCCOPY);
}

void Player::Update(const double deltaTime) {
    this->x += 20*deltaTime;
    
    if (this->x > WindowOption::MAP_CANVAS_RECT.right) {
        this->x -= WindowOption::MAP_CANVAS_RECT.right;
    }
    //this->y += deltaTime
}