// src/game/Map.cpp
#include "Map.h"
#include "../engine/Engine.h"

Map::Map(int width, int height, int tileSize)
    : mapTexture(nullptr)
    , width(width)
    , height(height)
    , tileSize(tileSize)
    , wallColor(al_map_rgb(100, 100, 150))  // Kolor œcian/przeszkód
{
    initializeMapObjects();
}

Map::~Map() {
    if (mapTexture) {
        al_destroy_bitmap(mapTexture);
    }
}

bool Map::init(PrimitiveRenderer* renderer) {
    // Tworzymy teksturê o rozmiarze mapy
    mapTexture = al_create_bitmap(width * tileSize, height * tileSize);
    if (!mapTexture) return false;

    // Zapisujemy aktualny target
    ALLEGRO_BITMAP* prevTarget = al_get_target_bitmap();

    // Ustawiamy teksturê mapy jako cel renderowania
    al_set_target_bitmap(mapTexture);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Generujemy mapê
    generateMapTexture(renderer);

    // Przywracamy poprzedni target
    al_set_target_bitmap(prevTarget);

    return true;
}

void Map::drawGrid(PrimitiveRenderer* renderer) {
    renderer->setColor(al_map_rgb(50, 50, 50));

    // Linie pionowe
    for (int x = 0; x <= width; x++) {
        int pixelX = x * tileSize;
        renderer->drawLine(Point2D(pixelX, 0),
            Point2D(pixelX, height * tileSize));
    }

    // Linie poziome
    for (int y = 0; y <= height; y++) {
        int pixelY = y * tileSize;
        renderer->drawLine(Point2D(0, pixelY),
            Point2D(width * tileSize, pixelY));
    }
}

void Map::initializeMapObjects() {
    // Œrodkowa przeszkoda
    Rectangle centerWall(
        Point2D((width * tileSize) / 2 - 300, (height * tileSize) / 2 - 150),
        600, 300
    );
    colliders.push_back(centerWall);

    // Przeszkody w rogach
    // Lewy górny róg
    Rectangle topLeft(Point2D(200, 200), 300, 300);
    colliders.push_back(topLeft);

    // Prawy górny róg
    Rectangle topRight(
        Point2D(width * tileSize - 500, 200),
        300, 300
    );
    colliders.push_back(topRight);

    // Lewy dolny róg
    Rectangle bottomLeft(
        Point2D(200, height * tileSize - 500),
        300, 300
    );
    colliders.push_back(bottomLeft);

    // Prawy dolny róg
    Rectangle bottomRight(
        Point2D(width * tileSize - 500, height * tileSize - 500),
        300, 300
    );
    colliders.push_back(bottomRight);

    // Dodanie kilku mniejszych przeszkód
    for (int i = 0; i < 5; i++) {
        Rectangle obstacle(
            Point2D(
                (width * tileSize) / 6 + i * 600,
                (height * tileSize) / 2 + (i % 2 == 0 ? -400 : 400)
            ),
            200, 200
        );
        colliders.push_back(obstacle);
    }
}

void Map::drawMapObjects(PrimitiveRenderer* renderer) {
    renderer->setColor(wallColor);

    // Rysowanie wszystkich obiektów z kolizj¹
    for (const auto& collider : colliders) {
        // Rysowanie wype³nionego prostok¹ta
        renderer->drawRectangle(
            collider.getTopLeft(),
            collider.getWidth(),
            collider.getHeight(),
            true  // filled
        );

        // Rysowanie obramowania
        renderer->setColor(al_map_rgb(150, 150, 200));
        renderer->drawRectangle(
            collider.getTopLeft(),
            collider.getWidth(),
            collider.getHeight(),
            false  // not filled, only outline
        );
        renderer->setColor(wallColor);
    }
}


void Map::generateMapTexture(PrimitiveRenderer* renderer) {
    // Najpierw rysujemy siatkê
    drawGrid(renderer);

    // Nastêpnie rysujemy obiekty mapy
    drawMapObjects(renderer);
}
void Map::draw() {
    if (!mapTexture) return;

    al_draw_bitmap(mapTexture, 0, 0, 0);
}

void Map::addCollider(const Rectangle& collider) {
    colliders.push_back(collider);
}

bool Map::checkCollision(const Point2D& position, float radius) const {
    // Sprawdzanie kolizji z granicami mapy
    if (position.getX() - radius < 0 ||
        position.getX() + radius > width * tileSize ||
        position.getY() - radius < 0 ||
        position.getY() + radius > height * tileSize) {
        return true;
    }

    // Sprawdzanie kolizji z przeszkodami
    for (const auto& collider : colliders) {
        Point2D topLeft = collider.getTopLeft();

        if (position.getX() + radius > topLeft.getX() &&
            position.getX() - radius < topLeft.getX() + collider.getWidth() &&
            position.getY() + radius > topLeft.getY() &&
            position.getY() - radius < topLeft.getY() + collider.getHeight()) {
            return true;
        }
    }

    return false;
}

