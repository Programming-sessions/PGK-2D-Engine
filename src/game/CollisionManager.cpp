#include "CollisionManager.h"
#include <algorithm>
#include <cmath>
#include <glm/gtx/norm.hpp>

CollisionManager* CollisionManager::instance = nullptr;

CollisionManager* CollisionManager::getInstance() {
    if (!instance) {
        instance = new CollisionManager();
    }
    return instance;
}

void CollisionManager::releaseInstance() {
    if (instance) {
        instance->clear();
        delete instance;
        instance = nullptr;
    }
}

void CollisionManager::addCollision(Collision* collision) {
    if (collision) {
        collisions.push_back(collision);
    }
}

void CollisionManager::removeCollision(Collision* collision) {
    auto it = std::find(collisions.begin(), collisions.end(), collision);
    if (it != collisions.end()) {
        collisions.erase(it);
    }
}

bool CollisionManager::checkPointTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) const {
    float d1 = sign(p, a, b);
    float d2 = sign(p, b, c);
    float d3 = sign(p, c, a);

    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}

float CollisionManager::sign(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) const {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool CollisionManager::checkCircleCircle(const Collision* a, const Collision* b) const {
    float distanceSquared = glm::distance2(a->getPosition(), b->getPosition());
    float radiusSum = a->getRadius() + b->getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

bool CollisionManager::checkCircleRectangle(const Collision* circle, const Collision* rect) const {
    glm::vec2 circlePos = circle->getPosition();
    glm::vec2 rectPos = rect->getPosition();
    float rectW = rect->getWidth();
    float rectH = rect->getHeight();

    float testX = circlePos.x;
    float testY = circlePos.y;

    if (circlePos.x < rectPos.x) testX = rectPos.x;
    else if (circlePos.x > rectPos.x + rectW) testX = rectPos.x + rectW;

    if (circlePos.y < rectPos.y) testY = rectPos.y;
    else if (circlePos.y > rectPos.y + rectH) testY = rectPos.y + rectH;

    float distX = circlePos.x - testX;
    float distY = circlePos.y - testY;
    float distanceSquared = (distX * distX) + (distY * distY);

    return distanceSquared <= (circle->getRadius() * circle->getRadius());
}

bool CollisionManager::checkRectangleRectangle(const Collision* a, const Collision* b) const {
    return !(a->getPosition().x + a->getWidth() <= b->getPosition().x ||
        b->getPosition().x + b->getWidth() <= a->getPosition().x ||
        a->getPosition().y + a->getHeight() <= b->getPosition().y ||
        b->getPosition().y + b->getHeight() <= a->getPosition().y);
}

bool CollisionManager::checkCircleTriangle(const Collision* circle, const Collision* triangle) const {
    const auto& points = triangle->getTrianglePoints();
    const glm::vec2& circleCenter = circle->getPosition();
    float radius = circle->getRadius();

    // Check if circle center is inside triangle
    if (checkPointTriangle(circleCenter, points[0], points[1], points[2])) {
        return true;
    }

    // Check collision with each edge of the triangle
    for (int i = 0; i < 3; i++) {
        const glm::vec2& a = points[i];
        const glm::vec2& b = points[(i + 1) % 3];

        glm::vec2 ab = b - a;
        float t = glm::dot(circleCenter - a, ab) / glm::dot(ab, ab);
        t = std::max(0.0f, std::min(1.0f, t));

        glm::vec2 closest = a + t * ab;

        if (glm::distance2(circleCenter, closest) <= radius * radius) {
            return true;
        }
    }

    return false;
}

bool CollisionManager::checkTriangleRectangle(const Collision* triangle, const Collision* rectangle) const {
    const auto& points = triangle->getTrianglePoints();
    glm::vec2 rectPos = rectangle->getPosition();
    float rectWidth = rectangle->getWidth();
    float rectHeight = rectangle->getHeight();

    // Check if any triangle vertex is inside rectangle
    for (const auto& p : points) {
        if (p.x >= rectPos.x && p.x <= rectPos.x + rectWidth &&
            p.y >= rectPos.y && p.y <= rectPos.y + rectHeight) {
            return true;
        }
    }

    // Check if any rectangle vertex is inside triangle
    std::vector<glm::vec2> rectPoints = {
        rectPos,
        glm::vec2(rectPos.x + rectWidth, rectPos.y),
        glm::vec2(rectPos.x + rectWidth, rectPos.y + rectHeight),
        glm::vec2(rectPos.x, rectPos.y + rectHeight)
    };

    for (const auto& p : rectPoints) {
        if (checkPointTriangle(p, points[0], points[1], points[2])) {
            return true;
        }
    }

    // Check for line segment intersections
    for (int i = 0; i < 3; i++) {
        const glm::vec2& a = points[i];
        const glm::vec2& b = points[(i + 1) % 3];
        for (int j = 0; j < 4; j++) {
            const glm::vec2& c = rectPoints[j];
            const glm::vec2& d = rectPoints[(j + 1) % 4];

            float denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
            if (denominator == 0) continue;

            float ua = (((d.x - c.x) * (a.y - c.y)) - ((d.y - c.y) * (a.x - c.x))) / denominator;
            float ub = (((b.x - a.x) * (a.y - c.y)) - ((b.y - a.y) * (a.x - c.x))) / denominator;

            if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1) {
                return true;
            }
        }
    }

    return false;
}

bool CollisionManager::checkCollision(const Collision* a, const Collision* b) const {
    if (!a || !b || !a->isActive() || !b->isActive()) return false;

    if (a->getShape() == CollisionShape::CIRCLE && b->getShape() == CollisionShape::CIRCLE) {
        return checkCircleCircle(a, b);
    }
    else if (a->getShape() == CollisionShape::CIRCLE && b->getShape() == CollisionShape::RECTANGLE) {
        return checkCircleRectangle(a, b);
    }
    else if (a->getShape() == CollisionShape::RECTANGLE && b->getShape() == CollisionShape::CIRCLE) {
        return checkCircleRectangle(b, a);
    }
    else if (a->getShape() == CollisionShape::RECTANGLE && b->getShape() == CollisionShape::RECTANGLE) {
        return checkRectangleRectangle(a, b);
    }
    else if (a->getShape() == CollisionShape::CIRCLE && b->getShape() == CollisionShape::TRIANGLE) {
        return checkCircleTriangle(a, b);
    }
    else if (a->getShape() == CollisionShape::TRIANGLE && b->getShape() == CollisionShape::CIRCLE) {
        return checkCircleTriangle(b, a);
    }
    else if (a->getShape() == CollisionShape::TRIANGLE && b->getShape() == CollisionShape::RECTANGLE) {
        return checkTriangleRectangle(a, b);
    }
    else if (a->getShape() == CollisionShape::RECTANGLE && b->getShape() == CollisionShape::TRIANGLE) {
        return checkTriangleRectangle(b, a);
    }

    return false;
}

bool CollisionManager::checkCollision(const Collision* collision, const glm::vec2& position) const {
    if (!collision || !collision->isActive()) return false;

    glm::vec2 oldPos = collision->getPosition();
    const_cast<Collision*>(collision)->setPosition(position);

    bool collided = false;
    for (const auto& other : collisions) {
        if (other != collision && other->isActive()) {
            if (checkCollision(collision, other)) {
                collided = true;
                break;
            }
        }
    }

    const_cast<Collision*>(collision)->setPosition(oldPos);
    return collided;
}

std::vector<Collision*> CollisionManager::getCollisions(const Collision* collision) const {
    std::vector<Collision*> result;
    if (!collision || !collision->isActive()) return result;

    for (auto& other : collisions) {
        if (other != collision && other->isActive()) {
            if (checkCollision(collision, other)) {
                result.push_back(other);
            }
        }
    }

    return result;
}

void CollisionManager::clear() {
    collisions.clear();
}
