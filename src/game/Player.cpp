#include "Player.h"
#include <cmath>
#include <memory> // Required for std::make_unique

Player::Player()
    : Entity()
    , maxSpeed(300.0f)      // Pixels per second
    , acceleration(400.0f)
    , deceleration(800.0f)
    , isMoving(false)
    , shootCooldown(0.25f)  // 4 shots per second
    , currentCooldown(0.0f)
    , logMovement(false)
    , logReload(false)
    , maxAmmoInMag(10)      // Magazine capacity
    , currentAmmoInMag(10)  // Current ammo in magazine
    , totalAmmo(30)         // Total reserve ammo
    , reloadTime(1.05f)     // Reload time in seconds
    , currentReloadTime(0.0f) // Reload time counter
    , isReloading(false)    // Is reloading in progress?
    , camera(nullptr)
{
    tag = "Player";
    maxHealth = 100.0f;
    health = maxHealth;
    collision = new Collision(CollisionShape::CIRCLE, CollisionLayer::ENTITY);
    collision->setRadius(30.0f); // Adjust collision radius
    collision->setOwner(this);
}

Player::~Player() {
    delete collision;
}

bool Player::loadResources() {
    if (!sprite->loadTexture("assets/textures/player_sheet.png")) {
        return false;
    }

    const int FRAME_WIDTH = 180;
    const int FRAME_HEIGHT = 150;

    // Idle animation (first 4 rows)
    auto idleAnimation = std::make_unique<Animation>("idle", true);
    const float IDLE_FRAME_DURATION = 0.05f;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            idleAnimation->addFrame(
                col * FRAME_WIDTH,
                row * FRAME_HEIGHT,
                FRAME_WIDTH,
                FRAME_HEIGHT,
                IDLE_FRAME_DURATION
            );
        }
    }
    sprite->addAnimation(std::move(idleAnimation));

    // Reload animation (next 3 rows)
    auto reloadAnimation = std::make_unique<Animation>("reload", false);
    const float RELOAD_FRAME_DURATION = 0.07f;

    for (int row = 4; row < 7; row++) {
        for (int col = 0; col < 5; col++) {
            reloadAnimation->addFrame(
                col * FRAME_WIDTH,
                row * FRAME_HEIGHT,
                FRAME_WIDTH,
                FRAME_HEIGHT,
                RELOAD_FRAME_DURATION
            );
        }
    }

    sprite->addAnimation(std::move(reloadAnimation));
    sprite->playAnimation("idle");
    return true;
}

void Player::handleInput(float deltaTime) {
    Engine* engine = Engine::getInstance();
    Logger& logger = engine->getLogger();

    glm::vec2 dir(0.0f, 0.0f);

    if (engine->isKeyDown(ALLEGRO_KEY_W) || engine->isKeyDown(ALLEGRO_KEY_UP)) {
        dir.y -= 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_S) || engine->isKeyDown(ALLEGRO_KEY_DOWN)) {
        dir.y += 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_A) || engine->isKeyDown(ALLEGRO_KEY_LEFT)) {
        dir.x -= 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_D) || engine->isKeyDown(ALLEGRO_KEY_RIGHT)) {
        dir.x += 1.0f;
    }

    if (glm::length(dir) > 0.0f) {
        dir = glm::normalize(dir);
        isMoving = true;
        if (logMovement) { logger.info("Movement input - Dir: X=" + std::to_string(dir.x) + " Y=" + std::to_string(dir.y)); }
    }
    else {
        isMoving = false;
    }

    // Update velocity with acceleration
    if (isMoving) {
        velocity += dir * acceleration * deltaTime;

        if (glm::length(velocity) > maxSpeed) {
            velocity = glm::normalize(velocity) * maxSpeed;
        }
        if (logMovement) { logger.info("Velocity updated - V: X=" + std::to_string(velocity.x) + " Y=" + std::to_string(velocity.y)); }
    }
    else {
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

    // Handle shooting
    if (engine->isMouseButtonDown(0) && canShoot()) {
        shoot(logger);
    }

    // Handle reloading
    if (engine->isKeyDown(ALLEGRO_KEY_R)) {
        reload();
        if (isReloading && logReload) {
            logger.info("Reload started. Total ammo: " + std::to_string(totalAmmo));
        }
    }


    currentCooldown -= deltaTime;
}

