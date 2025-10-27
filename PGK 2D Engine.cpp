#include "src/engine/Engine.h"
#include "src/engine/Sprite.h"
#include "src/game/Player.h"
#include "src/game/Camera.h"
#include "src/game/Map.h"
#include "src/game/Enemy.h"
#include "src/game/BodyManager.h"
#include <iostream>
#include <sstream>
#include <string>

// Stałe dla mapy
const int TILE_SIZE = 250;
const int MAP_WIDTH = 16; // w kafelkach
const int MAP_HEIGHT = 8; // w kafelkach
const int MAP_PIXEL_WIDTH = MAP_WIDTH * TILE_SIZE;     // 2000px
const int MAP_PIXEL_HEIGHT = MAP_HEIGHT * TILE_SIZE; // 4000px

// Zmienne globalne
Engine* engine = nullptr;
ALLEGRO_FONT* font = nullptr;
Player* player = nullptr;
Camera* camera = nullptr;
Map* gameMap = nullptr;
std::vector<Enemy*> enemies;

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
    for (auto enemy : enemies) {
        enemy->update(deltaTime);
    }
    BulletManager::getInstance()->update(deltaTime);
}

void render() {
    if (camera) {
        camera->beginScene();
    }

    if (gameMap) {
        gameMap->draw();
    }

    BodyManager::getInstance()->draw();

    if (player) {
        player->draw();
    }

    for (auto enemy : enemies) {
        enemy->draw();
    }

    BulletManager::getInstance()->draw();

    if (camera) {
        camera->endScene();
    }

    // UI - rysowane bez transformacji kamery
    ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR healthColor = al_map_rgb(255, 50, 50);
    ALLEGRO_COLOR ammoColor = al_map_rgb(255, 215, 0);

    // Debug info
    glm::vec2 playerPos = player->getPosition();
    al_draw_textf(font, textColor, 10, 10, ALLEGRO_ALIGN_LEFT,
        "Player World Pos: X=%.1f Y=%.1f", playerPos.x, playerPos.y);

    // Pasek zdrowia
    float health = player->getHealth();
    float maxHealth = player->getMaxHealth();
    float healthBarWidth = 200.0f;
    float healthBarHeight = 20.0f;
    float healthBarX = 10;
    float healthBarY = Engine::getInstance()->getScreenHeight() - 40;

    // Tło paska zdrowia
    al_draw_filled_rectangle(healthBarX, healthBarY,
        healthBarX + healthBarWidth, healthBarY + healthBarHeight,
        al_map_rgb(60, 60, 60));

    // Aktualny poziom zdrowia
    float currentHealthWidth = (health / maxHealth) * healthBarWidth;
    al_draw_filled_rectangle(healthBarX, healthBarY,
        healthBarX + currentHealthWidth, healthBarY + healthBarHeight,
        healthColor);

    // Tekst zdrowia
    al_draw_textf(font, healthColor, healthBarX + healthBarWidth / 2, healthBarY - 25,
        ALLEGRO_ALIGN_CENTER, "HP: %.0f/%.0f", health, maxHealth);

    // Wyświetlanie amunicji
    int currentAmmo = player->getCurrentAmmo();
    int totalAmmo = player->getTotalAmmo();
    float ammoX = Engine::getInstance()->getScreenWidth() - 10;
    float ammoY = Engine::getInstance()->getScreenHeight() - 40;

    if (player->getIsReloading()) {
        float reloadProgress = player->getCurrentReloadTime() / player->getReloadTime();
        float reloadBarWidth = 150.0f;
        float reloadBarHeight = 10.0f;

        al_draw_filled_rectangle(ammoX - reloadBarWidth, ammoY,
            ammoX - reloadBarWidth + (reloadBarWidth * reloadProgress),
            ammoY + reloadBarHeight,
            ammoColor);

        al_draw_text(font, ammoColor, ammoX, ammoY - 25,
            ALLEGRO_ALIGN_RIGHT, "RELOADING");
    }
    else {
        al_draw_textf(font, ammoColor, ammoX, ammoY, ALLEGRO_ALIGN_RIGHT,
            "%d / %d", currentAmmo, totalAmmo);
    }
}



int main() {
    
    engine = Engine::getInstance();

    // Konfiguracja silnika
    engine->setScreenSize(1600, 900);
    engine->setTitle("Arena Combat Game");
    engine->setClearColor(al_map_rgb(0, 0, 0)); // Czarne tło
    engine->setFullscreen(false);               //Ustawienie trybu pełnoekranowego
    engine->setFPS(60);                         // Ustawienie ilości klatek na sekunde


    // Inicjalizacja silnika
    if (!engine->init()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    CollisionManager* collisionManager = CollisionManager::getInstance();

    player = new Player();
    if (!player->loadResources()) {
        std::cerr << "Failed to load player resources!" << std::endl;
        return -1;
    }
    player->setPosition(100, 100);      // Początkowa pozycja na środku ekranu

    camera = new Camera();
    camera->setViewport(engine->getScreenWidth(), engine->getScreenHeight());
    camera->setTarget(player);
    player->setCamera(camera);      // Ustawienie kamery dla gracza

    gameMap = new Map(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE);
    if (!gameMap->init()) {
        std::cerr << "Failed to initialize map!" << std::endl;
        return -1;
    }
	player->setMap(gameMap);      // Ustawienie mapy dla gracza
    // Inicjalizacja czcionki
    font = al_create_builtin_font();
    if (!font) {
        std::cerr << "Failed to create font!" << std::endl;
        return -1;
    }

    for (int i = 0; i < 5; i++) {
        Enemy* enemy = new Enemy();
        if (!enemy->loadResources()) {
            std::cerr << "Failed to load enemy resources!" << std::endl;
            return -1;
        }
        enemy->setPosition(
            600.0f + i * 500.0f,  // Różne pozycje X
            650.0f + i * 300.0f   // Różne pozycje Y
        );
        enemy->setMap(gameMap);
        enemy->setTarget(player);
        enemies.push_back(enemy);
    }

    // Dodaj kolizje gracza i przeciwników
    collisionManager->addCollision(player->getCollision());

    for (auto enemy : enemies) {
        collisionManager->addCollision(enemy->getCollision());
    }

    // Główna pętla gry
    engine->startGameLoop(update, render);

    // Sprzątanie
    delete player;
    
    delete gameMap;
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    BulletManager::releaseInstance();
    CollisionManager::releaseInstance();
    TextureManager::releaseInstance();
    al_destroy_font(font);
    engine->shutdown();

    return 0;
}