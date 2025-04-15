#include "src/engine/Engine.h"
#include "src/engine/PrimitiveRenderer.h"
#include "src/engine/Sprite.h"
#include "src/game/Player.h"
#include "src/game/Camera.h"
#include <iostream>
#include <sstream>
#include <string>

// Stałe dla mapy
const int TILE_SIZE = 250;
const int MAP_WIDTH = 8;  // w kafelkach
const int MAP_HEIGHT = 16; // w kafelkach
const int MAP_PIXEL_WIDTH = MAP_WIDTH * TILE_SIZE;   // 2000px
const int MAP_PIXEL_HEIGHT = MAP_HEIGHT * TILE_SIZE; // 4000px

// Zmienne globalne
Engine* engine = nullptr;
PrimitiveRenderer* renderer = nullptr;
ALLEGRO_FONT* font = nullptr;
Player* player = nullptr;
Camera* camera = nullptr;

// Funkcje pomocnicze
std::string getCoordinatesString(const std::string& name, float x, float y) {
    std::stringstream ss;
    ss << name << ": X=" << static_cast<int>(x) << " Y=" << static_cast<int>(y);
    return ss.str();
}

// Główne funkcje
void update(float deltaTime) {
    if (player) {
        player->update(deltaTime);
    }

    if (camera) {
        camera->update();
    }
}

void render() {
    if (camera) {
        camera->beginScene();
    }

    // Rysowanie siatki
    renderer->setColor(al_map_rgb(50, 50, 50));

    // Rysowanie linii pionowych
    for (int x = 0; x <= MAP_WIDTH; x++) {
        int pixelX = x * TILE_SIZE;
        renderer->drawLine(Point2D(pixelX, 0), Point2D(pixelX, MAP_PIXEL_HEIGHT));
    }

    // Rysowanie linii poziomych
    for (int y = 0; y <= MAP_HEIGHT; y++) {
        int pixelY = y * TILE_SIZE;
        renderer->drawLine(Point2D(0, pixelY), Point2D(MAP_PIXEL_WIDTH, pixelY));
    }

    // Rysowanie gracza
    if (player) {
        player->draw();
    }

    if (camera) {
        camera->endScene();
    }

    // UI - rysowane bez transformacji kamery
    ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255);
    Point2D playerPos = player->getPosition();
    al_draw_textf(font, textColor, 10, 10, ALLEGRO_ALIGN_LEFT,
        "Player World Pos: X=%.1f Y=%.1f", playerPos.getX(), playerPos.getY());
}

int main() {
    engine = Engine::getInstance();

    // Konfiguracja silnika
    engine->setScreenSize(1600, 900);
    engine->setTitle("Arena Combat Game");
    engine->setClearColor(al_map_rgb(0, 0, 0));  // Czarne tło

    // Inicjalizacja silnika
    if (!engine->init()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    player = new Player();
    if (!player->loadResources()) {
        std::cerr << "Failed to load player resources!" << std::endl;
        return -1;
    }
    player->setPosition(400, 300);  // Początkowa pozycja na środku ekranu

    camera = new Camera();
    camera->setViewport(engine->getScreenWidth(), engine->getScreenHeight());
    camera->setTarget(player);
    player->setCamera(camera);  // Ustawienie kamery dla gracza

    // Inicjalizacja renderera prymitywów
    renderer = new PrimitiveRenderer(engine->getDisplay());

    // Inicjalizacja czcionki
    font = al_create_builtin_font();
    if (!font) {
        std::cerr << "Failed to create font!" << std::endl;
        return -1;
    }

    // Główna pętla gry
    engine->startGameLoop(update, render);

    // Sprzątanie
    delete player;
    delete camera;
    delete renderer;
    al_destroy_font(font);
    engine->shutdown();

    return 0;
}
