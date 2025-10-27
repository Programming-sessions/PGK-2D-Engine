#include "Enemy.h"
#include "BodyManager.h"
#include <cmath>
#include <memory>

Enemy::Enemy()
    : Entity()
    , gameMap(nullptr)
    , targetPlayer(nullptr)
    , detectionRange(600.0f)
    , maxSpeed(200.0f)          // A bit slower than the player
    , acceleration(300.0f)
    , deceleration(600.0f)
    , isMoving(false)
    , shootCooldown(2.0f)       // Shoots every 2 seconds
    , currentCooldown(0.0f)
    , shootRange(500.0f)        // Shooting range
    , canShoot(true)
    , preferredDistance(400.0f) // Preferred distance from the player
    , distanceMargin(50.0f)     // Distance margin
    , hasLastKnownPosition(false)
    , arrivalThreshold(30.0f)   // How close the enemy must be to a point to consider it reached
    , alertDuration(3.0f)       // How long the bot remains in a state of alert after being hit
    , currentAlertTime(0.0f)    // Current alert time
    , isAlerted(false)          // Is the bot in a state of alert?
{
    tag = "Enemy";
    // Initialization of collision
    collision = new Collision(CollisionShape::CIRCLE, CollisionLayer::ENTITY);
    collision->setRadius(30.0f);
    collision->setOwner(this);
}

Enemy::~Enemy() {
    if (collision) {
        CollisionManager::getInstance()->removeCollision(collision);
        delete collision;
        collision = nullptr;
    }
}

bool Enemy::loadResources() {
    if (!sprite->loadTexture("assets/textures/enemy_idle.png")) {
        return false;
    }

    // Animation configuration (similar to Player)
    auto idleAnimation = std::make_unique<Animation>("idle", true);

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

    sprite->addAnimation(std::move(idleAnimation));
    sprite->playAnimation("idle");

    return true;
}

void Enemy::update(float deltaTime) {
    if (!isActive) return;
    if (isDying) {
        // If the unit is in the process of dying
        if (collision) {
            CollisionManager::getInstance()->removeCollision(collision);
            delete collision;
            collision = nullptr;
        }

        BodyManager::getInstance()->addBody(position, rotation, tag);
        destroy();
        return;
    }
    // AI Update - this already contains the logic for checking range, rotation, and movement
    updateAI(deltaTime);

    // Handle collision and movement
    glm::vec2 previousPosition = position;
    glm::vec2 movement = velocity * deltaTime;
    glm::vec2 newPosition = position + movement;

    collision->setPosition(newPosition);

    if (CollisionManager::getInstance()->checkCollision(collision, newPosition)) {
        // If a collision occurred, we try to move separately on the X and Y axes

        // Try moving on the X axis
        newPosition = previousPosition;
        newPosition.x += movement.x;
        collision->setPosition(newPosition);

        if (!CollisionManager::getInstance()->checkCollision(collision, newPosition)) {
            // Movement on the X axis is possible
            previousPosition = newPosition;
        }
        else {
            velocity.x = 0.0f;
        }

        // Try moving on the Y axis
        newPosition = previousPosition;
        newPosition.y += movement.y;
        collision->setPosition(newPosition);

        if (!CollisionManager::getInstance()->checkCollision(collision, newPosition)) {
            // Movement on the Y axis is possible
            previousPosition = newPosition;
        }
        else {
            velocity.y = 0.0f;
        }

        // The final position is previousPosition
        newPosition = previousPosition;
    }

    // Update position
    position = newPosition;
    collision->setPosition(position);

    // Update sprite
    if (sprite) {
        sprite->setPosition(position);
        sprite->setRotation(rotation);
        sprite->updateAnimation(deltaTime);
    }
}


void Enemy::updateAI(float deltaTime) {
    if (!targetPlayer) return;

    // Update alert status
    if (isAlerted) {
        currentAlertTime -= deltaTime;
        if (currentAlertTime <= 0) {
            isAlerted = false;
        }
    }

    // Update cooldown
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;
    }

    // Calculate the current distance from the player
    float currentDistance = glm::distance(targetPlayer->getPosition(), position);

    bool canSeePlayer = currentDistance <= detectionRange && hasLineOfSight();

    // Check if the player is within detection range
    if (canSeePlayer || isAlerted) {

        // The player is visible - save their position
        if (canSeePlayer) {
            updateLastKnownPlayerPosition(targetPlayer->getPosition());
        }

        // Always turn towards the player when in range
        rotateTowardsPlayer();

        // Check if it should move (based on preferred distance)
        bool shouldMove = abs(currentDistance - preferredDistance) > distanceMargin;

        if (shouldMove) {
            float moveDirection = (currentDistance > preferredDistance) ? 1.0f : -1.0f;

            // Calculate the target velocity vector
            glm::vec2 targetDir(cos(rotation) * moveDirection, sin(rotation) * moveDirection);

            // Smooth acceleration
            velocity += targetDir * acceleration * deltaTime;

            // Limit maximum speed
            if (glm::length(velocity) > maxSpeed) {
                velocity = glm::normalize(velocity) * maxSpeed;
            }

            isMoving = true;
        }
        else {
            // Smooth braking when at preferredDistance
            float speed = glm::length(velocity);
            if (speed > 0) {
                glm::vec2 decelVec = glm::normalize(velocity) * deceleration * deltaTime;
                if (glm::length(decelVec) > speed) {
                    velocity = glm::vec2(0.0f, 0.0f);
                } else {
                    velocity -= decelVec;
                }
            }
            isMoving = false;
        }

        // Shooting only when in shoot range
        if (currentDistance <= shootRange && hasLineOfSight() && currentCooldown <= 0) {
            shoot();
            currentCooldown = shootCooldown;
        }
    }
    else {
        // Player out of range or obscured - go to the last known position
        if (hasLastKnownPosition) {
            // Calculate the distance to the last known position
            float distanceToLastPos = glm::distance(lastKnownPlayerPosition, position);

            // If we are close enough, stop moving
            if (distanceToLastPos < arrivalThreshold) {
                hasLastKnownPosition = false; // Reset searching

                // Braking
                float speed = glm::length(velocity);
                if (speed > 0) {
                    glm::vec2 decelVec = glm::normalize(velocity) * deceleration * deltaTime;
                    if (glm::length(decelVec) > speed) {
                        velocity = glm::vec2(0.0f, 0.0f);
                    } else {
                        velocity -= decelVec;
                    }
                }
            }
            else {
                // Turn towards the last known position
                glm::vec2 dir = lastKnownPlayerPosition - position;
                rotation = atan2(dir.y, dir.x);

                // Move towards the point
                glm::vec2 targetDir(cos(rotation), sin(rotation));

                // Acceleration
                velocity += targetDir * acceleration * deltaTime;

                // Limit speed
                if (glm::length(velocity) > maxSpeed) {
                    velocity = glm::normalize(velocity) * maxSpeed;
                }

                isMoving = true;
            }
        }
        else {
            // No last known position - stop
            float speed = glm::length(velocity);
            if (speed > 0) {
                glm::vec2 decelVec = glm::normalize(velocity) * deceleration * deltaTime;
                if (glm::length(decelVec) > speed) {
                    velocity = glm::vec2(0.0f, 0.0f);
                } else {
                    velocity -= decelVec;
                }
            }
            isMoving = false;
        }
    }
}

