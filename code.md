## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\PGK 2D Engine.cpp
﻿#include "src/engine/Engine.h"
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


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Animation.cpp
#include "Animation.h"

Animation::Animation(const std::string& name, bool looping)
    : name(name)
    , totalDuration(0.0f)
    , isLooping(looping)
    , currentFrameIndex(0)
    , currentFrameTime(0.0f)
    , isPlaying(false)
{
}

void Animation::addFrame(int x, int y, int width, int height, float duration) {
    frames.emplace_back(x, y, width, height, duration);
    totalDuration += duration;
}

const AnimationFrame& Animation::getCurrentFrame() const {
    if (frames.empty()) {
        static AnimationFrame defaultFrame(0, 0, 0, 0, 0.0f);
        return defaultFrame;
    }
    return frames[currentFrameIndex];
}

void Animation::play() {
    isPlaying = true;
}

void Animation::pause() {
    isPlaying = false;
}

void Animation::stop() {
    isPlaying = false;
    reset();
}

void Animation::reset() {
    currentFrameIndex = 0;
    currentFrameTime = 0.0f;
}

void Animation::update(float deltaTime) {
    if (!isPlaying || frames.empty()) return;

    currentFrameTime += deltaTime;

    // Sprawdzamy czy czas obecnej klatki si� sko�czy�
    while (currentFrameTime >= frames[currentFrameIndex].duration) {
        currentFrameTime -= frames[currentFrameIndex].duration;
        currentFrameIndex++;

        // Sprawdzamy czy dotarli�my do ko�ca animacji
        if (currentFrameIndex >= frames.size()) {
            if (isLooping) {
                currentFrameIndex = 0;
            }
            else {
                currentFrameIndex = frames.size() - 1;
                isPlaying = false;
                break;
            }
        }
    }
}

bool Animation::isFinished() const {
    return !isLooping && !isPlaying && currentFrameIndex == frames.size() - 1;
}

std::string Animation::getName() const {
    return name;
}

bool Animation::getIsLooping() const {
    return isLooping;
}

bool Animation::getIsPlaying() const {
    return isPlaying;
}

void Animation::setLooping(bool loop) {
    isLooping = loop;
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Animation.h
#pragma once
// Animation.h
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>

// Struktura przechowuj�ca dane pojedynczej klatki animacji
struct AnimationFrame {
    int x;          // Pozycja X w sprite sheet
    int y;          // Pozycja Y w sprite sheet
    int width;      // Szeroko�� klatki
    int height;     // Wysoko�� klatki
    float duration; // Czas trwania klatki w sekundach

    AnimationFrame(int x, int y, int width, int height, float duration)
        : x(x), y(y), width(width), height(height), duration(duration) {
    }
};

class Animation {
private:
    std::string name;                    // Nazwa animacji
    std::vector<AnimationFrame> frames;  // Klatki animacji
    float totalDuration;                 // Ca�kowity czas trwania
    bool isLooping;                      // Czy animacja si� zap�tla

    // Stan animacji
    int currentFrameIndex;               // Indeks aktualnej klatki
    float currentFrameTime;              // Czas sp�dzony na aktualnej klatce
    bool isPlaying;                      // Czy animacja jest odtwarzana

public:
    Animation(const std::string& name, bool looping = true);

    // Zarz�dzanie klatkami
    void addFrame(int x, int y, int width, int height, float duration);
    const AnimationFrame& getCurrentFrame() const;

    // Kontrola odtwarzania
    void play();
    void pause();
    void stop();
    void reset();

    // Aktualizacja stanu
    void update(float deltaTime);

    // Gettery
    bool isFinished() const;
    std::string getName() const;
    bool getIsLooping() const;
    bool getIsPlaying() const;

    // Settery
    void setLooping(bool loop);
};

#endif // ANIMATION_H




## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Engine.cpp
#include "Engine.h"
#include <iostream>
#include <allegro5/allegro_color.h>

// Inicjalizacja statycznego wska�nika
Engine* Engine::instance = nullptr;

Engine* Engine::getInstance() {
    if (instance == nullptr) {
        instance = new Engine();
    }
    return instance;
}

Engine::Engine() :
    screenWidth(800),
    screenHeight(600),
    fullscreen(false),
    fps(60),
    title("PGK Engine"),
    display(nullptr),
    eventQueue(nullptr),
    timer(nullptr),
    running(false),
    redraw(false),
    mouseX(0),
    mouseY(0) {

    clearColor = al_map_rgb(0, 0, 0);

    // Inicjalizacja stan�w klawiszy i myszy
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        keys[i] = false;
    }

    for (int i = 0; i < 3; i++) {
        mouseButtons[i] = false;
    }
}

Engine::~Engine() {
    shutdown();
}

bool Engine::init() {
    // Inicjalizacja loggera
    if (!logger.init("engine_log.txt", true)) {
        std::cerr << "Failed to initialize logger!" << std::endl;
        return false;
    }

    logger.info("Engine initialization started");

    // Inicjalizacja poszczeg�lnych komponent�w
    if (!initAllegro()) {
        logger.fatal("Failed to initialize Allegro library");
        return false;
    }

    if (!initDisplay()) {
        logger.fatal("Failed to initialize display");
        return false;
    }

    if (!initInput()) {
        logger.fatal("Failed to initialize input devices");
        return false;
    }

    if (!initTimers()) {
        logger.fatal("Failed to initialize timers");
        return false;
    }

    logger.info("Engine initialized successfully");
    return true;
}

bool Engine::initAllegro() {
    if (!al_init()) {
        logger.fatal("Failed to initialize Allegro");
        return false;
    }

    if (!al_init_primitives_addon()) {
        logger.fatal("Failed to initialize primitives addon");
        return false;
    }

    if (!al_init_image_addon()) {
        logger.fatal("Failed to initialize image addon");
        return false;
    }

    if (!al_init_font_addon()) {
        logger.fatal("Failed to initialize font addon");
        return false;
    }

    if (!al_init_ttf_addon()) {
        logger.fatal("Failed to initialize TTF addon");
        return false;
    }

    logger.info("Allegro library initialized");
    return true;
}

bool Engine::initDisplay() {
    // Ustawienie parametr�w wy�wietlania
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_display_flags(fullscreen ? ALLEGRO_FULLSCREEN : ALLEGRO_WINDOWED);

    display = al_create_display(screenWidth, screenHeight);
    if (!display) {
        logger.fatal("Failed to create display");
        return false;
    }

    al_set_window_title(display, title.c_str());
    logger.info("Display initialized: " + std::to_string(screenWidth) + "x" +
        std::to_string(screenHeight) + (fullscreen ? " fullscreen" : " windowed"));

    return true;
}

bool Engine::initInput() {
    if (!al_install_keyboard()) {
        logger.fatal("Failed to install keyboard");
        return false;
    }

    if (!al_install_mouse()) {
        logger.fatal("Failed to install mouse");
        return false;
    }

    logger.info("Input devices initialized");
    return true;
}

bool Engine::initTimers() {
    timer = al_create_timer(1.0 / fps);
    if (!timer) {
        logger.fatal("Failed to create timer");
        return false;
    }

    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        logger.fatal("Failed to create event queue");
        return false;
    }

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    logger.info("Timers initialized at " + std::to_string(fps) + " FPS");
    return true;
}

