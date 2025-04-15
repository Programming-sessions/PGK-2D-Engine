#include "Engine.h"
#include "Primitives.h"
#include "PrimitiveRenderer.h"
#include "Transform.h"
#include "Sprite.h"
#include <iostream>

// Deklaracje funkcji
void update(float deltaTime);
void render();
// Globalne zmienne
Point2D playerPosition(400.0f, 300.0f);
float playerSpeed = 200.0f;
PrimitiveRenderer* renderer = nullptr;
ALLEGRO_FONT* font = nullptr;
Sprite* playerSprite = nullptr;

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
    playerSprite = new Sprite();
    if (!playerSprite->loadTexture("assets/textures/hero_idle.png")) {
        std::cerr << "Failed to load hero_idle texture!" << std::endl;
        return -1;
    }
    // Tworzenie animacji idle
    Animation* idleAnimation = new Animation("idle", true); // true dla zapętlenia

    // Szerokość i wysokość pojedynczej klatki
    const int FRAME_WIDTH = 253;
    const int FRAME_HEIGHT = 216;
    const float FRAME_DURATION = 0.05f; // 20 fps

    // Dodawanie 20 klatek w odpowiedniej kolejności
    // Numeracja klatek:
    // 0  1  2  3  4
    // 5  6  7  8  9
    // 10 11 12 13 14
    // 15 16 17 18 19
    for (int frame = 0; frame < 20; frame++) {
        // Obliczamy pozycję klatki w sprite sheet
        int row = frame / 5;        // Dzielenie całkowite da nam numer wiersza
        int col = frame % 5;        // Modulo da nam numer kolumny

        int x = col * FRAME_WIDTH;  // Pozycja X klatki
        int y = row * FRAME_HEIGHT; // Pozycja Y klatki

        idleAnimation->addFrame(x, y, FRAME_WIDTH, FRAME_HEIGHT, FRAME_DURATION);
    }

    // Dodajemy animację do sprite'a
    playerSprite->addAnimation(idleAnimation);

    // Rozpoczynamy odtwarzanie animacji
    playerSprite->playAnimation("idle");
    playerSprite->setPosition(400.0f, 300.0f);  // Początkowa pozycja na środku
    playerSprite->setScale(1.0f, 1.0f);         // Domyślna skala
    engine->startGameLoop(update, render);
    delete playerSprite;
    delete renderer;

    return 0;
}

void update(float deltaTime) {
    Engine* engine = Engine::getInstance();

    // Zapisanie poprzedniej pozycji (na wypadek kolizji ze ścianami)
    float previousX = playerPosition.getX();
    float previousY = playerPosition.getY();

    // Obsługa ruchu
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

    // Pobierz wymiary sprite'a
    const float SPRITE_HALF_WIDTH = 253.0f / 2;  // Połowa szerokości klatki
    const float SPRITE_HALF_HEIGHT = 216.0f / 2; // Połowa wysokości klatki

    // Clamp z uwzględnieniem wymiarów sprite'a
    playerPosition.clamp(
        SPRITE_HALF_WIDTH,                           // minX
        SPRITE_HALF_HEIGHT,                          // minY
        engine->getScreenWidth() - SPRITE_HALF_WIDTH,   // maxX
        engine->getScreenHeight() - SPRITE_HALF_HEIGHT  // maxY
    );

    // Obliczanie kąta między sprite'em a kursorem
    float mouseX = engine->getMouseX();
    float mouseY = engine->getMouseY();
    float spriteX = playerPosition.getX();
    float spriteY = playerPosition.getY();

    // Obliczanie różnicy pozycji
    float dx = mouseX - spriteX;
    float dy = mouseY - spriteY;

    // Obliczanie kąta w radianach
    float angle = atan2(dy, dx);

    // Ustawianie rotacji sprite'a
    playerSprite->setRotation(angle);

    // Aktualizacja pozycji sprite'a
    playerSprite->setPosition(playerPosition.getX(), playerPosition.getY());

    // Aktualizacja animacji
    playerSprite->updateAnimation(deltaTime);
}

// Modyfikujemy funkcję render():
void render() {
    Engine* engine = Engine::getInstance();

    int mouseX = engine->getMouseX();
    int mouseY = engine->getMouseY();
    Point2D mousePoint(mouseX, mouseY);

    // Rysowanie różnych kształtów w zależności od wciśniętego klawisza
    if (engine->isKeyDown(ALLEGRO_KEY_1)) {
        // Rysowanie trójkąta
        Triangle triangle(
            playerPosition,
            Point2D(mouseX, mouseY),
            Point2D(playerPosition.getX() - (mouseX - playerPosition.getX()), mouseY),
            true
        );
        renderer->setColor(al_map_rgb(255, 0, 0));
        renderer->drawTriangle(triangle);
    }
    else if (engine->isKeyDown(ALLEGRO_KEY_2)) {
        // Rysowanie prostokąta
        Rectangle rectangle(
            playerPosition,
            mouseX - playerPosition.getX(),
            mouseY - playerPosition.getY(),
            true
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

    // Rysowanie sprite'a gracza zamiast punktu
    playerSprite->draw();

    // Rysowanie punktu kursora
    if (engine->isMouseButtonDown(0)) {  // LPM
        renderer->setColor(al_map_rgb(0, 255, 0));
    }
    else {
        renderer->setColor(al_map_rgb(255, 255, 255));
    }
    renderer->drawPoint(mousePoint);
}