void Player::lookAtMouse() {
    if (!camera) return;

    Engine* engine = Engine::getInstance();
    glm::vec2 mouseScreen(engine->getMouseX(), engine->getMouseY());
    glm::vec2 mouseWorld = camera->screenToWorld(mouseScreen);

    glm::vec2 dir = mouseWorld - position;
    rotation = atan2(dir.y, dir.x);
}

void Player::update(float deltaTime) {
    if (!isActive) return;

    // Update reloading
    if (isReloading) {
        currentReloadTime += deltaTime;
        if (sprite && sprite->getCurrentAnimation() && logReload) {
            Engine::getInstance()->getLogger().info(
                "Reload progress: " + std::to_string(currentReloadTime) + "/" +
                std::to_string(reloadTime) + " Animation: " +
                sprite->getCurrentAnimation()->getName()
            );
        }

        if (currentReloadTime >= reloadTime) {
            // Finish reloading
            int ammoNeeded = maxAmmoInMag - currentAmmoInMag;
            int ammoToLoad = std::min(ammoNeeded, totalAmmo);

            currentAmmoInMag += ammoToLoad;
            totalAmmo -= ammoToLoad;

            isReloading = false;
            currentReloadTime = 0.0f;

            // Switch back to idle
            if (sprite) {
                sprite->stopAnimation();
                sprite->playAnimation("idle");
                if(logReload) Engine::getInstance()->getLogger().info("Reload complete, switching to idle");
            }
        }
    }

    // Handle controls
    handleInput(deltaTime);
    lookAtMouse();

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
            previousPosition = newPosition;
        }
        else {
            velocity.y = 0.0f;
        }

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

void Player::shoot(Logger& logger) {
    // Offset from the center of the character to the gun's muzzle when the character is looking to the right
    const float MUZZLE_OFFSET_X = 65.0f;
    const float MUZZLE_OFFSET_Y = 42.0f;

    // We calculate the muzzle position considering the character's rotation
    glm::vec2 bulletPos = position;
    float rotatedMuzzleX = MUZZLE_OFFSET_X * cos(rotation) - MUZZLE_OFFSET_Y * sin(rotation);
    float rotatedMuzzleY = MUZZLE_OFFSET_X * sin(rotation) + MUZZLE_OFFSET_Y * cos(rotation);

    bulletPos.x += rotatedMuzzleX;
    bulletPos.y += rotatedMuzzleY;

    // Calculate the new rotation from the muzzle point to the player
    Engine* engine = Engine::getInstance();
    glm::vec2 mouseScreen(engine->getMouseX(), engine->getMouseY());
    glm::vec2 mouseWorld = camera->screenToWorld(mouseScreen);
    glm::vec2 dir = mouseWorld - bulletPos;
    float bulletRotation = atan2(dir.y, dir.x);

    BulletManager::getInstance()->createBullet(bulletPos, bulletRotation, this);
    currentCooldown = shootCooldown;
    currentAmmoInMag--;

    logger.info("Shot fired. Ammo remaining: " + std::to_string(currentAmmoInMag));
}

void Player::heal(float amount) {
    health = std::min(maxHealth, health + amount);
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::setCamera(Camera* cam) {
    camera = cam;
}

void Player::setMap(Map* map) {
    gameMap = map;
}

void Player::reload() {
    if (isReloading || totalAmmo <= 0 || currentAmmoInMag >= maxAmmoInMag) {
        return;
    }

    isReloading = true;
    currentReloadTime = 0.0f;

    // Add debug log
    Engine::getInstance()->getLogger().info("Starting reload animation");

    // Make sure the animation is switched correctly
    if (sprite) {
        sprite->stopAnimation(); // Stop the current animation
        sprite->playAnimation("reload");

        // Debug - check if the animation has changed
        if (Animation* current = sprite->getCurrentAnimation()) {
            Engine::getInstance()->getLogger().info("Current animation: " + current->getName());
        }
    }
}

bool Player::canShoot(){
    return currentAmmoInMag > 0 && !isReloading && currentCooldown <= 0;
}