// src/game/Map.h
#ifndef MAP_H
#define MAP_H

#include <vector>
#include "../engine/Primitives.h"
#include "../engine/PrimitiveRenderer.h"
#include <allegro5/allegro.h>

class Map {
private:
    ALLEGRO_BITMAP* mapTexture;
    std::vector<Rectangle> colliders;
    int width;
    int height;
    int tileSize;

    void generateMapTexture(PrimitiveRenderer* renderer);
    void drawGrid(PrimitiveRenderer* renderer);

    void initializeMapObjects();
    void drawMapObjects(PrimitiveRenderer* renderer);
    ALLEGRO_COLOR wallColor;
public:
    Map(int width, int height, int tileSize);
    ~Map();

    bool init(PrimitiveRenderer* renderer);
    void draw();
    void addCollider(const Rectangle& collider);
    bool checkCollision(const Point2D& position, float radius) const;

    // Gettery
    int getWidth() const { return width * tileSize; }
    int getHeight() const { return height * tileSize; }
    int getTileSize() const { return tileSize; }
};

#endif // MAP_H
