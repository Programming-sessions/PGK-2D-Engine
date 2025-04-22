#include "Map.h"
#include <allegro5/allegro_primitives.h>

Map::Map(int width, int height, int tileSize)
    : width(width)
    , height(height)
    , tileSize(tileSize)
    , renderer(nullptr)
    , wallColor(al_map_rgb(100, 100, 150))
    , wallTexture(nullptr)
    , mapTexture(nullptr)
    , floorTexture(nullptr)  // inicjalizacja nowego pola
{
}

Map::~Map() {
    for (auto wall : walls) {
        if (wall->collision) {
            CollisionManager::getInstance()->removeCollision(wall->collision);
        }
        delete wall;
    }
    walls.clear();

    if (mapTexture) {
        al_destroy_bitmap(mapTexture);
    }
    if (floorTexture) {
        al_destroy_bitmap(floorTexture);
    }
}

void Map::initializeWalls() {
    const int BRICK_SIZE = 64;

    // Œciany graniczne (bez zmian)
    walls.push_back(new Wall(Rectangle(Point2D(0, 0), width * tileSize, BRICK_SIZE), true));
    walls.push_back(new Wall(Rectangle(Point2D(0, height * tileSize - BRICK_SIZE), width * tileSize, BRICK_SIZE), true));
    walls.push_back(new Wall(Rectangle(Point2D(0, 0), BRICK_SIZE, height * tileSize), true));
    walls.push_back(new Wall(Rectangle(Point2D(width * tileSize - BRICK_SIZE, 0), BRICK_SIZE, height * tileSize), true));

    // Œrodkowa przeszkoda - ko³o
    walls.push_back(new Wall(
        Point2D(width * tileSize / 2, height * tileSize / 2),
        200.0f
    ));

    // Przeszkody w rogach - trójk¹ty
    walls.push_back(new Wall(
        Point2D(200, 200),
        Point2D(500, 200),
        Point2D(350, 500)
    ));

    walls.push_back(new Wall(
        Point2D(width * tileSize - 200, 200),
        Point2D(width * tileSize - 500, 200),
        Point2D(width * tileSize - 350, 500)
    ));

    // Dodatkowe przeszkody - mieszanka kszta³tów
    // Prostok¹ty
    walls.push_back(new Wall(Rectangle(Point2D(400, height * tileSize - 700), 300, 200)));
    walls.push_back(new Wall(Rectangle(Point2D(width * tileSize - 700, height * tileSize - 700), 300, 200)));

    // Ko³a
    walls.push_back(new Wall(Point2D(800, 800), 150.0f));
    walls.push_back(new Wall(Point2D(width * tileSize - 800, 800), 150.0f));

    // Trójk¹ty
    walls.push_back(new Wall(
        Point2D(width * tileSize / 2 - 300, height * tileSize / 2 + 500),
        Point2D(width * tileSize / 2 + 300, height * tileSize / 2 + 500),
        Point2D(width * tileSize / 2, height * tileSize / 2 + 200)
    ));

    // Dodaj kolizje
    for (auto wall : walls) {
        CollisionManager::getInstance()->addCollision(wall->collision);
    }
}

