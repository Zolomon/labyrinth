#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "WindowOptions.h"

class Player : public Entity
{
public:
    Player(double x, double y);
    ~Player();

    void Update(const double deltaTime);
    void Render(HDC hdcWindow, const BITMAP& bm, HDC hdcImage, const double interpolation) const;
private:

};
#endif