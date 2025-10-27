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
    Map* gameMap; // Wskaźnik na mapę
    Collision* collision;

    bool logMovement;
    bool logReload;

    //Ammo
    int maxAmmoInMag;           // Pojemność magazynka
    int currentAmmoInMag;     // Aktualna ilość naboi w magazynku
    int totalAmmo;            // Całkowita ilość amunicji zapasowej
    float reloadTime;         // Czas przeładowania w sekundach
    float currentReloadTime;  // Licznik czasu przeładowania
    bool isReloading;         // Czy trwa przeładowanie

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
    void shoot();
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