#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
public:
    Entity();
    ~Entity();

    void Render() const;
    void Update();

private:

};

Entity::Entity()
{
}

Entity::~Entity()
{
}

#endif