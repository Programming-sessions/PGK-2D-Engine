#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "../engine/Engine.h"
#include "../game/Camera.h"
#include "CollisionManager.h"
#include "BulletManager.h"
#include "../engine/Logger.h"
#include "Map.h"

class Player : public Entity {
private:
    float maxSpeed;
    float acceleration;
    float deceleration;
    bool isMoving;
    float health;
    float maxHealth;
    float shootCooldown;
    float currentCooldown;
    Camera* camera;
    Map* gameMap;  // WskaŸnik na mapê
    Collision* collision;

    bool logMovement;

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
    void shoot();

    // Gettery
    float getHealth() const;
    float getMaxHealth() const;
    bool isAlive() const;
    Collision* getCollision() const { return collision; }

	// Settery
    void setCamera(Camera* cam);
    void setMap(Map* map);
};

#endif // PLAYER_H


