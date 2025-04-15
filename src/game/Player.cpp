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

    // Próba ruchu w osi X
    position.setX(position.getX() + velocity.getX() * deltaTime);
    if (gameMap && gameMap->checkCollision(position, collisionRadius)) {
        position.setX(previousPosition.getX());  // Cofnij tylko X
        velocity.setX(0.0f);  // Zatrzymaj ruch w osi X
    }

    // Próba ruchu w osi Y
    position.setY(position.getY() + velocity.getY() * deltaTime);
    if (gameMap && gameMap->checkCollision(position, collisionRadius)) {
        position.setY(previousPosition.getY());  // Cofnij tylko Y
        velocity.setY(0.0f);  // Zatrzymaj ruch w osi Y
    }

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
