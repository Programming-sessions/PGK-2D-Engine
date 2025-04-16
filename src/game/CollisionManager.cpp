#include "CollisionManager.h"
#include <algorithm>
#include <cmath>

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

bool CollisionManager::checkPointTriangle(const Point2D& p, const Point2D& a, const Point2D& b, const Point2D& c) const {
    float d1 = sign(p, a, b);
    float d2 = sign(p, b, c);
    float d3 = sign(p, c, a);

    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}

float CollisionManager::sign(const Point2D& p1, const Point2D& p2, const Point2D& p3) const {
    return (p1.getX() - p3.getX()) * (p2.getY() - p3.getY()) -
        (p2.getX() - p3.getX()) * (p1.getY() - p3.getY());
}

bool CollisionManager::checkCircleCircle(const Collision* a, const Collision* b) const {
    float dx = a->getPosition().getX() - b->getPosition().getX();
    float dy = a->getPosition().getY() - b->getPosition().getY();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = a->getRadius() + b->getRadius();
    return distanceSquared <= radiusSum * radiusSum;
}

bool CollisionManager::checkCircleRectangle(const Collision* circle, const Collision* rect) const {
    float circleX = circle->getPosition().getX();
    float circleY = circle->getPosition().getY();
    float rectX = rect->getPosition().getX();
    float rectY = rect->getPosition().getY();
    float rectW = rect->getWidth();
    float rectH = rect->getHeight();

    float testX = circleX;
    float testY = circleY;

    if (circleX < rectX) testX = rectX;
    else if (circleX > rectX + rectW) testX = rectX + rectW;

    if (circleY < rectY) testY = rectY;
    else if (circleY > rectY + rectH) testY = rectY + rectH;

    float distX = circleX - testX;
    float distY = circleY - testY;
    float distanceSquared = (distX * distX) + (distY * distY);

    return distanceSquared <= (circle->getRadius() * circle->getRadius());
}

bool CollisionManager::checkRectangleRectangle(const Collision* a, const Collision* b) const {
    return !(a->getPosition().getX() + a->getWidth() <= b->getPosition().getX() ||
        b->getPosition().getX() + b->getWidth() <= a->getPosition().getX() ||
        a->getPosition().getY() + a->getHeight() <= b->getPosition().getY() ||
        b->getPosition().getY() + b->getHeight() <= a->getPosition().getY());
}

bool CollisionManager::checkCircleTriangle(const Collision* circle, const Collision* triangle) const {
    const auto& points = triangle->getTrianglePoints();
    const Point2D& circleCenter = circle->getPosition();
    float radius = circle->getRadius();

    // Check if circle center is inside triangle
    if (checkPointTriangle(circleCenter, points[0], points[1], points[2])) {
        return true;
    }

    // Check collision with each edge of the triangle
    for (int i = 0; i < 3; i++) {
        const Point2D& a = points[i];
        const Point2D& b = points[(i + 1) % 3];

        float ab_x = b.getX() - a.getX();
        float ab_y = b.getY() - a.getY();
        float t = ((circleCenter.getX() - a.getX()) * ab_x +
            (circleCenter.getY() - a.getY()) * ab_y) /
            (ab_x * ab_x + ab_y * ab_y);
        t = std::max(0.0f, std::min(1.0f, t));

        Point2D closest(
            a.getX() + t * ab_x,
            a.getY() + t * ab_y
        );

        float dx = circleCenter.getX() - closest.getX();
        float dy = circleCenter.getY() - closest.getY();
        if (dx * dx + dy * dy <= radius * radius) {
            return true;
        }
    }

    return false;
}

bool CollisionManager::checkTriangleRectangle(const Collision* triangle, const Collision* rectangle) const {
    const auto& points = triangle->getTrianglePoints();
    Point2D rectPos = rectangle->getPosition();
    float rectWidth = rectangle->getWidth();
    float rectHeight = rectangle->getHeight();

    // Check if any triangle vertex is inside rectangle
    for (const auto& p : points) {
        if (p.getX() >= rectPos.getX() && p.getX() <= rectPos.getX() + rectWidth &&
            p.getY() >= rectPos.getY() && p.getY() <= rectPos.getY() + rectHeight) {
            return true;
        }
    }

    // Check if any rectangle vertex is inside triangle
    std::vector<Point2D> rectPoints = {
        rectPos,
        Point2D(rectPos.getX() + rectWidth, rectPos.getY()),
        Point2D(rectPos.getX() + rectWidth, rectPos.getY() + rectHeight),
        Point2D(rectPos.getX(), rectPos.getY() + rectHeight)
    };

    for (const auto& p : rectPoints) {
        if (checkPointTriangle(p, points[0], points[1], points[2])) {
            return true;
        }
    }

    // Check for line segment intersections
    for (int i = 0; i < 3; i++) {
        const Point2D& a = points[i];
        const Point2D& b = points[(i + 1) % 3];
        for (int j = 0; j < 4; j++) {
            const Point2D& c = rectPoints[j];
            const Point2D& d = rectPoints[(j + 1) % 4];

            float denominator = ((b.getX() - a.getX()) * (d.getY() - c.getY())) -
                ((b.getY() - a.getY()) * (d.getX() - c.getX()));
            if (denominator == 0) continue;

            float ua = (((d.getX() - c.getX()) * (a.getY() - c.getY())) -
                ((d.getY() - c.getY()) * (a.getX() - c.getX()))) / denominator;
            float ub = (((b.getX() - a.getX()) * (a.getY() - c.getY())) -
                ((b.getY() - a.getY()) * (a.getX() - c.getX()))) / denominator;

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

bool CollisionManager::checkCollision(const Collision* collision, const Point2D& position) const {
    if (!collision || !collision->isActive()) return false;

    Point2D oldPos = collision->getPosition();
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