bool Enemy::isPlayerInRange() const {
    if (!targetPlayer) return false;

    return glm::distance(targetPlayer->getPosition(), position) <= detectionRange;
}

void Enemy::rotateTowardsPlayer() {
    if (!targetPlayer) return;

    glm::vec2 dir = targetPlayer->getPosition() - position;
    rotation = atan2(dir.y, dir.x);
}

void Enemy::moveTowardsPlayer(float deltaTime) {
    if (!targetPlayer) return;

    // Calculate direction to player
    glm::vec2 dir = targetPlayer->getPosition() - position;
    if (glm::length(dir) > 0) {
        dir = glm::normalize(dir);
        isMoving = true;
    }

    // Update velocity with acceleration
    if (isMoving) {
        velocity += dir * acceleration * deltaTime;

        // Limit maximum speed
        if (glm::length(velocity) > maxSpeed) {
            velocity = glm::normalize(velocity) * maxSpeed;
        }
    }
}

bool Enemy::isPlayerInShootRange() const {
    if (!targetPlayer) return false;

    return glm::distance(targetPlayer->getPosition(), position) <= shootRange;
}

void Enemy::shoot() {
    if (!canShoot || !targetPlayer) return;

    // Offset for the starting point of the projectile
    const float MUZZLE_OFFSET_X = 65.0f;
    const float MUZZLE_OFFSET_Y = 42.0f;

    // Calculate muzzle position
    glm::vec2 bulletPos = position;
    float rotatedMuzzleX = MUZZLE_OFFSET_X * cos(rotation) - MUZZLE_OFFSET_Y * sin(rotation);
    float rotatedMuzzleY = MUZZLE_OFFSET_X * sin(rotation) + MUZZLE_OFFSET_Y * cos(rotation);

    bulletPos.x += rotatedMuzzleX;
    bulletPos.y += rotatedMuzzleY;

    // Calculate new rotation from the muzzle point to the player
    glm::vec2 targetPos = targetPlayer->getPosition();
    glm::vec2 dir = targetPos - bulletPos;
    float bulletRotation = atan2(dir.y, dir.x);

    BulletManager::getInstance()->createBullet(bulletPos, bulletRotation, this);
}

bool Enemy::hasLineOfSight() const {
    if (!targetPlayer) return false;

    glm::vec2 start = position;
    glm::vec2 end = targetPlayer->getPosition();

    glm::vec2 dir = end - start;
    float distance = glm::length(dir);
    dir = glm::normalize(dir);

    const float step = 10.0f;
    glm::vec2 checkPoint = start;

    for (float i = 0; i < distance; i += step) {
        checkPoint = start + dir * i;

        Collision tempCollision(CollisionShape::CIRCLE, CollisionLayer::PROJECTILE);
        tempCollision.setPosition(checkPoint);
        tempCollision.setRadius(5.0f);

        auto collisions = CollisionManager::getInstance()->getCollisions(&tempCollision);
        for (auto* hit : collisions) {
            // Check if the collision is with a wall
            if (hit->getLayer() == CollisionLayer::WALL) {
                return false;
            }
        }
    }

    return true;
}

void Enemy::updateLastKnownPlayerPosition(const glm::vec2& pos) {
    lastKnownPlayerPosition = pos;
    hasLastKnownPosition = true;
    isAlerted = true;
    currentAlertTime = alertDuration;
}

void Enemy::takeDamage(float damage) {
    health -= damage;

    // Update knowledge about the player
    if (targetPlayer) {
        updateLastKnownPlayerPosition(targetPlayer->getPosition());
        isAlerted = true;
        currentAlertTime = alertDuration;
    }

    if (health <= 0) {
        isDying = true; // We mark that the unit is dying, but do not delete it yet
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