#include "Enemy.h"
#include <cmath>

Enemy::Enemy()
    : Entity()
    , gameMap(nullptr)
    , targetPlayer(nullptr)
    , detectionRange(600.0f)
    , maxSpeed(200.0f)        // Trochê wolniejszy ni¿ gracz
    , acceleration(300.0f)
    , deceleration(600.0f)
    , isMoving(false)
    , shootCooldown(2.0f)       // Strza³ co 2 sekundy
    , currentCooldown(0.0f)
    , shootRange(500.0f)         // Zasiêg strza³u
    , canShoot(true)
	, preferredDistance(400.0f)  // Preferowana odleg³oœæ od gracza
	, distanceMargin(50.0f)      // Margines odleg³oœci
	, hasLastKnownPosition(false)
	, arrivalThreshold(30.0f) // Jak blisko punktu musi byæ przeciwnik ¿eby uznaæ ¿e dotar³
	, alertDuration(3.0f)        // Jak d³ugo bot jest w stanie czujnoœci po trafieniu
	, currentAlertTime(0.0f)     // Obecny czas czujnoœci
	, isAlerted(false)          // Czy bot jest w stanie czujnoœci
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

    // Aktualizacja stanu czujnoœci
    if (isAlerted) {
        currentAlertTime -= deltaTime;
        if (currentAlertTime <= 0) {
            isAlerted = false;
        }
    }

    // Aktualizacja cooldownu
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;
    }

    // Oblicz aktualn¹ odleg³oœæ od gracza
    float dx = targetPlayer->getPosition().getX() - position.getX();
    float dy = targetPlayer->getPosition().getY() - position.getY();
    float currentDistance = sqrt(dx * dx + dy * dy);

    bool canSeePlayer = currentDistance <= detectionRange && hasLineOfSight();

    // SprawdŸ czy gracz jest w zasiêgu wykrywania
    if (canSeePlayer || isAlerted) {

        // Gracz jest widoczny - zapisz jego pozycjê
        if (canSeePlayer) {
            updateLastKnownPlayerPosition(targetPlayer->getPosition());
        }

        // Zawsze obracaj siê w stronê gracza gdy jest w zasiêgu
        rotateTowardsPlayer();

        // SprawdŸ czy powinien siê poruszaæ (based on preferred distance)
        bool shouldMove = abs(currentDistance - preferredDistance) > distanceMargin;

        if (shouldMove) {
            float moveDirection = (currentDistance > preferredDistance) ? 1.0f : -1.0f;

            // Oblicz docelowy wektor prêdkoœci
            float targetDirX = cos(rotation) * moveDirection;
            float targetDirY = sin(rotation) * moveDirection;

            // P³ynne przyspieszanie
            velocity.setX(velocity.getX() + targetDirX * acceleration * deltaTime);
            velocity.setY(velocity.getY() + targetDirY * acceleration * deltaTime);

            // Ograniczenie maksymalnej prêdkoœci
            float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
            if (speed > maxSpeed) {
                float scale = maxSpeed / speed;
                velocity.setX(velocity.getX() * scale);
                velocity.setY(velocity.getY() * scale);
            }

            isMoving = true;
        }
        else {
            // P³ynne hamowanie gdy jest w preferredDistance
            float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
            if (speed > 0) {
                float scale = std::max(0.0f, speed - deceleration * deltaTime) / speed;
                velocity.setX(velocity.getX() * scale);
                velocity.setY(velocity.getY() * scale);
            }
            isMoving = false;
        }

        // Strzelanie tylko gdy jest w zasiêgu strza³u
        if (currentDistance <= shootRange && hasLineOfSight() && currentCooldown <= 0) {
            shoot();
            currentCooldown = shootCooldown;
        }
    }
    else {
        // Gracz poza zasiêgiem lub zas³oniêty - idŸ do ostatniej znanej pozycji
        if (hasLastKnownPosition) {
            // Oblicz odleg³oœæ do ostatniej znanej pozycji
            float dxLast = lastKnownPlayerPosition.getX() - position.getX();
            float dyLast = lastKnownPlayerPosition.getY() - position.getY();
            float distanceToLastPos = sqrt(dxLast * dxLast + dyLast * dyLast);

            // Jeœli jesteœmy wystarczaj¹co blisko, przestañ siê poruszaæ
            if (distanceToLastPos < arrivalThreshold) {
                hasLastKnownPosition = false;  // Reset poszukiwania

                // Hamowanie
                float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
                if (speed > 0) {
                    float scale = std::max(0.0f, speed - deceleration * deltaTime) / speed;
                    velocity.setX(velocity.getX() * scale);
                    velocity.setY(velocity.getY() * scale);
                }
            }
            else {
                // Obróæ siê w kierunku ostatniej znanej pozycji
                float targetRotation = atan2(dyLast, dxLast);
                rotation = targetRotation;

                // Ruch w kierunku punktu
                float targetDirX = cos(rotation);
                float targetDirY = sin(rotation);

                // Przyspieszanie
                velocity.setX(velocity.getX() + targetDirX * acceleration * deltaTime);
                velocity.setY(velocity.getY() + targetDirY * acceleration * deltaTime);

                // Ograniczenie prêdkoœci
                float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
                if (speed > maxSpeed) {
                    float scale = maxSpeed / speed;
                    velocity.setX(velocity.getX() * scale);
                    velocity.setY(velocity.getY() * scale);
                }

                isMoving = true;
            }
        }
        else {
            // Nie ma ostatniej znanej pozycji - zatrzymaj siê
            float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
            if (speed > 0) {
                float scale = std::max(0.0f, speed - deceleration * deltaTime) / speed;
                velocity.setX(velocity.getX() * scale);
                velocity.setY(velocity.getY() * scale);
            }
            isMoving = false;
        }
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

bool Enemy::hasLineOfSight() const {
    if (!targetPlayer) return false;

    // Raycast od pozycji przeciwnika do gracza
    Point2D start = position;
    Point2D end = targetPlayer->getPosition();

    // Oblicz wektor kierunkowy
    float dx = end.getX() - start.getX();
    float dy = end.getY() - start.getY();
    float distance = sqrt(dx * dx + dy * dy);

    // Normalizacja wektora
    dx /= distance;
    dy /= distance;

    // SprawdŸ kolizje wzd³u¿ linii
    const float step = 10.0f;  // Krok raycasta
    Point2D checkPoint = start;

    for (float i = 0; i < distance; i += step) {
        checkPoint.setX(start.getX() + dx * i);
        checkPoint.setY(start.getY() + dy * i);

        // Stwórz tymczasow¹ kolizjê do sprawdzenia
        Collision tempCollision(CollisionShape::CIRCLE, CollisionLayer::PROJECTILE);
        tempCollision.setPosition(checkPoint);
        tempCollision.setRadius(5.0f);

        auto collisions = CollisionManager::getInstance()->getCollisions(&tempCollision);
        for (auto* hit : collisions) {
            // Ignoruj kolizje z graczem i samym sob¹
            if (hit->getOwner() != this && hit->getOwner() != targetPlayer &&
                hit->getLayer() == CollisionLayer::ENTITY) {
                return false;  // Znaleziono przeszkodê
            }
        }
    }

    return true;  // Brak przeszkód
}

void Enemy::updateLastKnownPlayerPosition(const Point2D& position) {
    lastKnownPlayerPosition = position;
    hasLastKnownPosition = true;
    isAlerted = true;
    currentAlertTime = alertDuration;
}

void Enemy::takeDamage(float damage) {
    Entity::takeDamage(damage);  // Wywo³aj metodê bazow¹

	// Zak³adamy ¿e ka¿dy pocisk który trafia bota jest od gracza //TODO : Przekazywanie ownera Bulleta przez TakeDamage
    if (targetPlayer) {  // Upewniamy siê ¿e mamy referencjê do gracza
        updateLastKnownPlayerPosition(targetPlayer->getPosition());
        isAlerted = true;
        currentAlertTime = alertDuration;
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

bool Enemy::isAlive() const {
    return health > 0;
}

