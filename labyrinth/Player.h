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
    //void Render(const Game& game, const double interpolation) const;

    const double GetDrawX() const;
    const double GetDrawY() const;
private:

};
#endif