void Engine::startGameLoop(std::function<void(float)> updateCallback,
    std::function<void()> renderCallback) {
    if (!timer || !eventQueue) {
        logger.error("Cannot start game loop without timer or event queue");
        return;
    }

    float deltaTime = 1.0f / fps;
    running = true;
    redraw = false;

    al_start_timer(timer);
    logger.info("Game loop started");

    while (running) {
        processEvents();

        if (redraw && al_is_event_queue_empty(eventQueue)) {
            redraw = false;

            // Wywo�anie funkcji aktualizuj�cej logik� gry
            if (updateCallback) {
                updateCallback(deltaTime);
            }

            // Czyszczenie ekranu
            clearScreen();

            // Wywo�anie funkcji renderuj�cej
            if (renderCallback) {
                renderCallback();
            }

            // Zamiana bufor�w
            flipDisplay();
        }
    }

    logger.info("Game loop ended");
}

void Engine::processEvents() {
    ALLEGRO_EVENT event;

    while (al_get_next_event(eventQueue, &event)) {
        switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            keys[event.keyboard.keycode] = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                running = false;
            }
            break;

        case ALLEGRO_EVENT_KEY_UP:
            keys[event.keyboard.keycode] = false;
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
            mouseX = event.mouse.x;
            mouseY = event.mouse.y;
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (event.mouse.button <= 3) {
                mouseButtons[event.mouse.button - 1] = true;
            }
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            if (event.mouse.button <= 3) {
                mouseButtons[event.mouse.button - 1] = false;
            }
            break;
        }
    }
}

void Engine::shutdown() {
    logger.info("Engine shutdown initiated");

    // Zniszczenie zasob�w Allegro w odwrotnej kolejno�ci od utworzenia
    if (timer) {
        al_destroy_timer(timer);
        timer = nullptr;
    }

    if (eventQueue) {
        al_destroy_event_queue(eventQueue);
        eventQueue = nullptr;
    }

    if (display) {
        al_destroy_display(display);
        display = nullptr;
    }

    // Deinicjalizacja dodatk�w Allegro
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();

    logger.info("Engine shutdown completed");
    logger.close();
}

// Obs�uga urz�dze� wej�cia
bool Engine::isKeyDown(int keycode) {
    if (keycode >= 0 && keycode < ALLEGRO_KEY_MAX) {
        return keys[keycode];
    }
    return false;
}

bool Engine::isMouseButtonDown(int button) {
    if (button >= 0 && button < 3) {
        return mouseButtons[button];
    }
    return false;
}

int Engine::getMouseX() {
    return mouseX;
}

int Engine::getMouseY() {
    return mouseY;
}

// Operacje graficzne
void Engine::clearScreen() {
    clearBitmap(al_get_backbuffer(display), clearColor);
}

void Engine::clearBitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color) {
    ALLEGRO_BITMAP* prevTarget = al_get_target_bitmap();
    al_set_target_bitmap(bitmap);
    al_clear_to_color(color);
    al_set_target_bitmap(prevTarget);
}

void Engine::flipDisplay() {
    al_flip_display();
}

// Settery konfiguracji
void Engine::setScreenSize(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    if (display) {
        al_resize_display(display, width, height);
        logger.info("Display resized to " + std::to_string(width) + "x" + std::to_string(height));
    }
}

void Engine::setFullscreen(bool fs) {
    if (fullscreen != fs) {
        fullscreen = fs;

        if (display) {
            if (fullscreen) {
                al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
            }
            else {
                al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
                al_resize_display(display, screenWidth, screenHeight);
            }

            logger.info(std::string("Switched to ") + (fullscreen ? "fullscreen" : "windowed") + " mode");
        }
    }
}

void Engine::setFPS(int newFps) {
    if (fps != newFps && newFps > 0) {
        fps = newFps;

        if (timer) {
            al_set_timer_speed(timer, 1.0 / fps);
            logger.info("FPS changed to " + std::to_string(fps));
        }
    }
}

void Engine::setTitle(const std::string& newTitle) {
    title = newTitle;

    if (display) {
        al_set_window_title(display, title.c_str());
    }
}

void Engine::setClearColor(ALLEGRO_COLOR color) {
    clearColor = color;
}

// Gettery
int Engine::getScreenWidth() const {
    return screenWidth;
}

int Engine::getScreenHeight() const {
    return screenHeight;
}

bool Engine::isRunning() const {
    return running;
}

float Engine::getDeltaTime() const {
    return 1.0f / fps;
}

ALLEGRO_DISPLAY* Engine::getDisplay() {
    return display;
}

## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Engine.h
#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <functional>
#include <map>
#include "Logger.h"

class Engine {
private:
    // Singleton
    static Engine* instance;
    Engine();

    // Podstawowe parametry
    int screenWidth;
    int screenHeight;
    bool fullscreen;
    int fps;
    std::string title;
    ALLEGRO_COLOR clearColor;

    // Zasoby Allegro
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_TIMER* timer;

    // Stan klawiatury i myszy
    bool keys[ALLEGRO_KEY_MAX];
    bool mouseButtons[3];
    int mouseX, mouseY;

    // Stan silnika
    bool running;
    bool redraw;

    // Obs�uga b��d�w
    Logger logger;

    // Inicjalizacja komponent�w
    bool initAllegro();
    bool initDisplay();
    bool initInput();
    bool initTimers();

public:
    // Singleton
    static Engine* getInstance();
    ~Engine();

    // Inicjalizacja i zamkni�cie
    bool init();
    void shutdown();

    // Konfiguracja
    void setScreenSize(int width, int height);
    void setFullscreen(bool fullscreen);
    void setFPS(int fps);
    void setTitle(const std::string& title);
    void setClearColor(ALLEGRO_COLOR color);

    // G��wna p�tla i obs�uga zdarze�
    void startGameLoop(std::function<void(float)> updateCallback,
        std::function<void()> renderCallback);
    void processEvents();

    // Obs�uga urz�dze� wej�cia
    bool isKeyDown(int keycode);
    bool isMouseButtonDown(int button);
    int getMouseX();
    int getMouseY();

    // Operacje graficzne
    void clearScreen();
    void clearBitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color);
    void flipDisplay();

    // Gettery
    int getScreenWidth() const;
    int getScreenHeight() const;
    bool isRunning() const;
    float getDeltaTime() const;
    ALLEGRO_DISPLAY* getDisplay();
};

#endif // ENGINE_H

## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Logger.cpp
#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

Logger::Logger() : consoleOutput(true) {}

Logger::~Logger() {
    close();
}

bool Logger::init(const std::string& filePath, bool outputToConsole) {
    logFilePath = filePath;
    consoleOutput = outputToConsole;

    logFile.open(filePath, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        if (consoleOutput) {
            std::cerr << "Failed to open log file: " << filePath << std::endl;
        }
        return false;
    }

    return true;
}

void Logger::log(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    // Bezpieczna konwersja czasu do ci�gu znak�w
    char timeStr[26];
    ctime_s(timeStr, sizeof(timeStr), &time);
    std::string timeString(timeStr);
    // Usuni�cie znaku nowej linii, kt�ry dodaje ctime_s
    if (!timeString.empty() && timeString[timeString.length() - 1] == '\n') {
        timeString.erase(timeString.length() - 1);
    }

    std::string levelString;
    switch (level) {
    case LogLevel::INFO:
        levelString = "INFO";
        break;
    case LogLevel::WARNING:
        levelString = "WARNING";
        break;
    case LogLevel::ERROR:
        levelString = "ERROR";
        break;
    case LogLevel::FATAL:
        levelString = "FATAL";
        break;
    }

    std::string logMessage = "[" + levelString + "] " +
        timeString + " - " + message;

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
        logFile.flush();
    }

    if (consoleOutput) {
        if (level == LogLevel::ERROR || level == LogLevel::FATAL) {
            std::cerr << logMessage << std::endl;
        }
        else {
            std::cout << logMessage << std::endl;
        }
    }
}


