#ifndef MAP_H
#define MAP_H

#include <vector>
#include "../engine/Primitives.h"
#include "../engine/PrimitiveRenderer.h"
#include "CollisionManager.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

enum class WallShape {
    RECTANGLE,
    CIRCLE,
    TRIANGLE
};

class Wall {
public:
    WallShape shape;
    Rectangle bounds;
    Point2D center;      // dla ko³a
    float radius;        // dla ko³a
    std::vector<Point2D> points; // dla trójk¹ta
    Collision* collision;
    bool isBorder;

    // Konstruktor dla prostok¹ta
    Wall(const Rectangle& bounds, bool isBorder = false)
        : shape(WallShape::RECTANGLE), bounds(bounds), isBorder(isBorder), collision(nullptr) {
        collision = new Collision(CollisionShape::RECTANGLE, CollisionLayer::WALL);
        collision->setSize(bounds.getWidth(), bounds.getHeight());
        collision->setPosition(bounds.getTopLeft());
    }

    // Konstruktor dla ko³a
    Wall(const Point2D& center, float radius)
        : shape(WallShape::CIRCLE), center(center), radius(radius), isBorder(false), collision(nullptr) {
        bounds = Rectangle(Point2D(center.getX() - radius, center.getY() - radius), radius * 2, radius * 2);
        collision = new Collision(CollisionShape::CIRCLE, CollisionLayer::WALL);
        collision->setRadius(radius);
        collision->setPosition(center);
    }

    // Konstruktor dla trójk¹ta
    Wall(const Point2D& p1, const Point2D& p2, const Point2D& p3)
        : shape(WallShape::TRIANGLE), isBorder(false), collision(nullptr) {
        points = { p1, p2, p3 };
        // Oblicz bounds dla trójk¹ta
        float minX = std::min({ p1.getX(), p2.getX(), p3.getX() });
        float minY = std::min({ p1.getY(), p2.getY(), p3.getY() });
        float maxX = std::max({ p1.getX(), p2.getX(), p3.getX() });
        float maxY = std::max({ p1.getY(), p2.getY(), p3.getY() });
        bounds = Rectangle(Point2D(minX, minY), maxX - minX, maxY - minY);

        collision = new Collision(CollisionShape::TRIANGLE, CollisionLayer::WALL);
        collision->setTrianglePoints(p1, p2, p3);
    }

    ~Wall() {
        delete collision;
    }
};

class Map {
private:
    std::vector<Wall*> walls;
    int width;
    int height;
    int tileSize;
    PrimitiveRenderer* renderer;
    ALLEGRO_COLOR wallColor;
    ALLEGRO_BITMAP* wallTexture;
    ALLEGRO_BITMAP* mapTexture;
    ALLEGRO_BITMAP* floorTexture;

    void initializeWalls();
    void createMapTexture();

public:
    Map(int width, int height, int tileSize);
    ~Map();

    bool init(PrimitiveRenderer* renderer);
    void draw();
    bool isInBounds(const Point2D& position) const;

    int getWidth() const { return width * tileSize; }
    int getHeight() const { return height * tileSize; }
    int getTileSize() const { return tileSize; }
};

#endif // MAP_H
