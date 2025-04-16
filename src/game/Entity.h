#pragma once
// src/game/Entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include "../engine/Sprite.h"
#include "../engine/Primitives.h"
#include <string>

class Entity {
protected:
    Sprite* sprite;
    Point2D position;
    Point2D velocity;
    float rotation;
    float speed;
    bool isActive;
    std::string tag;


public:
    Entity();
    virtual ~Entity();

    // Podstawowe funkcje
    virtual void update(float deltaTime);
    virtual void draw();
    virtual void destroy();

    // Settery
    void setPosition(float x, float y);
    void setRotation(float angle);
    void setVelocity(float x, float y);
    void setSpeed(float newSpeed);
    void setActive(bool active);
    void setTag(const std::string& newTag);
    void setCollisionRadius(float radius);

    // Gettery
    Point2D getPosition() const;
    Point2D getVelocity() const;
    float getRotation() const;
    float getSpeed() const;
    bool getIsActive() const;
    std::string getTag() const;
    float getCollisionRadius() const;

    // £adowanie zasobów
    virtual bool loadResources();
};

#endif // ENTITY_H

