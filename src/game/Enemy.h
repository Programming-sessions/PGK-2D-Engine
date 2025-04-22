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
    float shootCooldown;
    float currentCooldown;
    float shootRange;      // Zasiêg z którego przeciwnik bêdzie strzela³
    bool canShoot;         // Flaga okreœlaj¹ca czy przeciwnik mo¿e strzelaæ

    Collision* collision;

	// Metody do obs³ugi AI
    float preferredDistance;  // Preferowana odleg³oœæ od gracza
    float distanceMargin;     // Margines odleg³oœci (¿eby nie byli sztywno na jednej odleg³oœci)
    Point2D lastKnownPlayerPosition;
    bool hasLastKnownPosition;
    float arrivalThreshold;  // Jak blisko punktu musi byæ przeciwnik ¿eby uznaæ ¿e dotar³
    float alertDuration;    // Jak d³ugo bot jest w stanie czujnoœci po trafieniu
    float currentAlertTime; // Obecny czas czujnoœci
    bool isAlerted;        // Czy bot jest w stanie czujnoœci


    // Pomocnicze metody AI
    void updateAI(float deltaTime);
    bool isPlayerInRange() const;
    void rotateTowardsPlayer();
    void moveTowardsPlayer(float deltaTime);
    bool hasLineOfSight() const;
    void updateLastKnownPlayerPosition(const Point2D& position);

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

    void shoot();
    void takeDamage(float damage);
    bool isPlayerInShootRange() const;
};

#endif // ENEMY_H
