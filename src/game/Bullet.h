// Bullet.h
#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "CollisionManager.h"
#include "../engine/TextureManager.h"

class Bullet : public Entity {
private:
    float lifeTime;
    float speed;
    float damage;
    Entity* owner;
    Collision* collision;
    ALLEGRO_BITMAP* bulletTexture;
    bool logBullet;

public:
    Bullet();
    virtual ~Bullet();

    virtual void update(float deltaTime) override;
    virtual void draw() override;

    void initialize(const Point2D& position, float rotation, Entity* shooter);
    float getDamage() const { return damage; }
    Entity* getOwner() const { return owner; }
    Collision* getCollision() const { return collision; }
};

#endif
