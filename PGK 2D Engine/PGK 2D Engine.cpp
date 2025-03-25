#include "Engine.h"
#include "Primitives.h"
#include "PrimitiveRenderer.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Deklaracje funkcji
void update(float deltaTime);
void render();
std::string getCoordinatesString(const char* label, float x, float y);

// Globalne zmienne
Point2D playerPosition(400.0f, 300.0f);
float playerSpeed = 200.0f;
PrimitiveRenderer* renderer = nullptr;
ALLEGRO_FONT* font = nullptr;
bool drawingTriangle = false;
bool drawingRectangle = false;
bool isFilled = false;

int main() {
    Engine* engine = Engine::getInstance();

    engine->setScreenSize(800, 600);
    engine->setFullscreen(false);
    engine->setFPS(60);
    engine->setTitle("2D Engine Demo - Line Comparison");
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

    al_destroy_font(font);
    delete renderer;

    return 0;
}

std::string getCoordinatesString(const char* label, float x, float y) {
    std::stringstream ss;
    ss << label << ": ("
        << std::fixed << std::setprecision(1) << x << ", "
        << std::fixed << std::setprecision(1) << y << ")";
    return ss.str();
}

void update(float deltaTime) {
    Engine* engine = Engine::getInstance();

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

    playerPosition.clamp(0, 0,
        engine->getScreenWidth(),
        engine->getScreenHeight());

    if (engine->isKeyDown(ALLEGRO_KEY_Q)) {
        isFilled = !isFilled;
    }
}

void render() {
    Engine* engine = Engine::getInstance();

    int mouseX = engine->getMouseX();
    int mouseY = engine->getMouseY();
    Point2D mousePoint(mouseX, mouseY);

    // Rysowanie różnych kształtów w zależności od wciśniętego klawisza
    if (engine->isKeyDown(ALLEGRO_KEY_1)) {
        // Rysowanie trójkąta
        Triangle triangle(
            playerPosition,                          // pierwszy wierzchołek
            Point2D(mouseX, mouseY),                // drugi wierzchołek
            Point2D(playerPosition.getX() - (mouseX - playerPosition.getX()), mouseY), // trzeci wierzchołek
            isFilled // wypełniony?
        );
        renderer->setColor(al_map_rgb(255, 0, 0));
        renderer->drawTriangle(triangle);
    }
    else if (engine->isKeyDown(ALLEGRO_KEY_2)) {
        // Rysowanie prostokąta
        Rectangle rectangle(
            playerPosition,  // lewy górny róg
            mouseX - playerPosition.getX(),  // szerokość
            mouseY - playerPosition.getY(),  // wysokość
            isFilled  // wypełniony?
        );
        renderer->setColor(al_map_rgb(0, 255, 0));
        renderer->drawRectangle(rectangle);
    }
    else if (engine->isMouseButtonDown(1)) {  // PPM
        al_draw_line(
            playerPosition.getX(),
            playerPosition.getY(),
            mousePoint.getX(),
            mousePoint.getY(),
            al_map_rgb(255, 0, 0),
            2.0f
        );
    }
    else {
        LineSegment playerToMouse(playerPosition, mousePoint);
        renderer->setColor(al_map_rgb(255, 255, 0));
        renderer->drawLine(playerToMouse);
    }

    // Rysowanie gracza
    renderer->setColor(al_map_rgb(255, 0, 0));
    renderer->drawPoint(playerPosition);

    // Rysowanie punktu kursora
    if (engine->isMouseButtonDown(0)) {  // LPM
        renderer->setColor(al_map_rgb(0, 255, 0));
    }
    else {
        renderer->setColor(al_map_rgb(255, 255, 255));
    }
    renderer->drawPoint(mousePoint);

    // Rysowanie legendy i współrzędnych
    ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255);
    int lineHeight = al_get_font_line_height(font);

    // Instrukcje
    al_draw_text(
        font,
        textColor,
        10, 10,
        0,
        "LPM - zmiana koloru kursora | PPM - przelaczenie na linie Allegro | 1 - trojkat | 2 - prostokat"
    );

    // Współrzędne gracza
    al_draw_text(
        font,
        textColor,
        10, 10 + lineHeight,
        0,
        getCoordinatesString("Gracz", playerPosition.getX(), playerPosition.getY()).c_str()
    );

    // Współrzędne kursora
    al_draw_text(
        font,
        textColor,
        10, 10 + 2 * lineHeight,
        0,
        getCoordinatesString("Kursor", mousePoint.getX(), mousePoint.getY()).c_str()
    );
}