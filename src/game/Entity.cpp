// src/game/Entity.cpp
#include "Entity.h"
#include <cmath>
#include "DeadBody.h"
#include "BodyManager.h"

Entity::Entity()
    : sprite(nullptr)
    , position(0.0f, 0.0f)
    , velocity(0.0f, 0.0f)
    , rotation(0.0f)
	, health(15.0f)
    , speed(0.0f)
    , isActive(true)
    , tag("")
{
    sprite = new Sprite();
}

Entity::~Entity() {
    destroy();
}

void Entity::update(float deltaTime) {
    if (!isActive) return;

    // Aktualizacja pozycji na podstawie prêdkoœci
    position.setX(position.getX() + velocity.getX() * deltaTime);
    position.setY(position.getY() + velocity.getY() * deltaTime);

    // Aktualizacja sprite'a
    if (sprite) {
        sprite->setPosition(position.getX(), position.getY());
        sprite->setRotation(rotation);
        sprite->updateAnimation(deltaTime);
    }
}

void Entity::draw() {
    if (!isActive) return;

    if (sprite) {
        sprite->draw();
    }
}

void Entity::destroy() {
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }
    isActive = false;
}

void Entity::takeDamage(float damage) {
    health -= damage;

    if (health <= 0) {
        BodyManager::getInstance()->addBody(position, rotation, tag);
        destroy();
    }
}


bool Entity::loadResources() {
    // Podstawowa implementacja - do nadpisania w klasach pochodnych
    return true;
}

// Settery
void Entity::setPosition(float x, float y) {
    position.setPosition(x, y);
    if (sprite) {
        sprite->setPosition(x, y);
    }
}

void Entity::setRotation(float angle) {
    rotation = angle;
    if (sprite) {
        sprite->setRotation(angle);
    }
}

void Entity::setVelocity(float x, float y) {
    velocity.setPosition(x, y);
}

void Entity::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Entity::setActive(bool active) {
    isActive = active;
}

void Entity::setTag(const std::string& newTag) {
    tag = newTag;
}
void Entity::setHealth(float newHealth) {
	health = newHealth;
}

void Entity::setMaxHealth(float newMaxHealth) {
	maxHealth = newMaxHealth;
}


// Gettery
Point2D Entity::getPosition() const {
    return position;
}

Point2D Entity::getVelocity() const {
    return velocity;
}

float Entity::getRotation() const {
    return rotation;
}

float Entity::getSpeed() const {
    return speed;
}

bool Entity::getIsActive() const {
    return isActive;
}

std::string Entity::getTag() const {
    return tag;
}
float Entity::getHealth() const {
    return health;
}

float Entity::getMaxHealth() const {
	return maxHealth;
}