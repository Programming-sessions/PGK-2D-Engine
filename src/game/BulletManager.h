// BulletManager.h
#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <vector>
#include "Bullet.h"
#include "../engine/Logger.h"

class BulletManager {
private:
    static BulletManager* instance;
    static const int MAX_BULLETS = 100;
    std::vector<Bullet*> bullets;

    // Prywatny konstruktor i destruktor (singleton)
    BulletManager();
    ~BulletManager();

    // Zablokowanie kopiowania
    BulletManager(const BulletManager&) = delete;
    BulletManager& operator=(const BulletManager&) = delete;

public:
    static BulletManager* getInstance();
    static void releaseInstance();

    void update(float deltaTime);
    void draw();
    Bullet* createBullet(const Point2D& position, float rotation, Entity* shooter);
    void cleanup();
};

#endif