void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) {
    log(LogLevel::FATAL, message);
}

void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
private:
    std::ofstream logFile;
    bool consoleOutput;
    std::string logFilePath;

public:
    Logger();
    ~Logger();

    bool init(const std::string& filePath, bool outputToConsole = true);
    void log(LogLevel level, const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
    void close();
};

#endif // LOGGER_H

## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\PrimitiveRenderer.cpp
#include "PrimitiveRenderer.h"
#include <cmath>
#include <algorithm>

PrimitiveRenderer::PrimitiveRenderer(ALLEGRO_DISPLAY* display)
    : display(display), currentColor(al_map_rgb(255, 255, 255)) {
}

void PrimitiveRenderer::setColor(ALLEGRO_COLOR color) {
    currentColor = color;
}

void PrimitiveRenderer::putPixel(int x, int y, ALLEGRO_COLOR color) {
    ALLEGRO_BITMAP* target = al_get_target_bitmap();
    if (x >= 0 && x < al_get_bitmap_width(target) &&
        y >= 0 && y < al_get_bitmap_height(target)) {
        al_draw_pixel(x, y, color);
    }
}

void PrimitiveRenderer::drawPoint(const Point2D& point) {
    putPixel(static_cast<int>(point.getX()),
        static_cast<int>(point.getY()),
        currentColor);
}

void PrimitiveRenderer::bresenhamLine(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        putPixel(x1, y1, color);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void PrimitiveRenderer::drawLine(const LineSegment& line) {
    drawLine(line.getStart(), line.getEnd());
}

void PrimitiveRenderer::drawLine(const Point2D& start, const Point2D& end) {
    bresenhamLine(
        static_cast<int>(start.getX()),
        static_cast<int>(start.getY()),
        static_cast<int>(end.getX()),
        static_cast<int>(end.getY()),
        currentColor
    );
}

void PrimitiveRenderer::scanlineFill(int y, int x1, int x2) {
    // Upewniamy si�, �e x1 < x2
    if (x1 > x2) std::swap(x1, x2);

    // Rysujemy lini� poziom� punkt po punkcie
    for (int x = x1; x <= x2; x++) {
        putPixel(x, y, currentColor);
    }
}

void PrimitiveRenderer::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    // Sortujemy punkty wed�ug y
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
    if (y2 > y3) { std::swap(x2, x3); std::swap(y2, y3); }
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }

    if (y2 == y3) { // Tr�jk�t z p�ask� podstaw� na dole
        float slope1 = (float)(x2 - x1) / (y2 - y1);
        float slope2 = (float)(x3 - x1) / (y3 - y1);

        float curX1 = x1;
        float curX2 = x1;

        for (int y = y1; y <= y2; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope1;
            curX2 += slope2;
        }
    }
    else if (y1 == y2) { // Tr�jk�t z p�ask� podstaw� na g�rze
        float slope1 = (float)(x3 - x1) / (y3 - y1);
        float slope2 = (float)(x3 - x2) / (y3 - y2);

        float curX1 = x1;
        float curX2 = x2;

        for (int y = y1; y <= y3; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope1;
            curX2 += slope2;
        }
    }
    else { // Standardowy przypadek
        float slope1 = (float)(x2 - x1) / (y2 - y1);
        float slope2 = (float)(x3 - x1) / (y3 - y1);
        float slope3 = (float)(x3 - x2) / (y3 - y2);

        float curX1 = x1;
        float curX2 = x1;

        // Pierwsza cz�� tr�jk�ta
        for (int y = y1; y <= y2; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope1;
            curX2 += slope2;
        }

        // Druga cz�� tr�jk�ta
        curX1 = x2;
        for (int y = y2; y <= y3; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope3;
            curX2 += slope2;
        }
    }
}

void PrimitiveRenderer::drawTriangle(const Triangle& triangle) {
    drawTriangle(triangle.getP1(), triangle.getP2(), triangle.getP3(), triangle.isFilled());
}

void PrimitiveRenderer::drawTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled) {
    int x1 = static_cast<int>(p1.getX());
    int y1 = static_cast<int>(p1.getY());
    int x2 = static_cast<int>(p2.getX());
    int y2 = static_cast<int>(p2.getY());
    int x3 = static_cast<int>(p3.getX());
    int y3 = static_cast<int>(p3.getY());

    if (filled) {
        fillTriangle(x1, y1, x2, y2, x3, y3);
    }

    // Zawsze rysujemy obrys
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p1);
}

void PrimitiveRenderer::drawRectangle(const Rectangle& rectangle) {
    drawRectangle(rectangle.getTopLeft(),
        rectangle.getWidth(),
        rectangle.getHeight(),
        rectangle.isFilled());
}

void PrimitiveRenderer::drawRectangle(const Point2D& topLeft, float width, float height, bool filled) {
    int x = static_cast<int>(topLeft.getX());
    int y = static_cast<int>(topLeft.getY());
    int w = static_cast<int>(width);
    int h = static_cast<int>(height);

    // Normalizacja wymiar�w - obs�uga ujemnych warto�ci
    if (w < 0) {
        x += w;
        w = -w;
    }
    if (h < 0) {
        y += h;
        h = -h;
    }

    if (filled) {
        // Wype�niamy prostok�t linia po linii
        for (int currY = y; currY <= y + h; currY++) {
            scanlineFill(currY, x, x + w);
        }
    }

    // Rysujemy obrys prostok�ta
    Point2D topRight(x + w, y);
    Point2D bottomLeft(x, y + h);
    Point2D bottomRight(x + w, y + h);
    Point2D actualTopLeft(x, y);

    drawLine(actualTopLeft, topRight);
    drawLine(topRight, bottomRight);
    drawLine(bottomRight, bottomLeft);
    drawLine(bottomLeft, actualTopLeft);
}

void PrimitiveRenderer::plotCirclePoints(int centerX, int centerY, int x, int y) {
    putPixel(centerX + x, centerY + y, currentColor);
    putPixel(centerX - x, centerY + y, currentColor);
    putPixel(centerX + x, centerY - y, currentColor);
    putPixel(centerX - x, centerY - y, currentColor);
    putPixel(centerX + y, centerY + x, currentColor);
    putPixel(centerX - y, centerY + x, currentColor);
    putPixel(centerX + y, centerY - x, currentColor);
    putPixel(centerX - y, centerY - x, currentColor);
}

void PrimitiveRenderer::fillCircle(int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        int x_val = static_cast<int>(sqrt(radius * radius - y * y));
        scanlineFill(centerY + y, centerX - x_val, centerX + x_val);
    }
}

void PrimitiveRenderer::drawCircle(const Circle& circle) {
    drawCircle(circle.getCenter(), circle.getRadius(), circle.isFilled());
}

