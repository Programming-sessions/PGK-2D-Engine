#include "Bullet.h"
#include "../engine/Engine.h"

Bullet::Bullet()
    : Entity()
    , lifeTime(2.0f)
    , speed(1500.0f)
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

void Bullet::initialize(const glm::vec2& pos, float rot, Entity* shooter) {
    setPosition(pos);
    setRotation(rot);
    owner = shooter;

    velocity.x = cos(rotation) * speed;
    velocity.y = sin(rotation) * speed;

    collision->setPosition(pos);
    bulletTexture = TextureManager::getInstance()->loadTexture("assets/textures/bullet.png");
}

void Bullet::update(float deltaTime) {
    if (!isActive) return;

    lifeTime -= deltaTime;
    if (lifeTime <= 0) {
        if (logBullet) {
            Logger::getInstance().info("Bullet destroyed - lifetime exceeded at: X=" +
                std::to_string(position.x) + " Y=" +
                std::to_string(position.y));
        }
        destroy();
        return;
    }

    glm::vec2 newPos = getPosition() + velocity * deltaTime;
    collision->setPosition(newPos);

    auto collisions = CollisionManager::getInstance()->getCollisions(collision);
    for (auto* hitCollision : collisions) {
        if (hitCollision->getLayer() != CollisionLayer::PROJECTILE) {
            if (logBullet) {
                Logger::getInstance().info("Bullet destroyed - collision detected at: X=" +
                    std::to_string(newPos.x) + " Y=" +
                    std::to_string(newPos.y) +
                    " with object of layer: " + std::to_string(static_cast<int>(hitCollision->getLayer())));
            }

            if (hitCollision->getLayer() == CollisionLayer::ENTITY && hitCollision->getOwner() != owner) {
                hitCollision->getOwner()->takeDamage(damage);
                if (logBullet) {
                    Logger::getInstance().info("Bullet hit entity at: X=" +
                        std::to_string(newPos.x) + " Y=" +
                        std::to_string(newPos.y) +
                        " Damage dealt: " + std::to_string(damage) +
                        " Target health remaining: " + std::to_string(hitCollision->getOwner()->getHealth()));
                }
            }
            destroy();
            return;
        }
    }

    setPosition(newPos);
}

void Bullet::draw() {
    if (!isActive || !bulletTexture) return;

    float adjustedRotation = rotation + ALLEGRO_PI / 2;

    al_draw_rotated_bitmap(bulletTexture,
        8, 8,
        position.x, position.y,
        adjustedRotation,
        0
    );
}