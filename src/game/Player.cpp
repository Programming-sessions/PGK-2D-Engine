#include "Player.h"
#include <cmath>

Player::Player()
    : Entity()
    , maxSpeed(300.0f)  // Piksele na sekundê
    , acceleration(400.0f)
    , deceleration(800.0f)
    , isMoving(false)
    , shootCooldown(0.25f)  // 4 strza³y na sekundê
    , currentCooldown(0.0f)
	, logMovement(false)
	, maxAmmoInMag(10)  // Pojemnoœæ magazynka
	, currentAmmoInMag(10)  // Aktualna iloœæ naboi w magazynku
	, totalAmmo(30)  // Ca³kowita iloœæ amunicji zapasowej
	, reloadTime(1.5f)  // Czas prze³adowania w sekundach
	, currentReloadTime(0.0f) // Licznik czasu prze³adowania
	, isReloading(false)  // Czy trwa prze³adowanie
	, camera(nullptr)
{
    tag = "Player";
	maxHealth = 100.0f;
	health = maxHealth;
    collision = new Collision(CollisionShape::CIRCLE, CollisionLayer::ENTITY);
    collision->setRadius(30.0f);  // Dostosuj promieñ kolizji
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

    // Animacja idle (pierwsze 4 rzêdy)
    Animation* idleAnimation = new Animation("idle", true);
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
    sprite->addAnimation(idleAnimation);

    // Animacja prze³adowania (nastêpne 3 rzêdy)
    Animation* reloadAnimation = new Animation("reload", true);
    const float RELOAD_FRAME_DURATION = 0.09f;

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

    sprite->addAnimation(reloadAnimation);
    return true;
}

void Player::handleInput(float deltaTime) {
    Engine* engine = Engine::getInstance();
    Logger& logger = engine->getLogger();

    float dirX = 0.0f;
    float dirY = 0.0f;

    if (engine->isKeyDown(ALLEGRO_KEY_W) || engine->isKeyDown(ALLEGRO_KEY_UP)) {
        dirY -= 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_S) || engine->isKeyDown(ALLEGRO_KEY_DOWN)) {
        dirY += 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_A) || engine->isKeyDown(ALLEGRO_KEY_LEFT)) {
        dirX -= 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_D) || engine->isKeyDown(ALLEGRO_KEY_RIGHT)) {
        dirX += 1.0f;
    }

    // Logujemy tylko gdy faktycznie jest ruch
    if (dirX != 0.0f || dirY != 0.0f) {
        float length = sqrt(dirX * dirX + dirY * dirY);
        dirX /= length;
        dirY /= length;
        isMoving = true;
        if (logMovement) { logger.info("Movement input - Dir: X=" + std::to_string(dirX) + " Y=" + std::to_string(dirY)); }
    }
    else {
        isMoving = false;
    }

    // Aktualizacja prêdkoœci z przyspieszeniem
    if (isMoving) {
        velocity.setX(velocity.getX() + dirX * acceleration * deltaTime);
        velocity.setY(velocity.getY() + dirY * acceleration * deltaTime);

        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > maxSpeed) {
            float scale = maxSpeed / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
        if (logMovement) { logger.info("Velocity updated - V: X=" + std::to_string(velocity.getX()) + " Y=" + std::to_string(velocity.getY())); }
    }
    else {
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > 0) {
            float scale = std::max(0.0f, speed - deceleration * deltaTime) / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
    }

    // Obs³uga strzelania
    if (engine->isMouseButtonDown(0) && canShoot()) {
		shoot(logger);
    }

    // Obs³uga prze³adowania
    if (engine->isKeyDown(ALLEGRO_KEY_R)) {
        reload();
        if (isReloading) {
            logger.info("Reload started. Total ammo: " + std::to_string(totalAmmo));
        }
    }


    currentCooldown -= deltaTime;
}

void Player::lookAtMouse() {
    if (!camera) return;

    Engine* engine = Engine::getInstance();
    Point2D mouseScreen(engine->getMouseX(), engine->getMouseY());
    Point2D mouseWorld = camera->screenToWorld(mouseScreen);

    float dx = mouseWorld.getX() - position.getX();
    float dy = mouseWorld.getY() - position.getY();
    rotation = atan2(dy, dx);
}

void Player::update(float deltaTime) {
    if (!isActive) return;

    // Aktualizacja prze³adowania
    if (isReloading) {
        currentReloadTime += deltaTime;
        if (sprite && sprite->getCurrentAnimation()) {
            Engine::getInstance()->getLogger().info(
                "Reload progress: " + std::to_string(currentReloadTime) + "/" +
                std::to_string(reloadTime) + " Animation: " +
                sprite->getCurrentAnimation()->getName()
            );
        }

        if (currentReloadTime >= reloadTime) {
            // Zakoñcz prze³adowanie
            int ammoNeeded = maxAmmoInMag - currentAmmoInMag;
            int ammoToLoad = std::min(ammoNeeded, totalAmmo);

            currentAmmoInMag += ammoToLoad;
            totalAmmo -= ammoToLoad;

            isReloading = false;
            currentReloadTime = 0.0f;

            // Prze³¹cz z powrotem na idle
            if (sprite) {
                sprite->stopAnimation();
                sprite->playAnimation("idle");
                Engine::getInstance()->getLogger().info("Reload complete, switching to idle");
            }
        }
    }

    // Obs³uga sterowania
    handleInput(deltaTime);
    lookAtMouse();

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

void Player::shoot(Logger& logger) {
    // Offset od œrodka postaci do lufy pistoletu gdy postaæ patrzy w prawo
    const float MUZZLE_OFFSET_X = 65.0f;
    const float MUZZLE_OFFSET_Y = 42.0f;

    // Obliczamy pozycjê lufy z uwzglêdnieniem rotacji postaci
    Point2D bulletPos = position;
    float rotatedMuzzleX = MUZZLE_OFFSET_X * cos(rotation) - MUZZLE_OFFSET_Y * sin(rotation);
    float rotatedMuzzleY = MUZZLE_OFFSET_X * sin(rotation) + MUZZLE_OFFSET_Y * cos(rotation);

    bulletPos.setX(bulletPos.getX() + rotatedMuzzleX);
    bulletPos.setY(bulletPos.getY() + rotatedMuzzleY);

    // Oblicz now¹ rotacjê od punktu lufy do gracza
    Engine* engine = Engine::getInstance();
    Point2D mouseScreen(engine->getMouseX(), engine->getMouseY());
    Point2D mouseWorld = camera->screenToWorld(mouseScreen);
    float dx = mouseWorld.getX() - bulletPos.getX();
    float dy = mouseWorld.getY() - bulletPos.getY();
    float bulletRotation = atan2(dy, dx);

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

    // Dodaj debug log
    Engine::getInstance()->getLogger().info("Starting reload animation");

    // Upewnij siê, ¿e animacja jest prawid³owo prze³¹czana
    if (sprite) {
        sprite->stopAnimation(); // Zatrzymaj aktualn¹ animacjê
        sprite->playAnimation("reload");

        // Debug - sprawdŸ czy animacja siê zmieni³a
        if (Animation* current = sprite->getCurrentAnimation()) {
            Engine::getInstance()->getLogger().info("Current animation: " + current->getName());
        }
    }
}

bool Player::canShoot(){
    return currentAmmoInMag > 0 && !isReloading && currentCooldown <= 0;
}
