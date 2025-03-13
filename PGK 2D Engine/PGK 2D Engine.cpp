#include "Engine.h"
#include <iostream>

// Deklaracje funkcji
void update(float deltaTime);
void render();

// Globalne zmienne
float playerX = 400.0f;
float playerY = 300.0f;
float playerSpeed = 200.0f;

int main() {
    // Pobierz instancję silnika
    Engine* engine = Engine::getInstance();

    // Konfiguracja silnika
    engine->setScreenSize(800, 600);
    engine->setFullscreen(false);
    engine->setFPS(60);
    engine->setTitle("2D Engine Demo");
    engine->setClearColor(al_map_rgb(64, 64, 100));

    // Inicjalizacja silnika
    if (!engine->init()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    // Uruchomienie głównej pętli gry
    engine->startGameLoop(update, render);

    return 0;
}

// Funkcja aktualizująca logikę gry
void update(float deltaTime) {
    Engine* engine = Engine::getInstance();

    // Obsługa klawiatury - poruszanie prostokątem
    if (engine->isKeyDown(ALLEGRO_KEY_W)) {
        playerY -= playerSpeed * deltaTime;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_S)) {
        playerY += playerSpeed * deltaTime;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_A)) {
        playerX -= playerSpeed * deltaTime;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_D)) {
        playerX += playerSpeed * deltaTime;
    }

    // Ograniczenie ruchu do obszaru ekranu
    if (playerX < 0) playerX = 0;
    if (playerY < 0) playerY = 0;
    if (playerX > engine->getScreenWidth()) playerX = engine->getScreenWidth();
    if (playerY > engine->getScreenHeight()) playerY = engine->getScreenHeight();
}

// Funkcja renderująca
void render() {
    Engine* engine = Engine::getInstance();

    // Rysowanie gracza (prostokąt)
    al_draw_filled_rectangle(playerX - 20, playerY - 20,
        playerX + 20, playerY + 20,
        al_map_rgb(255, 0, 0));

    // Rysowanie tekstu z informacją o położeniu myszy
    int mouseX = engine->getMouseX();
    int mouseY = engine->getMouseY();

    // Rysowanie okręgu w pozycji kursora myszy gdy lewy przycisk wciśnięty
    if (engine->isMouseButtonDown(0)) {
        al_draw_filled_circle(mouseX, mouseY, 15, al_map_rgb(0, 255, 0));
    }
    else {
        al_draw_circle(mouseX, mouseY, 15, al_map_rgb(255, 255, 255), 2);
    }
}