void PrimitiveRenderer::drawCircle(const Point2D& center, float radius, bool filled) {
    int centerX = static_cast<int>(center.getX());
    int centerY = static_cast<int>(center.getY());
    int intRadius = static_cast<int>(radius);

    if (filled) {
        fillCircle(centerX, centerY, intRadius);
    }

    // Algorytm Bresenhama dla okr�gu
    int x = 0;
    int y = intRadius;
    int d = 3 - 2 * intRadius;

    plotCirclePoints(centerX, centerY, x, y);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else {
            d = d + 4 * x + 6;
        }
        plotCirclePoints(centerX, centerY, x, y);
    }
}



## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\PrimitiveRenderer.h
#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <allegro5/allegro.h>
#include "Primitives.h"

class PrimitiveRenderer {
private:
    ALLEGRO_COLOR currentColor;
    ALLEGRO_DISPLAY* display;

    // Metoda pomocnicza do rysowania pojedynczego piksela
    void putPixel(int x, int y, ALLEGRO_COLOR color);

    // Implementacja algorytmu Bresenhama dla linii
    void bresenhamLine(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color);

    // Metody pomocnicze do wype�niania
    void scanlineFill(int y, int x1, int x2);
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

    // Metody pomocnicze do rysowania okr�g�w
    void plotCirclePoints(int centerX, int centerY, int x, int y);
    void fillCircle(int centerX, int centerY, int radius);

public:
    PrimitiveRenderer(ALLEGRO_DISPLAY* display);

    // Ustawienie koloru rysowania
    void setColor(ALLEGRO_COLOR color);

    // Metody rysuj�ce
    void drawPoint(const Point2D& point);
    void drawLine(const LineSegment& line);
    void drawLine(const Point2D& start, const Point2D& end);

    // Metody do rysowania tr�jk�t�w
    void drawTriangle(const Triangle& triangle);
    void drawTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled);

    // Metody do rysowania prostok�t�w
    void drawRectangle(const Rectangle& rectangle);
    void drawRectangle(const Point2D& topLeft, float width, float height, bool filled);

    // Metody do rysowania okr�g�w
    void drawCircle(const Circle& circle);
    void drawCircle(const Point2D& center, float radius, bool filled);
};

#endif // PRIMITIVE_RENDERER_H

## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Primitives.cpp
#include "Primitives.h"
#include "Transform.h"
// Constructors
Point2D::Point2D() : x(0.0f), y(0.0f) {}

Point2D::Point2D(float x, float y) : x(x), y(y) {}

// Getters
float Point2D::getX() const {
    return x;
}

float Point2D::getY() const {
    return y;
}

// Setters
void Point2D::setX(float newX) {
    x = newX;
}

void Point2D::setY(float newY) {
    y = newY;
}

void Point2D::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

// Transformations
void Point2D::translate(float dx, float dy) {
    Transform::translate(*this, dx, dy);
}

void Point2D::scale(float sx, float sy, const Point2D& center) {
    Transform::scale(*this, sx, sy, center);
}

void Point2D::rotate(float angle, const Point2D& center) {
    Transform::rotate(*this, angle, center);
}

// Other Functions
void Point2D::clamp(float minX, float minY, float maxX, float maxY) {
    if (x < minX) x = minX;
    if (y < minY) y = minY;
    if (x > maxX) x = maxX;
    if (y > maxY) y = maxY;
}

// Konstruktory
LineSegment::LineSegment() : start(0.0f, 0.0f), end(0.0f, 0.0f) {}

LineSegment::LineSegment(const Point2D& start, const Point2D& end)
    : start(start), end(end) {
}

LineSegment::LineSegment(float x1, float y1, float x2, float y2)
    : start(x1, y1), end(x2, y2) {
}

// Gettery
Point2D LineSegment::getStart() const {
    return start;
}

Point2D LineSegment::getEnd() const {
    return end;
}

// Settery
void LineSegment::setStart(const Point2D& point) {
    start = point;
}

void LineSegment::setEnd(const Point2D& point) {
    end = point;
}

void LineSegment::setStart(float x, float y) {
    start.setPosition(x, y);
}

void LineSegment::setEnd(float x, float y) {
    end.setPosition(x, y);
}

void LineSegment::setLine(const Point2D& newStart, const Point2D& newEnd) {
    start = newStart;
    end = newEnd;
}

void LineSegment::setLine(float x1, float y1, float x2, float y2) {
    start.setPosition(x1, y1);
    end.setPosition(x2, y2);
}

// Transformations
void LineSegment::translate(float dx, float dy) {
    start.translate(dx, dy);
    end.translate(dx, dy);
}

void LineSegment::scale(float sx, float sy, const Point2D& center) {
    start.scale(sx, sy, center);
    end.scale(sx, sy, center);
}

void LineSegment::rotate(float angle, const Point2D& center) {
    start.rotate(angle, center);
    end.rotate(angle, center);
}


// Triangle Implementation
Triangle::Triangle() : p1(0.0f, 0.0f), p2(0.0f, 0.0f), p3(0.0f, 0.0f), filled(false) {}

Triangle::Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled)
    : p1(p1), p2(p2), p3(p3), filled(filled) {
}

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled)
    : p1(x1, y1), p2(x2, y2), p3(x3, y3), filled(filled) {
}

Point2D Triangle::getP1() const { return p1; }
Point2D Triangle::getP2() const { return p2; }
Point2D Triangle::getP3() const { return p3; }
bool Triangle::isFilled() const { return filled; }

void Triangle::setP1(const Point2D& point) { p1 = point; }
void Triangle::setP2(const Point2D& point) { p2 = point; }
void Triangle::setP3(const Point2D& point) { p3 = point; }

void Triangle::setPoints(const Point2D& newP1, const Point2D& newP2, const Point2D& newP3) {
    p1 = newP1;
    p2 = newP2;
    p3 = newP3;
}

void Triangle::setPoints(float x1, float y1, float x2, float y2, float x3, float y3) {
    p1.setPosition(x1, y1);
    p2.setPosition(x2, y2);
    p3.setPosition(x3, y3);
}

void Triangle::setFilled(bool fill) { filled = fill; }

// Transformations
void Triangle::translate(float dx, float dy) {
    p1.translate(dx, dy);
    p2.translate(dx, dy);
    p3.translate(dx, dy);
}

void Triangle::scale(float sx, float sy, const Point2D& center) {
    p1.scale(sx, sy, center);
    p2.scale(sx, sy, center);
    p3.scale(sx, sy, center);
}

void Triangle::rotate(float angle, const Point2D& center) {
    p1.rotate(angle, center);
    p2.rotate(angle, center);
    p3.rotate(angle, center);
}

// Rectangle Implementation
Rectangle::Rectangle()
    : topLeft(0.0f, 0.0f), width(0.0f), height(0.0f), filled(false) {
}

Rectangle::Rectangle(const Point2D& topLeft, float width, float height, bool filled)
    : topLeft(topLeft), width(width), height(height), filled(filled) {
}

Rectangle::Rectangle(float x, float y, float width, float height, bool filled)
    : topLeft(x, y), width(width), height(height), filled(filled) {
}

Point2D Rectangle::getTopLeft() const { return topLeft; }

Point2D Rectangle::getTopRight() const {
    return Point2D(topLeft.getX() + width, topLeft.getY());
}

Point2D Rectangle::getBottomLeft() const {
    return Point2D(topLeft.getX(), topLeft.getY() + height);
}

Point2D Rectangle::getBottomRight() const {
    return Point2D(topLeft.getX() + width, topLeft.getY() + height);
}

float Rectangle::getWidth() const { return width; }
float Rectangle::getHeight() const { return height; }
bool Rectangle::isFilled() const { return filled; }