void Map::createMapTexture() {
    // Za³aduj tekstury
    wallTexture = al_load_bitmap("assets/textures/wall_brick.png");
    floorTexture = al_load_bitmap("assets/textures/floor.png");
    if (!wallTexture || !floorTexture) return;

    // Stwórz teksturê mapy
    mapTexture = al_create_bitmap(width * tileSize, height * tileSize);
    if (!mapTexture) return;

    // Zachowaj obecny target
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

    // Ustaw target na now¹ teksturê mapy
    al_set_target_bitmap(mapTexture);

    // Wyczyœæ t³o
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    // Renderuj pod³ogê
    const int FLOOR_SIZE = 512; // rozmiar tekstury pod³ogi
    for (int x = 0; x < width * tileSize; x += FLOOR_SIZE) {
        for (int y = 0; y < height * tileSize; y += FLOOR_SIZE) {
            // Oblicz rozmiar ostatniej p³ytki jeœli nie mieœci siê ca³a
            int drawWidth = std::min(FLOOR_SIZE, (width * tileSize) - x);
            int drawHeight = std::min(FLOOR_SIZE, (height * tileSize) - y);

            if (drawWidth != FLOOR_SIZE || drawHeight != FLOOR_SIZE) {
                // Dla krawêdzi mapy - rysuj tylko czêœæ tekstury
                al_draw_scaled_bitmap(floorTexture,
                    0, 0,                    // source x, y
                    FLOOR_SIZE, FLOOR_SIZE,  // source width, height
                    x, y,                    // dest x, y
                    drawWidth, drawHeight,   // dest width, height
                    0                        // flags
                );
            }
            else {
                // Standardowe rysowanie pe³nej tekstury
                al_draw_bitmap(floorTexture, x, y, 0);
            }
        }
    }

    for (const auto wall : walls) {
        if (wall->isBorder) {
            // Narysuj teksturowan¹ œcianê (bez zmian)
            const Rectangle& bounds = wall->bounds;
            for (float x = bounds.getTopLeft().getX(); x < bounds.getTopLeft().getX() + bounds.getWidth(); x += 64) {
                for (float y = bounds.getTopLeft().getY(); y < bounds.getTopLeft().getY() + bounds.getHeight(); y += 64) {
                    al_draw_bitmap(wallTexture, x, y, 0);
                }
            }
        }
        else {
            // Narysuj przeszkodê w zale¿noœci od kszta³tu
            switch (wall->shape) {
            case WallShape::RECTANGLE: {
                const Rectangle& bounds = wall->bounds;
                al_draw_filled_rectangle(
                    bounds.getTopLeft().getX(),
                    bounds.getTopLeft().getY(),
                    bounds.getTopLeft().getX() + bounds.getWidth(),
                    bounds.getTopLeft().getY() + bounds.getHeight(),
                    wallColor
                );
                al_draw_rectangle(
                    bounds.getTopLeft().getX(),
                    bounds.getTopLeft().getY(),
                    bounds.getTopLeft().getX() + bounds.getWidth(),
                    bounds.getTopLeft().getY() + bounds.getHeight(),
                    al_map_rgb(150, 150, 200),
                    2.0
                );
                break;
            }
            case WallShape::CIRCLE: {
                al_draw_filled_circle(
                    wall->center.getX(),
                    wall->center.getY(),
                    wall->radius,
                    wallColor
                );
                al_draw_circle(
                    wall->center.getX(),
                    wall->center.getY(),
                    wall->radius,
                    al_map_rgb(150, 150, 200),
                    2.0
                );
                break;
            }
            case WallShape::TRIANGLE: {
                float vertices[] = {
                    wall->points[0].getX(), wall->points[0].getY(),
                    wall->points[1].getX(), wall->points[1].getY(),
                    wall->points[2].getX(), wall->points[2].getY()
                };
                al_draw_filled_triangle(
                    vertices[0], vertices[1],
                    vertices[2], vertices[3],
                    vertices[4], vertices[5],
                    wallColor
                );
                al_draw_triangle(
                    vertices[0], vertices[1],
                    vertices[2], vertices[3],
                    vertices[4], vertices[5],
                    al_map_rgb(150, 150, 200),
                    2.0
                );
                break;
            }
            }
        }
    }

    // Przywróæ poprzedni target
    al_set_target_bitmap(prev_target);

    // Zwolnij tekstury
    al_destroy_bitmap(wallTexture);
    al_destroy_bitmap(floorTexture);
    wallTexture = nullptr;
    floorTexture = nullptr;
}

bool Map::init(PrimitiveRenderer* rendererPtr) {
    if (!rendererPtr) return false;
    renderer = rendererPtr;

    initializeWalls();
    createMapTexture();
    return true;
}

void Map::draw() {
    if (!mapTexture) return;

    // Narysuj ca³¹ mapê jednym wywo³aniem
    al_draw_bitmap(mapTexture, 0, 0, 0);
}

bool Map::isInBounds(const Point2D& position) const {
    return position.getX() >= 0 && position.getX() < width * tileSize &&
        position.getY() >= 0 && position.getY() < height * tileSize;
}
