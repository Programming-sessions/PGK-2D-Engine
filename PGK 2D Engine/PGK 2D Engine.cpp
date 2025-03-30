#include "Engine.h"
#include "Primitives.h"
#include "PrimitiveRenderer.h"
#include "Transform.h"
#include <iostream>

// Deklaracje funkcji
void update(float deltaTime);
void render();

// Globalne zmienne
Point2D playerPosition(400.0f, 300.0f);
float playerSpeed = 200.0f;
PrimitiveRenderer* renderer = nullptr;
ALLEGRO_FONT* font = nullptr;

// Zmienne dla aktualnie rysowanej figury
enum class ShapeType { NONE, LINE, TRIANGLE, RECTANGLE, CIRCLE };
ShapeType currentShapeType = ShapeType::NONE;
LineSegment currentLine;
Triangle currentTriangle;
Rectangle currentRectangle;
Circle currentCircle;

int main() {
    Engine* engine = Engine::getInstance();

    engine->setScreenSize(800, 600);
    engine->setFullscreen(false);
    engine->setFPS(60);
    engine->setTitle("2D Engine Demo");
    engine->setClearColor(al_map_rgb(64, 64, 100));

    if (!engine->init()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    font = al_create_builtin_font();
    if (!font) {
        std::cerr << "Failed to create font!" << std::endl;
        return -1;
    }

    renderer = new PrimitiveRenderer(engine->getDisplay());
    engine->startGameLoop(update, render);
    delete renderer;

    return 0;
}

void update(float deltaTime) {
    Engine* engine = Engine::getInstance();

    // Obsługa ruchu gracza
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

    playerPosition.clamp(0, 0, engine->getScreenWidth(), engine->getScreenHeight());

    // Obsługa transformacji dla aktualnej figury
    Point2D mousePoint(engine->getMouseX(), engine->getMouseY());
    float rotationSpeed = 2.0f; // stopnie na klatkę
    float scaleSpeed = 0.02f; // procent zmiany na klatkę

    switch (currentShapeType) {
    case ShapeType::LINE:
        if (engine->isKeyDown(ALLEGRO_KEY_R)) {
            currentLine.rotate(rotationSpeed, playerPosition);
        }
        if (engine->isKeyDown(ALLEGRO_KEY_E)) {
            currentLine.scale(1.0f + scaleSpeed, 1.0f + scaleSpeed, playerPosition);
        }
        if (engine->isKeyDown(ALLEGRO_KEY_Q)) {
            currentLine.scale(1.0f - scaleSpeed, 1.0f - scaleSpeed, playerPosition);
        }
        break;

    case ShapeType::TRIANGLE:
        if (engine->isKeyDown(ALLEGRO_KEY_R)) {
            currentTriangle.rotate(rotationSpeed, playerPosition);
        }
        if (engine->isKeyDown(ALLEGRO_KEY_E)) {
            currentTriangle.scale(1.0f + scaleSpeed, 1.0f + scaleSpeed, playerPosition);
        }
        if (engine->isKeyDown(ALLEGRO_KEY_Q)) {
            currentTriangle.scale(1.0f - scaleSpeed, 1.0f - scaleSpeed, playerPosition);
        }
        break;

    case ShapeType::RECTANGLE:
        if (engine->isKeyDown(ALLEGRO_KEY_R)) {
            currentRectangle.rotate(rotationSpeed, playerPosition);
        }
        if (engine->isKeyDown(ALLEGRO_KEY_E)) {
            currentRectangle.scale(1.0f + scaleSpeed, 1.0f + scaleSpeed, playerPosition);
        }
        if (engine->isKeyDown(ALLEGRO_KEY_Q)) {
            currentRectangle.scale(1.0f - scaleSpeed, 1.0f - scaleSpeed, playerPosition);
        }
        break;

    case ShapeType::CIRCLE:
        if (engine->isKeyDown(ALLEGRO_KEY_E)) {
            currentCircle.scale(1.0f + scaleSpeed, 1.0f + scaleSpeed);
        }
        if (engine->isKeyDown(ALLEGRO_KEY_Q)) {
            currentCircle.scale(1.0f - scaleSpeed, 1.0f - scaleSpeed);
        }
        break;
    }
}

void render() {
    Engine* engine = Engine::getInstance();

    int mouseX = engine->getMouseX();
    int mouseY = engine->getMouseY();
    Point2D mousePoint(mouseX, mouseY);

    // Obliczanie promienia dla koła
    float radius = sqrt(
        pow(mouseX - playerPosition.getX(), 2) +
        pow(mouseY - playerPosition.getY(), 2)
    );

    // Wybór i rysowanie figury
    if (engine->isKeyDown(ALLEGRO_KEY_1)) {
        currentShapeType = ShapeType::TRIANGLE;
        currentTriangle = Triangle(
            playerPosition,
            Point2D(mouseX, mouseY),
            Point2D(playerPosition.getX() - (mouseX - playerPosition.getX()), mouseY),
            true
        );
    }
    else if (engine->isKeyDown(ALLEGRO_KEY_2)) {
        currentShapeType = ShapeType::RECTANGLE;
        currentRectangle = Rectangle(
            playerPosition,
            mouseX - playerPosition.getX(),
            mouseY - playerPosition.getY(),
            true
        );
    }
    else if (engine->isKeyDown(ALLEGRO_KEY_3)) {
        currentShapeType = ShapeType::CIRCLE;
        currentCircle = Circle(playerPosition, radius, true);
    }
    else if (engine->isMouseButtonDown(1)) {
        currentShapeType = ShapeType::LINE;
        currentLine = LineSegment(playerPosition, mousePoint);
    }

    // Rysowanie aktualnej figury
    renderer->setColor(al_map_rgb(255, 255, 0));
    switch (currentShapeType) {
    case ShapeType::LINE:
        renderer->drawLine(currentLine);
        break;
    case ShapeType::TRIANGLE:
        renderer->drawTriangle(currentTriangle);
        break;
    case ShapeType::RECTANGLE:
        renderer->drawRectangle(currentRectangle);
        break;
    case ShapeType::CIRCLE:
        renderer->drawCircle(currentCircle);
        break;
    }

    // Rysowanie gracza
    renderer->setColor(al_map_rgb(255, 0, 0));
    renderer->drawPoint(playerPosition);

    // Rysowanie kursora
    renderer->setColor(engine->isMouseButtonDown(0) ?
        al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255));
    renderer->drawPoint(mousePoint);

    // Wyświetlanie instrukcji
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        10, 10,
        0,
        "1-trojkat | 2-prostokat | 3-okrag | PPM-linia | Q/E-skala | R-obrot"
    );
}