void Rectangle::setTopLeft(const Point2D& point) { topLeft = point; }

void Rectangle::setTopLeft(float x, float y) {
    topLeft.setPosition(x, y);
}

void Rectangle::setSize(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}

void Rectangle::setFilled(bool fill) { filled = fill; }

bool Rectangle::contains(const Point2D& point) const {
    float px = point.getX();
    float py = point.getY();
    float left = topLeft.getX();
    float top = topLeft.getY();

    return (px >= left && px <= left + width &&
        py >= top && py <= top + height);
}

bool Rectangle::intersects(const Rectangle& other) const {
    float left1 = topLeft.getX();
    float top1 = topLeft.getY();
    float right1 = left1 + width;
    float bottom1 = top1 + height;

    float left2 = other.getTopLeft().getX();
    float top2 = other.getTopLeft().getY();
    float right2 = left2 + other.getWidth();
    float bottom2 = top2 + other.getHeight();

    return !(right1 < left2 || left1 > right2 ||
        bottom1 < top2 || top1 > bottom2);
}

// Transformations
void Rectangle::translate(float dx, float dy) {
    topLeft.translate(dx, dy);
}

void Rectangle::scale(float sx, float sy, const Point2D& center) {
    topLeft.scale(sx, sy, center);
    width *= sx;
    height *= sy;
}

void Rectangle::rotate(float angle, const Point2D& center) {
    // Dla prostok�ta rotacja jest bardziej skomplikowana
    // Trzeba przekszta�ci� go w cztery punkty, obr�ci� i utworzy� nowy prostok�t
    Point2D tr = getTopRight();
    Point2D bl = getBottomLeft();
    Point2D br = getBottomRight();

    topLeft.rotate(angle, center);
    tr.rotate(angle, center);
    bl.rotate(angle, center);
    br.rotate(angle, center);

    // Po obrocie mo�emy potrzebowa� przekalkulowa� wymiary
    // To jest uproszczona wersja - w praktyce mo�e wymaga� dodatkowych oblicze�
    width = tr.getX() - topLeft.getX();
    height = bl.getY() - topLeft.getY();
}

Circle::Circle()
    : center(0.0f, 0.0f), radius(0.0f), filled(false) {
}

Circle::Circle(const Point2D& center, float radius, bool filled)
    : center(center), radius(radius), filled(filled) {
}

Circle::Circle(float x, float y, float radius, bool filled)
    : center(x, y), radius(radius), filled(filled) {
}

// Gettery
Point2D Circle::getCenter() const { return center; }
float Circle::getRadius() const { return radius; }
bool Circle::isFilled() const { return filled; }

// Settery
void Circle::setCenter(const Point2D& point) { center = point; }
void Circle::setCenter(float x, float y) { center.setPosition(x, y); }
void Circle::setRadius(float newRadius) { radius = newRadius; }
void Circle::setFilled(bool fill) { filled = fill; }

// Transformations
void Circle::translate(float dx, float dy) {
    center.translate(dx, dy);
}

void Circle::scale(float sx, float sy) {
    // Dla ko�a u�ywamy tylko jednego wsp�czynnika skalowania
    float scale = (sx + sy) / 2.0f;  // Mo�na te� u�y� std::max(sx, sy)
    radius *= scale;
}

// Dodatkowe metody
bool Circle::contains(const Point2D& point) const {
    float dx = point.getX() - center.getX();
    float dy = point.getY() - center.getY();
    return (dx * dx + dy * dy) <= (radius * radius);
}

## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Primitives.h
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

class Point2D {
private:
    float x;
    float y;

public:
    // Constructors
    Point2D();
    Point2D(float x, float y);

    // Getters
    float getX() const;
    float getY() const;

    // Setters
    void setX(float newX);
    void setY(float newY);
    void setPosition(float newX, float newY);


    //Transformations
    void translate(float dx, float dy);
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    void rotate(float angle, const Point2D& center = Point2D(0, 0));

    // Other Functions
    void clamp(float minX, float minY, float maxX, float maxY);
};

class LineSegment {
private:
    Point2D start;
    Point2D end;

public:
	// Constructors
    LineSegment();
    LineSegment(const Point2D& start, const Point2D& end);
    LineSegment(float x1, float y1, float x2, float y2);


    // Getters
    Point2D getStart() const;
    Point2D getEnd() const;

    // Setters
    void setStart(const Point2D& point);
    void setEnd(const Point2D& point);
    void setStart(float x, float y);
    void setEnd(float x, float y);
    void setLine(const Point2D& newStart, const Point2D& newEnd);
    void setLine(float x1, float y1, float x2, float y2);

    //Transformations
    void translate(float dx, float dy);
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

class Triangle {
private:
    Point2D p1;
    Point2D p2;
    Point2D p3;
    bool filled;

public:
    // Constructors
    Triangle();
    Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled = false);
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);

    // Getters
    Point2D getP1() const;
    Point2D getP2() const;
    Point2D getP3() const;
    bool isFilled() const;

    // Setters
    void setP1(const Point2D& point);
    void setP2(const Point2D& point);
    void setP3(const Point2D& point);
    void setPoints(const Point2D& p1, const Point2D& p2, const Point2D& p3);
    void setPoints(float x1, float y1, float x2, float y2, float x3, float y3);
    void setFilled(bool fill);

    //Transformations
    void translate(float dx, float dy);
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

class Rectangle {
private:
    Point2D topLeft;
    float width;
    float height;
    bool filled;

public:
    // Constructors
    Rectangle();
    Rectangle(const Point2D& topLeft, float width, float height, bool filled = false);
    Rectangle(float x, float y, float width, float height, bool filled = false);

    // Getters
    Point2D getTopLeft() const;
    Point2D getTopRight() const;
    Point2D getBottomLeft() const;
    Point2D getBottomRight() const;
    float getWidth() const;
    float getHeight() const;
    bool isFilled() const;

    // Setters
    void setTopLeft(const Point2D& point);
    void setTopLeft(float x, float y);
    void setSize(float width, float height);
    void setFilled(bool fill);


	// Transformations
    void translate(float dx, float dy);
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    void rotate(float angle, const Point2D& center = Point2D(0, 0));


    // Additional methods
    bool contains(const Point2D& point) const;
    bool intersects(const Rectangle& other) const;
};

class Circle {
private:
    Point2D center;
    float radius;
    bool filled;

public:
	// Constructors
    Circle();
    Circle(const Point2D& center, float radius, bool filled = false);
    Circle(float x, float y, float radius, bool filled = false);

    // Getters
    Point2D getCenter() const;
    float getRadius() const;
    bool isFilled() const;


    // Setters
    void setCenter(const Point2D& point);
    void setCenter(float x, float y);
    void setRadius(float newRadius);
    void setFilled(bool fill);


    // Transformations
    void translate(float dx, float dy);
    void scale(float sx, float sy);  // Dla ko�a sx i sy powinny by� r�wne
    // Ko�o nie potrzebuje rotacji, bo jest symetryczne

	// Additional methods
    bool contains(const Point2D& point) const;
};


#endif // PRIMITIVES_H


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Sprite.cpp
#include "Sprite.h"
#include "TextureManager.h"

Sprite::Sprite()
    : texture(nullptr)
    , position(0.0f, 0.0f)
    , scale(1.0f, 1.0f)
    , rotation(0.0f)
    , alpha(1.0f)
    , currentAnimation(nullptr)
    , sourceX(0)
    , sourceY(0)
    , sourceWidth(0)
    , sourceHeight(0)
{
}

