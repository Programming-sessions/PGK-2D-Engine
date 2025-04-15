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
{
    tag = "Player";
    collisionRadius = 30.0f;  // Dostosuj wed³ug rozmiaru sprite'a
}

Player::~Player() {
}

bool Player::loadResources() {
    if (!sprite->loadTexture("assets/textures/hero_idle.png")) {
        return false;
    }

    // Tworzenie animacji idle
    Animation* idleAnimation = new Animation("idle", true);

    // Ustawienia klatek animacji
    const int FRAME_WIDTH = 253;
    const int FRAME_HEIGHT = 216;
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

    // Obliczanie wektora kierunku na podstawie wciœniêtych klawiszy
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

    // Normalizacja wektora kierunku
    if (dirX != 0.0f || dirY != 0.0f) {
        float length = sqrt(dirX * dirX + dirY * dirY);
        dirX /= length;
        dirY /= length;
        isMoving = true;
    }
    else {
        isMoving = false;
    }

    // Aktualizacja prêdkoœci z przyspieszeniem
    if (isMoving) {
        velocity.setX(velocity.getX() + dirX * acceleration * deltaTime);
        velocity.setY(velocity.getY() + dirY * acceleration * deltaTime);

        // Ograniczenie maksymalnej prêdkoœci
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > maxSpeed) {
            float scale = maxSpeed / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
    }
    else {
        // Hamowanie
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > 0) {
            float scale = std::max(0.0f, speed - deceleration * deltaTime) / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
    }
}

void Player::lookAtMouse() {
    Engine* engine = Engine::getInstance();

    float mouseX = engine->getMouseX();
    float mouseY = engine->getMouseY();

    // Obliczanie k¹ta miêdzy graczem a kursorem
    float dx = mouseX - position.getX();
    float dy = mouseY - position.getY();
    rotation = atan2(dy, dx);
}

void Player::update(float deltaTime) {
    if (!isActive) return;

    handleInput(deltaTime);
    lookAtMouse();

    // Aktualizacja pozycji
    Entity::update(deltaTime);
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
