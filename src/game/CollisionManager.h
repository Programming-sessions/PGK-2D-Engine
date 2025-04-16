// CollisionManager.h
#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "../engine/Primitives.h"

enum class CollisionShape {
    RECTANGLE,
    CIRCLE,
    TRIANGLE
};

enum class CollisionLayer {
    WALL,
    ENTITY,
    PROJECTILE
};

class Collision {
private:
    Point2D position;
    float radius;
    float width;
    float height;
    CollisionShape shape;
    CollisionLayer layer;
    bool active;
    std::vector<Point2D> trianglePoints;

public:
    Collision(CollisionShape shape = CollisionShape::CIRCLE,
        CollisionLayer layer = CollisionLayer::ENTITY)
        : position(0.0f, 0.0f)
        , radius(0.0f)
        , width(0.0f)
        , height(0.0f)
        , shape(shape)
        , layer(layer)
        , active(true) {
    }

    // Settery
    void setPosition(const Point2D& pos) { position = pos; }
    void setPosition(float x, float y) { position.setPosition(x, y); }
    void setRadius(float r) { radius = r; }
    void setSize(float w, float h) { width = w; height = h; }
    void setActive(bool isActive) { active = isActive; }
    void setShape(CollisionShape newShape) { shape = newShape; }
    void setLayer(CollisionLayer newLayer) { layer = newLayer; }
    void setTrianglePoints(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
        trianglePoints = { p1, p2, p3 };
    }

    // Gettery
    Point2D getPosition() const { return position; }
    float getRadius() const { return radius; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool isActive() const { return active; }
    CollisionShape getShape() const { return shape; }
    CollisionLayer getLayer() const { return layer; }
    const std::vector<Point2D>& getTrianglePoints() const { return trianglePoints; }
};

class CollisionManager {
private:
    static CollisionManager* instance;
    std::vector<Collision*> collisions;

    bool checkCircleCircle(const Collision* a, const Collision* b) const;
    bool checkCircleRectangle(const Collision* circle, const Collision* rect) const;
    bool checkRectangleRectangle(const Collision* a, const Collision* b) const;
    bool checkPointTriangle(const Point2D& p, const Point2D& a, const Point2D& b, const Point2D& c) const;
    bool checkCircleTriangle(const Collision* circle, const Collision* triangle) const;
    bool checkTriangleRectangle(const Collision* triangle, const Collision* rectangle) const;
    float sign(const Point2D& p1, const Point2D& p2, const Point2D& p3) const;

    CollisionManager() {}

public:
    static CollisionManager* getInstance();
    static void releaseInstance();
    void addCollision(Collision* collision);
    void removeCollision(Collision* collision);
    bool checkCollision(const Collision* a, const Collision* b) const;
    bool checkCollision(const Collision* collision, const Point2D& position) const;
    std::vector<Collision*> getCollisions(const Collision* collision) const;
    void clear();
};

#endif // COLLISION_MANAGER_H