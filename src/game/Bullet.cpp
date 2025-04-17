// Bullet.cpp
#include "Bullet.h"
#include "../engine/Engine.h"

Bullet::Bullet()
    : Entity()
    , lifeTime(2.0f)
    , speed(800.0f)
    , damage(10.0f)
    , owner(nullptr)
    , bulletTexture(nullptr)
    , logBullet(true)
{
    collision = new Collision(CollisionShape::CIRCLE, CollisionLayer::PROJECTILE);
    collision->setRadius(8.0f);
    CollisionManager::getInstance()->addCollision(collision);
}

Bullet::~Bullet() {
    if (collision) {
        CollisionManager::getInstance()->removeCollision(collision);
        delete collision;
    }
}

void Bullet::initialize(const Point2D& position, float rotation, Entity* shooter) {
    setPosition(position.getX(), position.getY());
    setRotation(rotation);
    owner = shooter;

    float velX = cos(rotation) * speed;
    float velY = sin(rotation) * speed;
    setVelocity(velX, velY);

    collision->setPosition(position);
    bulletTexture = TextureManager::getInstance()->loadTexture("assets/textures/bullet.png");
}

void Bullet::update(float deltaTime) {
    if (!isActive) return;

    lifeTime -= deltaTime;
    if (lifeTime <= 0) {
        if (logBullet) {
            Engine::getInstance()->getLogger().info("Bullet destroyed - lifetime exceeded at: X=" +
                std::to_string(position.getX()) + " Y=" +
                std::to_string(position.getY()));
        }
        destroy();
        return;
    }

    Point2D newPos = getPosition();
    newPos.setX(newPos.getX() + velocity.getX() * deltaTime);
    newPos.setY(newPos.getY() + velocity.getY() * deltaTime);
    collision->setPosition(newPos);

    auto collisions = CollisionManager::getInstance()->getCollisions(collision);
    for (auto* hitCollision : collisions) {
        if (hitCollision->getLayer() != CollisionLayer::PROJECTILE) {
            if (logBullet) {
                Engine::getInstance()->getLogger().info("Bullet destroyed - collision detected at: X=" +
                    std::to_string(newPos.getX()) + " Y=" +
                    std::to_string(newPos.getY()) +
                    " with object of layer: " + std::to_string(static_cast<int>(hitCollision->getLayer())));
            }
            destroy();
            return;
        }
    }

    setPosition(newPos.getX(), newPos.getY());
}

void Bullet::draw() {
    if (!isActive || !bulletTexture) return;

    ALLEGRO_TRANSFORM currentTransform;
    al_copy_transform(&currentTransform, al_get_current_transform());

    float screenX = position.getX() * currentTransform.m[0][0] + currentTransform.m[2][0];
    float screenY = position.getY() * currentTransform.m[1][1] + currentTransform.m[2][1];
    float adjustedRotation = rotation + ALLEGRO_PI / 2;

    al_draw_rotated_bitmap(bulletTexture,
        8, 8,
        screenX, screenY,
        adjustedRotation,
        0
    );
}
