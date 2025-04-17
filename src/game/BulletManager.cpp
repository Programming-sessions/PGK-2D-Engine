// BulletManager.cpp
#include "BulletManager.h"

BulletManager* BulletManager::instance = nullptr;

BulletManager* BulletManager::getInstance() {
    if (!instance) {
        instance = new BulletManager();
    }
    return instance;
}

void BulletManager::releaseInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

BulletManager::BulletManager() {
    bullets.reserve(MAX_BULLETS);
}

BulletManager::~BulletManager() {
    cleanup();
}

void BulletManager::update(float deltaTime) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        if ((*it)->getIsActive()) {
            (*it)->update(deltaTime);
            ++it;
        }
        else {
            delete* it;
            it = bullets.erase(it);
        }
    }
}

void BulletManager::draw() {
    for (auto bullet : bullets) {
        if (bullet->getIsActive()) {
            bullet->draw();
        }
    }
}

Bullet* BulletManager::createBullet(const Point2D& position, float rotation, Entity* shooter) {
    if (bullets.size() >= MAX_BULLETS) {
        return nullptr;
    }

    Bullet* bullet = new Bullet();
    bullet->initialize(position, rotation, shooter);
    bullets.push_back(bullet);
    return bullet;
}

void BulletManager::cleanup() {
    for (auto bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
}