Sprite::~Sprite() {
    // Czyszczenie animacji
    for (auto& pair : animations) {
        delete pair.second;
    }
    animations.clear();
}

bool Sprite::loadTexture(const std::string& path) {
    texture = TextureManager::getInstance()->loadTexture(path);
    if (texture) {
        sourceWidth = al_get_bitmap_width(texture);
        sourceHeight = al_get_bitmap_height(texture);
        return true;
    }
    return false;
}

void Sprite::setPosition(float x, float y) {
    position.setPosition(x, y);
}

void Sprite::setRotation(float angle) {
    rotation = angle;
}

void Sprite::setScale(float scaleX, float scaleY) {
    scale.setPosition(scaleX, scaleY);
}

void Sprite::setAlpha(float newAlpha) {
    alpha = newAlpha < 0.0f ? 0.0f : (newAlpha > 1.0f ? 1.0f : newAlpha);
}

void Sprite::setSourceRect(int x, int y, int width, int height) {
    sourceX = x;
    sourceY = y;
    sourceWidth = width;
    sourceHeight = height;
}

void Sprite::resetSourceRect() {
    if (texture) {
        sourceX = sourceY = 0;
        sourceWidth = al_get_bitmap_width(texture);
        sourceHeight = al_get_bitmap_height(texture);
    }
}

void Sprite::addAnimation(Animation* animation) {
    if (animation) {
        // Je�li animacja o tej nazwie ju� istnieje, usu� j�
        auto it = animations.find(animation->getName());
        if (it != animations.end()) {
            delete it->second;
        }

        animations[animation->getName()] = animation;

        // Je�li to pierwsza animacja, ustaw j� jako aktualn�
        if (!currentAnimation) {
            currentAnimation = animation;
        }
    }
}

void Sprite::playAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (currentAnimation != it->second) {
            if (currentAnimation) {
                currentAnimation->stop();
            }
            currentAnimation = it->second;
        }
        currentAnimation->play();
    }
}

void Sprite::stopAnimation() {
    if (currentAnimation) {
        currentAnimation->stop();
    }
}

void Sprite::pauseAnimation() {
    if (currentAnimation) {
        currentAnimation->pause();
    }
}

void Sprite::resumeAnimation() {
    if (currentAnimation) {
        currentAnimation->play();
    }
}

void Sprite::updateAnimation(float deltaTime) {
    if (currentAnimation) {
        currentAnimation->update(deltaTime);

        // Aktualizacja �r�d�owego prostok�ta na podstawie aktualnej klatki
        const AnimationFrame& frame = currentAnimation->getCurrentFrame();
        sourceX = frame.x;
        sourceY = frame.y;
        sourceWidth = frame.width;
        sourceHeight = frame.height;
    }
}

Animation* Sprite::getCurrentAnimation() const {
    return currentAnimation;
}

Point2D Sprite::getPosition() const {
    return position;
}

Point2D Sprite::getScale() const {
    return scale;
}

float Sprite::getRotation() const {
    return rotation;
}

float Sprite::getAlpha() const {
    return alpha;
}

int Sprite::getWidth() const {
    return sourceWidth;
}

int Sprite::getHeight() const {
    return sourceHeight;
}

void Sprite::draw() {
    if (!texture) return;

    // Zachowanie obecnego stanu transformacji
    ALLEGRO_TRANSFORM transform;
    al_copy_transform(&transform, al_get_current_transform());

    // Ustawienie nowej transformacji dla sprite'a
    ALLEGRO_TRANSFORM spriteTransform;
    al_identity_transform(&spriteTransform);
    al_translate_transform(&spriteTransform, -sourceWidth / 2.0f, -sourceHeight / 2.0f);
    al_scale_transform(&spriteTransform, scale.getX(), scale.getY());
    al_rotate_transform(&spriteTransform, rotation);
    al_translate_transform(&spriteTransform, position.getX(), position.getY());
    al_compose_transform(&spriteTransform, &transform);

    // Zastosowanie transformacji
    al_use_transform(&spriteTransform);

    // Rysowanie z uwzgl�dnieniem przezroczysto�ci i aktualnej klatki animacji
    al_draw_tinted_bitmap_region(
        texture,
        al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha),
        sourceX, sourceY,
        sourceWidth, sourceHeight,
        0, 0,
        0
    );

    // Przywr�cenie poprzedniej transformacji
    al_use_transform(&transform);
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Sprite.h
// Sprite.h
#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Primitives.h"
#include "Animation.h"

class Sprite {
private:
    ALLEGRO_BITMAP* texture;
    Point2D position;
    Point2D scale;
    float rotation;
    float alpha;

    // Mapy animacji i stanu animacji
    std::map<std::string, Animation*> animations;
    Animation* currentAnimation;

    // �r�d�owy prostok�t dla sprite sheet
    int sourceX;
    int sourceY;
    int sourceWidth;
    int sourceHeight;

public:
    Sprite();
    ~Sprite();

    // Podstawowe ustawienia
    bool loadTexture(const std::string& path);
    void setPosition(float x, float y);
    void setRotation(float angle);
    void setScale(float scaleX, float scaleY);
    void setAlpha(float alpha);
    void setSourceRect(int x, int y, int width, int height);
    void resetSourceRect();

    // Zarz�dzanie animacjami 
    void addAnimation(Animation* animation);
    void playAnimation(const std::string& name);
    void stopAnimation();
    void pauseAnimation();
    void resumeAnimation();
    void updateAnimation(float deltaTime);
    Animation* getCurrentAnimation() const;

    // Gettery
    Point2D getPosition() const;
    Point2D getScale() const;
    float getRotation() const;
    float getAlpha() const;
    int getWidth() const;
    int getHeight() const;

    // Rysowanie
    void draw();
};

#endif // SPRITE_H


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\TextureManager.cpp
// TextureManager.cpp
#include "TextureManager.h"
#include <allegro5/allegro_image.h>

TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager() {
    // Inicjalizacja dodatku do obs�ugi obraz�w
    if (!al_init_image_addon()) {
        logger.error("Failed to initialize image addon!");
    }
}

TextureManager::~TextureManager() {
    unloadAllTextures();
}

TextureManager* TextureManager::getInstance() {
    if (instance == nullptr) {
        instance = new TextureManager();
    }
    return instance;
}

void TextureManager::releaseInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

ALLEGRO_BITMAP* TextureManager::loadTexture(const std::string& path) {
    // Sprawd� czy tekstura ju� jest za�adowana
    if (isTextureLoaded(path)) {
        return textures[path];
    }

    // Za�aduj now� tekstur�
    ALLEGRO_BITMAP* texture = al_load_bitmap(path.c_str());

    if (texture == nullptr) {
        logger.error("Failed to load texture: " + path);
        return nullptr;
    }

    // Zapisz tekstur� w mapie
    textures[path] = texture;
    logger.info("Texture loaded: " + path);

    return texture;
}

ALLEGRO_BITMAP* TextureManager::getTexture(const std::string& path) {
    if (!isTextureLoaded(path)) {
        return loadTexture(path);
    }
    return textures[path];
}

bool TextureManager::isTextureLoaded(const std::string& path) const {
    return textures.find(path) != textures.end();
}

