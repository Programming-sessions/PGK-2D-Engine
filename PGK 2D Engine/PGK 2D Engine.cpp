#include "Engine.h"
#include "Primitives.h"
#include "PrimitiveRenderer.h"
#include <iostream>

// Deklaracje funkcji
void update(float deltaTime);
void render();

// Globalne zmienne
Point2D playerPosition(400.0f, 300.0f);
float playerSpeed = 200.0f;
PrimitiveRenderer* renderer = nullptr;

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

    // Utworzenie renderera prymitywów
    renderer = new PrimitiveRenderer(engine->getDisplay());

    // Uruchomienie głównej pętli gry
    engine->startGameLoop(update, render);

    // Czyszczenie
    delete renderer;

    return 0;
}

void update(float deltaTime) {
    Engine* engine = Engine::getInstance();

    // Obsługa klawiatury - poruszanie punktem
    if (engine->isKeyDown(ALLEGRO_KEY_W) || engine->isKeyDown(ALLEGRO_KEY_UP)) {
        playerPosition.setY(playerPosition.getY() - playerSpeed * deltaTime);
    }
    if (engine->isKeyDown(ALLEGRO_KEY_S) || engine->isKeyDown(ALLEGRO_KEY_DOWN)) {
        playerPosition.setY(playerPosition.getY() + playerSpeed * deltaTime);
    }
    if (engine->isKeyDown(ALLEGRO_KEY_A) || engine->isKeyDown(ALLEGRO_KEY_LEFT)) {
        playerPosition.setX(playerPosition.getX() - playerSpeed * deltaTime);
    }
    if (engine->isKeyDown(ALLEGRO_KEY_D) || engine->isKeyDown(ALLEGRO_KEY_RIGHT)) {
        playerPosition.setX(playerPosition.getX() + playerSpeed * deltaTime);
    }

    // Ograniczenie ruchu do obszaru ekranu
    playerPosition.clamp(0, 0,
        engine->getScreenWidth(),
        engine->getScreenHeight());
}

void render() {
    Engine* engine = Engine::getInstance();

    // Rysowanie gracza (punkt)
    renderer->setColor(al_map_rgb(255, 0, 0));  // Czerwony kolor
    renderer->drawPoint(playerPosition);

    // Rysowanie punktu w pozycji kursora myszy
    int mouseX = engine->getMouseX();
    int mouseY = engine->getMouseY();
    Point2D mousePoint(mouseX, mouseY);

    if (engine->isMouseButtonDown(0)) {
        renderer->setColor(al_map_rgb(0, 255, 0));  // Zielony gdy wciśnięty
    }
    else {
        renderer->setColor(al_map_rgb(255, 255, 255));  // Biały gdy nie wciśnięty
    }
    renderer->drawPoint(mousePoint);
}
