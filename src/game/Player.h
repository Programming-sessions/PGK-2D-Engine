#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "../engine/Engine.h"
#include "../game/Camera.h"

class Player : public Entity {
private:
    float maxSpeed;
    float acceleration;
    float deceleration;
    bool isMoving;
    float health;
    float maxHealth;
    Camera* camera;

public:
    Player();
    virtual ~Player();

    // Nadpisane metody z klasy Entity
    virtual void update(float deltaTime) override;
    virtual bool loadResources() override;

    // Specyficzne metody dla gracza
    void handleInput(float deltaTime);
    void lookAtMouse();
    void takeDamage(float amount);
    void heal(float amount);

    // Gettery
    float getHealth() const;
    float getMaxHealth() const;
    bool isAlive() const;

    void setCamera(Camera* cam);
};

#endif // PLAYER_H


