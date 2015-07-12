#include "Entity.h"

Entity::Entity(bool isDrawable, const double x, const double y) : IsDrawable(isDrawable), x(x), y(y), vx(0), vy(0) {
}

Entity::~Entity() { }