void TextureManager::unloadTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        al_destroy_bitmap(it->second);
        textures.erase(it);
        logger.info("Texture unloaded: " + path);
    }
}

void TextureManager::unloadAllTextures() {
    for (auto& pair : textures) {
        al_destroy_bitmap(pair.second);
        logger.info("Texture unloaded: " + pair.first);
    }
    textures.clear();
}

bool TextureManager::getTextureDimensions(const std::string& path, int& width, int& height) {
    ALLEGRO_BITMAP* texture = getTexture(path);
    if (texture == nullptr) {
        return false;
    }

    width = al_get_bitmap_width(texture);
    height = al_get_bitmap_height(texture);
    return true;
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\TextureManager.h
#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Logger.h"

class TextureManager {
private:
    // Singleton instance
    static TextureManager* instance;

    // Mapa przechowuj�ca tekstury (�cie�ka -> bitmap)
    std::map<std::string, ALLEGRO_BITMAP*> textures;

    // Logger do obs�ugi b��d�w
    Logger logger;

    // Prywatny konstruktor (Singleton)
    TextureManager();

    // Prywatny destruktor
    ~TextureManager();

    // Zablokowanie kopiowania
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

public:
    // Pobranie instancji singletona
    static TextureManager* getInstance();

    // Zwolnienie instancji
    static void releaseInstance();

    // �adowanie tekstury
    ALLEGRO_BITMAP* loadTexture(const std::string& path);

    // Pobranie za�adowanej tekstury
    ALLEGRO_BITMAP* getTexture(const std::string& path);

    // Sprawdzenie czy tekstura jest za�adowana
    bool isTextureLoaded(const std::string& path) const;

    // Zwolnienie pojedynczej tekstury
    void unloadTexture(const std::string& path);

    // Zwolnienie wszystkich tekstur
    void unloadAllTextures();

    // Pobranie wymiar�w tekstury
    bool getTextureDimensions(const std::string& path, int& width, int& height);
};

#endif // TEXTURE_MANAGER_H



## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Transform.cpp
#include "Transform.h"

void Transform::translate(Point2D& point, float dx, float dy) {
    point.setX(point.getX() + dx);
    point.setY(point.getY() + dy);
}

void Transform::scale(Point2D& point, float sx, float sy, const Point2D& center) {
    // Przesuni�cie do punktu (0,0)
    float x = point.getX() - center.getX();
    float y = point.getY() - center.getY();

    // Skalowanie
    x *= sx;
    y *= sy;

    // Przesuni�cie z powrotem
    point.setX(x + center.getX());
    point.setY(y + center.getY());
}

void Transform::rotate(Point2D& point, float angle, const Point2D& center) {
    // Konwersja k�ta na radiany
    float radians = angle * PI / 180.0f;

    // Przesuni�cie do punktu (0,0)
    float x = point.getX() - center.getX();
    float y = point.getY() - center.getY();

    // Rotacja
    float newX = x * cos(radians) - y * sin(radians);
    float newY = x * sin(radians) + y * cos(radians);

    // Przesuni�cie z powrotem
    point.setX(newX + center.getX());
    point.setY(newY + center.getY());
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\engine\Transform.h
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>
#include "Primitives.h"
#define PI 3.14159265358979323846

class Transform {
public:
    static void translate(Point2D& point, float dx, float dy);
    static void scale(Point2D& point, float sx, float sy, const Point2D& center = Point2D(0, 0));
    static void rotate(Point2D& point, float angle, const Point2D& center = Point2D(0, 0));
};

#endif


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Camera.cpp
// src/game/Camera.cpp
#include "Camera.h"
#include "../engine/Engine.h"

Camera::Camera()
    : position(0.0f, 0.0f)
    , zoom(1.0f)
    , target(nullptr)
    , viewportWidth(800)
    , viewportHeight(600)
{
}

Camera::~Camera() {
}

void Camera::update() {
    if (target) {
        // P�ynne �ledzenie celu (gracza)
        Point2D targetPos = target->getPosition();
        position = targetPos;
    }
}

void Camera::beginScene() {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);

    // Przesuni�cie do �rodka ekranu
    al_translate_transform(&transform,
        viewportWidth / 2.0f - position.getX() * zoom,
        viewportHeight / 2.0f - position.getY() * zoom);

    // Skalowanie (zoom)
    al_scale_transform(&transform, zoom, zoom);

    al_use_transform(&transform);
}

void Camera::endScene() {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_use_transform(&transform);
}

void Camera::setPosition(float x, float y) {
    position.setPosition(x, y);
}

void Camera::setTarget(Entity* newTarget) {
    target = newTarget;
}

void Camera::setZoom(float newZoom) {
    zoom = newZoom;
}

void Camera::setViewport(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}

Point2D Camera::getPosition() const {
    return position;
}

float Camera::getZoom() const {
    return zoom;
}

Point2D Camera::worldToScreen(const Point2D& worldPos) const {
    float screenX = (worldPos.getX() - position.getX()) * zoom + viewportWidth / 2.0f;
    float screenY = (worldPos.getY() - position.getY()) * zoom + viewportHeight / 2.0f;
    return Point2D(screenX, screenY);
}

Point2D Camera::screenToWorld(const Point2D& screenPos) const {
    float worldX = (screenPos.getX() - viewportWidth / 2.0f) / zoom + position.getX();
    float worldY = (screenPos.getY() - viewportHeight / 2.0f) / zoom + position.getY();
    return Point2D(worldX, worldY);
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "../engine/Primitives.h"
#include "Entity.h"
#include <allegro5/allegro.h>

class Camera {
private:
    Point2D position;
    float zoom;
    Entity* target;
    int viewportWidth;
    int viewportHeight;

public:
    Camera();
    ~Camera();

    // Podstawowe funkcje
    void update();
    void beginScene();
    void endScene();

    // Settery
    void setPosition(float x, float y);
    void setTarget(Entity* newTarget);
    void setZoom(float newZoom);
    void setViewport(int width, int height);

    // Gettery
    Point2D getPosition() const;
    float getZoom() const;

    // Konwersja wsp�rz�dnych
    Point2D worldToScreen(const Point2D& worldPos) const;
    Point2D screenToWorld(const Point2D& screenPos) const;
};

#endif // CAMERA_H

## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Entity.cpp
// src/game/Entity.cpp
#include "Entity.h"
#include <cmath>

Entity::Entity()
    : sprite(nullptr)
    , position(0.0f, 0.0f)
    , velocity(0.0f, 0.0f)
    , rotation(0.0f)
    , speed(0.0f)
    , isActive(true)
    , tag("")
    , collisionRadius(0.0f)
{
    sprite = new Sprite();
}

Entity::~Entity() {
    destroy();
}

void Entity::update(float deltaTime) {
    if (!isActive) return;

    // Aktualizacja pozycji na podstawie pr�dko�ci
    position.setX(position.getX() + velocity.getX() * deltaTime);
    position.setY(position.getY() + velocity.getY() * deltaTime);

    // Aktualizacja sprite'a
    if (sprite) {
        sprite->setPosition(position.getX(), position.getY());
        sprite->setRotation(rotation);
        sprite->updateAnimation(deltaTime);
    }
}

void Entity::draw() {
    if (!isActive) return;

    if (sprite) {
        sprite->draw();
    }
}

void Entity::destroy() {
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }
    isActive = false;
}

bool Entity::loadResources() {
    // Podstawowa implementacja - do nadpisania w klasach pochodnych
    return true;
}

// Settery
void Entity::setPosition(float x, float y) {
    position.setPosition(x, y);
    if (sprite) {
        sprite->setPosition(x, y);
    }
}

void Entity::setRotation(float angle) {
    rotation = angle;
    if (sprite) {
        sprite->setRotation(angle);
    }
}

void Entity::setVelocity(float x, float y) {
    velocity.setPosition(x, y);
}

void Entity::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Entity::setActive(bool active) {
    isActive = active;
}

void Entity::setTag(const std::string& newTag) {
    tag = newTag;
}

void Entity::setCollisionRadius(float radius) {
    collisionRadius = radius;
}

// Gettery
Point2D Entity::getPosition() const {
    return position;
}

Point2D Entity::getVelocity() const {
    return velocity;
}

float Entity::getRotation() const {
    return rotation;
}

float Entity::getSpeed() const {
    return speed;
}

bool Entity::getIsActive() const {
    return isActive;
}

std::string Entity::getTag() const {
    return tag;
}

float Entity::getCollisionRadius() const {
    return collisionRadius;
}

bool Entity::checkCollision(const Entity* other) const {
    if (!other || !isActive || !other->getIsActive()) return false;

    float dx = position.getX() - other->getPosition().getX();
    float dy = position.getY() - other->getPosition().getY();
    float distance = sqrt(dx * dx + dy * dy);

    return distance < (collisionRadius + other->getCollisionRadius());
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Entity.h
#pragma once
// src/game/Entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include "../engine/Sprite.h"
#include "../engine/Primitives.h"
#include <string>

class Entity {
protected:
    Sprite* sprite;
    Point2D position;
    Point2D velocity;
    float rotation;
    float speed;
    bool isActive;
    std::string tag;

    // Kolizja
    float collisionRadius;

public:
    Entity();
    virtual ~Entity();

    // Podstawowe funkcje
    virtual void update(float deltaTime);
    virtual void draw();
    virtual void destroy();

    // Settery
    void setPosition(float x, float y);
    void setRotation(float angle);
    void setVelocity(float x, float y);
    void setSpeed(float newSpeed);
    void setActive(bool active);
    void setTag(const std::string& newTag);
    void setCollisionRadius(float radius);

    // Gettery
    Point2D getPosition() const;
    Point2D getVelocity() const;
    float getRotation() const;
    float getSpeed() const;
    bool getIsActive() const;
    std::string getTag() const;
    float getCollisionRadius() const;

    // Kolizje
    bool checkCollision(const Entity* other) const;

    // �adowanie zasob�w
    virtual bool loadResources();
};

#endif // ENTITY_H



## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Map.cpp


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Map.h


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Player.cpp
#include "Player.h"
#include <cmath>

Player::Player()
    : Entity()
    , maxSpeed(300.0f)  // Piksele na sekund�
    , acceleration(400.0f)
    , deceleration(800.0f)
    , isMoving(false)
    , health(100.0f)
    , maxHealth(100.0f)
{
    tag = "Player";
    collisionRadius = 30.0f;  // Dostosuj wed�ug rozmiaru sprite'a
}

Player::~Player() {
}

bool Player::loadResources() {
    if (!sprite->loadTexture("assets/textures/hero_idle.png")) {
        return false;
    }

    // Tworzenie animacji idle
    Animation* idleAnimation = new Animation("idle", true);

    // Ustawienia klatek animacji
    const int FRAME_WIDTH = 253;
    const int FRAME_HEIGHT = 216;
    const float FRAME_DURATION = 0.05f;

    // Dodawanie klatek animacji (4 rz�dy po 5 klatek)
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            idleAnimation->addFrame(
                col * FRAME_WIDTH,
                row * FRAME_HEIGHT,
                FRAME_WIDTH,
                FRAME_HEIGHT,
                FRAME_DURATION
            );
        }
    }

    sprite->addAnimation(idleAnimation);
    sprite->playAnimation("idle");

    return true;
}

