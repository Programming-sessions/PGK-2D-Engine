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
    bool isMoving;
    Collision* collision;

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
    void setDetectionRange(float range);

    // Gettery
    bool isAlive() const;
    Collision* getCollision() const { return collision; }
};

#endif // ENEMY_H
