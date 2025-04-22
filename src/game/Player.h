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
    float shootCooldown;
    float currentCooldown;
    Camera* camera;
    Map* gameMap;  // Wska�nik na map�
    Collision* collision;

    bool logMovement;

    //Ammo
    int maxAmmoInMag;        // Pojemno�� magazynka
    int currentAmmoInMag;   // Aktualna ilo�� naboi w magazynku
    int totalAmmo;           // Ca�kowita ilo�� amunicji zapasowej
    float reloadTime;     // Czas prze�adowania w sekundach
    float currentReloadTime;;// Licznik czasu prze�adowania
    bool isReloading;    // Czy trwa prze�adowanie

public:
    Player();
    virtual ~Player();

    // Nadpisane metody z klasy Entity
    virtual void update(float deltaTime) override;
    virtual bool loadResources() override;

    // Specyficzne metody dla gracza
    void handleInput(float deltaTime);
    void lookAtMouse();
    void heal(float amount);
    void shoot(Logger& logger);
    bool canShoot();
    void reload();

    // Gettery
    bool isAlive() const;
    int getCurrentAmmo() const { return currentAmmoInMag; }
    int getTotalAmmo() const { return totalAmmo; }
    bool getIsReloading() const { return isReloading; }
    float getMaxHealth() const { return maxHealth; }
    float getCurrentReloadTime() const { return currentReloadTime; }
    float getReloadTime() const { return reloadTime; }
    Collision* getCollision() const { return collision; }

	// Settery
    void setCamera(Camera* cam);
    void setMap(Map* map);
};

#endif // PLAYER_H


