#include "BodyManager.h"

BodyManager* BodyManager::instance = nullptr;

BodyManager* BodyManager::getInstance() {
    if (!instance) {
        instance = new BodyManager();
    }
    return instance;
}

void BodyManager::releaseInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

BodyManager::~BodyManager() {
    cleanup();
}

void BodyManager::addBody(const Point2D& position, float rotation, const std::string& entityTag) {
    DeadBody* body = new DeadBody(position, rotation, entityTag);
    bodies.push_back(body);
}

void BodyManager::draw() {
    for (auto body : bodies) {
        body->draw();
    }
}

void BodyManager::cleanup() {
    for (auto body : bodies) {
        delete body;
    }
    bodies.clear();
}
