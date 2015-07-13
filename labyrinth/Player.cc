#include "Player.h"

Player::Player(double x, double y) : Entity(true, x, y) {

}

Player::~Player() { }

void Player::Render(HDC hdcWindow, const BITMAP& bm, HDC hdcImage, const double interpolation) const {
    BitBlt(hdcWindow, 0,0,bm.bmWidth, bm.bmHeight, hdcImage,0,0, SRCCOPY);
}

void Player::Update(const double deltaTime) {
}