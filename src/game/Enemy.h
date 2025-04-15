#pragma
#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Player.h"
#include "Map.h"

class Enemy : public Entity {
private:
    Map* gameMap;
    Player* targetPlayer;

    float detectionRange;     // Zasiêg wykrywania gracza
    float maxSpeed;           // Maksymalna prêdkoœæ
    float acceleration;       // Przyspieszenie
    float deceleration;       // Hamowanie
    float health;
    float maxHealth;
    bool isMoving;

    // Pomocnicze metody AI
    void updateAI(float deltaTime);
    bool isPlayerInRange() const;
    void rotateTowardsPlayer();
    void moveTowardsPlayer(float deltaTime);

public:
    Enemy();
    virtual ~Enemy();

    // Nadpisane metody z klasy Entity
    virtual void update(float deltaTime) override;
    virtual bool loadResources() override;

    // Settery
    void setMap(Map* map);
    void setTarget(Player* player);
    void takeDamage(float amount);
    void setDetectionRange(float range);

    // Gettery
    float getHealth() const;
    float getMaxHealth() const;
    bool isAlive() const;
};

#endif // ENEMY_H
