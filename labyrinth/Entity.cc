#include "Entity.h"

Entity::Entity(bool isDrawable, const double x, const double y) : IsDrawable(isDrawable), x(x), y(y) { }
Entity::~Entity() { }