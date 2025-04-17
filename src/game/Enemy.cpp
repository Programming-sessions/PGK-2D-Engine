#include "Enemy.h"
#include <cmath>

Enemy::Enemy()
    : Entity()
    , gameMap(nullptr)
    , targetPlayer(nullptr)
    , detectionRange(500.0f)
    , maxSpeed(200.0f)        // Trochê wolniejszy ni¿ gracz
    , acceleration(300.0f)
    , deceleration(600.0f)
    , isMoving(false)
    , shootCooldown(2.0f)       // Strza³ co 2 sekundy
    , currentCooldown(0.0f)
    , shootRange(400.0f)         // Zasiêg strza³u
    , canShoot(true)
{
    tag = "Enemy";
    // Inicjalizacja kolizji
    collision = new Collision(CollisionShape::CIRCLE, CollisionLayer::ENTITY);
    collision->setRadius(30.0f);
	collision->setOwner(this);
}

Enemy::~Enemy() {
    delete collision;
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

    // Aktualizacja AI - to ju¿ zawiera logikê sprawdzania zasiêgu, rotacji i ruchu
    updateAI(deltaTime);

    // Obs³uga kolizji i ruchu
    Point2D previousPosition = position;
    Point2D newPosition = position;
    Point2D movement(velocity.getX() * deltaTime, velocity.getY() * deltaTime);

    // Próba ruchu po przek¹tnej
    newPosition = previousPosition;
    newPosition.setX(previousPosition.getX() + movement.getX());
    newPosition.setY(previousPosition.getY() + movement.getY());
    collision->setPosition(newPosition);

    if (CollisionManager::getInstance()->checkCollision(collision, newPosition)) {
        // Jeœli wyst¹pi³a kolizja, próbujemy ruch osobno w osiach X i Y

        // Próba ruchu w osi X
        newPosition = previousPosition;
        newPosition.setX(previousPosition.getX() + movement.getX());
        collision->setPosition(newPosition);

        if (!CollisionManager::getInstance()->checkCollision(collision, newPosition)) {
            // Ruch w osi X jest mo¿liwy
            previousPosition = newPosition;
        }
        else {
            velocity.setX(0.0f);
        }

        // Próba ruchu w osi Y
        newPosition = previousPosition;
        newPosition.setY(previousPosition.getY() + movement.getY());
        collision->setPosition(newPosition);

        if (!CollisionManager::getInstance()->checkCollision(collision, newPosition)) {
            // Ruch w osi Y jest mo¿liwy
            previousPosition = newPosition;
        }
        else {
            velocity.setY(0.0f);
        }

        // Koñcowa pozycja to previousPosition
        newPosition = previousPosition;
    }

    // Aktualizacja pozycji
    position = newPosition;
    collision->setPosition(position);

    // Aktualizacja sprite'a
    if (sprite) {
        sprite->setPosition(position.getX(), position.getY());
        sprite->setRotation(rotation);
        sprite->updateAnimation(deltaTime);
    }
}


void Enemy::updateAI(float deltaTime) {
    if (!targetPlayer) return;

    // Aktualizacja cooldownu
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;
    }

    if (isPlayerInRange()) {
        rotateTowardsPlayer();
        moveTowardsPlayer(deltaTime);
        isMoving = true;

        // Sprawdzenie czy mo¿e strzelaæ
        if (isPlayerInShootRange() && currentCooldown <= 0) {
            shoot();
            currentCooldown = shootCooldown;
        }
    }
    else {
        // P³ynne hamowanie
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > 0) {
            float scale = std::max(0.0f, speed - deceleration * deltaTime) / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
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

    // Aktualizacja prêdkoœci z przyspieszeniem
    if (isMoving) {
        velocity.setX(velocity.getX() + dx * acceleration * deltaTime);
        velocity.setY(velocity.getY() + dy * acceleration * deltaTime);

        // Ograniczenie maksymalnej prêdkoœci
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > maxSpeed) {
            float scale = maxSpeed / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
    }
}

bool Enemy::isPlayerInShootRange() const {
    if (!targetPlayer) return false;

    // Obliczamy odleg³oœæ miêdzy przeciwnikiem a graczem
    float dx = targetPlayer->getPosition().getX() - position.getX();
    float dy = targetPlayer->getPosition().getY() - position.getY();
    float distanceToPlayer = sqrt(dx * dx + dy * dy);

    return distanceToPlayer <= shootRange;
}

void Enemy::shoot() {
    if (!canShoot || !targetPlayer) return;

    // Offset dla punktu startowego pocisku
    const float MUZZLE_OFFSET_X = 80.0f;
    const float MUZZLE_OFFSET_Y = 40.0f;

    // Oblicz pozycjê lufy
    Point2D bulletPos = position;
    float rotatedMuzzleX = MUZZLE_OFFSET_X * cos(rotation) - MUZZLE_OFFSET_Y * sin(rotation);
    float rotatedMuzzleY = MUZZLE_OFFSET_X * sin(rotation) + MUZZLE_OFFSET_Y * cos(rotation);

    bulletPos.setX(bulletPos.getX() + rotatedMuzzleX);
    bulletPos.setY(bulletPos.getY() + rotatedMuzzleY);

    // Oblicz now¹ rotacjê od punktu lufy do gracza
    Point2D targetPos = targetPlayer->getPosition();
    float dx = targetPos.getX() - bulletPos.getX();
    float dy = targetPos.getY() - bulletPos.getY();
    float bulletRotation = atan2(dy, dx);

    BulletManager::getInstance()->createBullet(bulletPos, bulletRotation, this);
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

bool Enemy::isAlive() const {
    return health > 0;
}
