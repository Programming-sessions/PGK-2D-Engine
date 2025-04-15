#include "Enemy.h"
#include <cmath>

Enemy::Enemy()
    : Entity()
    , gameMap(nullptr)
    , targetPlayer(nullptr)
    , detectionRange(500.0f)
    , maxSpeed(200.0f)        // Troch� wolniejszy ni� gracz
    , acceleration(300.0f)
    , deceleration(600.0f)
    , health(100.0f)
    , maxHealth(100.0f)
    , isMoving(false)
{
    tag = "Enemy";
    collisionRadius = 30.0f;
}

Enemy::~Enemy() {
}

bool Enemy::loadResources() {
    if (!sprite->loadTexture("assets/textures/enemy_idle.png")) {
        return false;
    }

    // Konfiguracja animacji (podobnie jak w Player)
    Animation* idleAnimation = new Animation("idle", true);

    const int FRAME_WIDTH = 180;
    const int FRAME_HEIGHT = 150;
    const float FRAME_DURATION = 0.07f;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            idleAnimation->addFrame(
                col * FRAME_WIDTH,
                row * FRAME_HEIGHT,
                FRAME_WIDTH,
                FRAME_HEIGHT,
                FRAME_DURATION
            );
        }
    }

    sprite->addAnimation(idleAnimation);
    sprite->playAnimation("idle");

    return true;
}

void Enemy::update(float deltaTime) {
    if (!isActive) return;

    // Aktualizacja AI
    updateAI(deltaTime);

    // Zapisz poprzedni� pozycj� przed ruchem
    Point2D previousPosition = position;

    // Aktualizacja pozycji
    position.setX(position.getX() + velocity.getX() * deltaTime);
    if (gameMap && gameMap->checkCollision(position, collisionRadius)) {
        position.setX(previousPosition.getX());
        velocity.setX(0.0f);
    }

    position.setY(position.getY() + velocity.getY() * deltaTime);
    if (gameMap && gameMap->checkCollision(position, collisionRadius)) {
        position.setY(previousPosition.getY());
        velocity.setY(0.0f);
    }

    // Aktualizacja sprite'a
    if (sprite) {
        sprite->setPosition(position.getX(), position.getY());
        sprite->setRotation(rotation);
        sprite->updateAnimation(deltaTime);
    }
}

void Enemy::updateAI(float deltaTime) {
    if (!targetPlayer) return;

    if (isPlayerInRange()) {
        rotateTowardsPlayer();
        moveTowardsPlayer(deltaTime);
    }
    else {
        // Zatrzymaj si� je�li gracz jest poza zasi�giem
        velocity.setPosition(0.0f, 0.0f);
        isMoving = false;
    }
}

bool Enemy::isPlayerInRange() const {
    if (!targetPlayer) return false;

    float dx = targetPlayer->getPosition().getX() - position.getX();
    float dy = targetPlayer->getPosition().getY() - position.getY();
    float distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= detectionRange * detectionRange;
}

void Enemy::rotateTowardsPlayer() {
    if (!targetPlayer) return;

    float dx = targetPlayer->getPosition().getX() - position.getX();
    float dy = targetPlayer->getPosition().getY() - position.getY();
    rotation = atan2(dy, dx);
}

void Enemy::moveTowardsPlayer(float deltaTime) {
    if (!targetPlayer) return;

    // Oblicz kierunek do gracza
    float dx = targetPlayer->getPosition().getX() - position.getX();
    float dy = targetPlayer->getPosition().getY() - position.getY();
    float length = sqrt(dx * dx + dy * dy);

    if (length > 0) {
        dx /= length;
        dy /= length;
        isMoving = true;
    }

    // Aktualizacja pr�dko�ci z przyspieszeniem
    if (isMoving) {
        velocity.setX(velocity.getX() + dx * acceleration * deltaTime);
        velocity.setY(velocity.getY() + dy * acceleration * deltaTime);

        // Ograniczenie maksymalnej pr�dko�ci
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > maxSpeed) {
            float scale = maxSpeed / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
    }
}

void Enemy::setMap(Map* map) {
    gameMap = map;
}

void Enemy::setTarget(Player* player) {
    targetPlayer = player;
}

void Enemy::setDetectionRange(float range) {
    detectionRange = range;
}

void Enemy::takeDamage(float amount) {
    health = std::max(0.0f, health - amount);
}

float Enemy::getHealth() const {
    return health;
}

float Enemy::getMaxHealth() const {
    return maxHealth;
}

bool Enemy::isAlive() const {
    return health > 0;
}
