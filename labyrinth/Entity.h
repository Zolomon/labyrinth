#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
public:
    Entity(double x, double y);
    ~Entity();

    virtual void Render(const double interpolation) const = 0;
    virtual void Update(const double deltaTime) = 0;

    double x;
    double y;

    double vx;
    double vy;

    BITMAP bm;
};
#endif