void Player::handleInput(float deltaTime) {
    Engine* engine = Engine::getInstance();

    // Obliczanie wektora kierunku na podstawie wci�ni�tych klawiszy
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (engine->isKeyDown(ALLEGRO_KEY_W) || engine->isKeyDown(ALLEGRO_KEY_UP)) {
        dirY -= 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_S) || engine->isKeyDown(ALLEGRO_KEY_DOWN)) {
        dirY += 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_A) || engine->isKeyDown(ALLEGRO_KEY_LEFT)) {
        dirX -= 1.0f;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_D) || engine->isKeyDown(ALLEGRO_KEY_RIGHT)) {
        dirX += 1.0f;
    }

    // Normalizacja wektora kierunku
    if (dirX != 0.0f || dirY != 0.0f) {
        float length = sqrt(dirX * dirX + dirY * dirY);
        dirX /= length;
        dirY /= length;
        isMoving = true;
    }
    else {
        isMoving = false;
    }

    // Aktualizacja pr�dko�ci z przyspieszeniem
    if (isMoving) {
        velocity.setX(velocity.getX() + dirX * acceleration * deltaTime);
        velocity.setY(velocity.getY() + dirY * acceleration * deltaTime);

        // Ograniczenie maksymalnej pr�dko�ci
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > maxSpeed) {
            float scale = maxSpeed / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
    }
    else {
        // Hamowanie
        float speed = sqrt(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY());
        if (speed > 0) {
            float scale = std::max(0.0f, speed - deceleration * deltaTime) / speed;
            velocity.setX(velocity.getX() * scale);
            velocity.setY(velocity.getY() * scale);
        }
    }
}


void Player::setCamera(Camera* cam) {
    camera = cam;
}

void Player::lookAtMouse() {
    if (!camera) return;

    Engine* engine = Engine::getInstance();
    Point2D mouseScreen(engine->getMouseX(), engine->getMouseY());
    Point2D mouseWorld = camera->screenToWorld(mouseScreen);

    float dx = mouseWorld.getX() - position.getX();
    float dy = mouseWorld.getY() - position.getY();
    rotation = atan2(dy, dx);
}



void Player::update(float deltaTime) {
    if (!isActive) return;

    handleInput(deltaTime);
    lookAtMouse();

    // Aktualizacja pozycji
    Entity::update(deltaTime);
}

void Player::takeDamage(float amount) {
    health = std::max(0.0f, health - amount);
}

void Player::heal(float amount) {
    health = std::min(maxHealth, health + amount);
}

float Player::getHealth() const {
    return health;
}

float Player::getMaxHealth() const {
    return maxHealth;
}

bool Player::isAlive() const {
    return health > 0;
}


## C:\Users\zyraf\source\repos\PGK-Engine-Reorganized\src\game\Player.h
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "../engine/Engine.h"
#include "../game/Camera.h"

class Player : public Entity {
private:
    float maxSpeed;
    float acceleration;
    float deceleration;
    bool isMoving;
    float health;
    float maxHealth;
    Camera* camera;

public:
    Player();
    virtual ~Player();

    // Nadpisane metody z klasy Entity
    virtual void update(float deltaTime) override;
    virtual bool loadResources() override;

    // Specyficzne metody dla gracza
    void handleInput(float deltaTime);
    void lookAtMouse();
    void takeDamage(float amount);
    void heal(float amount);

    // Gettery
    float getHealth() const;
    float getMaxHealth() const;
    bool isAlive() const;

    void setCamera(Camera* cam);
};

#endif // PLAYER_H




