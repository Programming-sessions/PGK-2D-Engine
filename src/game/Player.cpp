#include "Player.h"
#include <cmath>

Player::Player()
    : Entity()
    , maxSpeed(300.0f)  // Piksele na sekundê
    , acceleration(400.0f)
    , deceleration(800.0f)
    , isMoving(false)
    , health(100.0f)
    , maxHealth(100.0f)
    , shootCooldown(0.25f)  // 4 strza³y na sekundê
    , currentCooldown(0.0f)
	, logMovement(false)
{
    tag = "Player";
    collision = new Collision(CollisionShape::CIRCLE, CollisionLayer::ENTITY);
    collision->setRadius(30.0f);  // Dostosuj promieñ kolizji
    collision->setOwner(this);
}

Player::~Player() {
    delete collision;
}

bool Player::loadResources() {
    if (!sprite->loadTexture("assets/textures/hero_idle.png")) {
        return false;
    }

    // Tworzenie animacji idle
    Animation* idleAnimation = new Animation("idle", true);

    // Ustawienia klatek animacji
    const int FRAME_WIDTH = 180;
    const int FRAME_HEIGHT = 150;
    const float FRAME_DURATION = 0.05f;

    // Dodawanie klatek animacji (4 rzêdy po 5 klatek)
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
    if (engine->isMouseButtonDown(0) && currentCooldown <= 0) {
        // Offset od œrodka postaci do lufy pistoletu gdy postaæ patrzy w prawo
        const float MUZZLE_OFFSET_X = 80.0f;  // (170 - 90) wzglêdem œrodka
        const float MUZZLE_OFFSET_Y = 40.0f;  // (115 - 75) wzglêdem œrodka

        // Obliczamy pozycjê lufy z uwzglêdnieniem rotacji postaci
        Point2D bulletPos = position;
        float rotatedMuzzleX = MUZZLE_OFFSET_X * cos(rotation) - MUZZLE_OFFSET_Y * sin(rotation);
        float rotatedMuzzleY = MUZZLE_OFFSET_X * sin(rotation) + MUZZLE_OFFSET_Y * cos(rotation);

        bulletPos.setX(bulletPos.getX() + rotatedMuzzleX);
        bulletPos.setY(bulletPos.getY() + rotatedMuzzleY);

        BulletManager::getInstance()->createBullet(bulletPos, rotation, this);
        currentCooldown = shootCooldown;

        logger.info("Shot fired - Mouse state before shot: isMoving=" + std::to_string(isMoving));
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

void Player::takeDamage(float amount) {
    health = std::max(0.0f, health - amount);
}

void Player::heal(float amount) {
    health = std::min(maxHealth, health + amount);
}

float Player::getHealth() const {
    return health;
}

float Player::getMaxHealth() const {
    return maxHealth;
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
