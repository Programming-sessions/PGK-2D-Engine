#include "Engine.h"
#include <iostream>
#include <allegro5/allegro_color.h>

// Inicjalizacja statycznego wskaŸnika
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
    title("Allegro Engine"),
    display(nullptr),
    eventQueue(nullptr),
    timer(nullptr),
    running(false),
    redraw(false),
    mouseX(0),
    mouseY(0) {

    clearColor = al_map_rgb(0, 0, 0);

    // Inicjalizacja stanów klawiszy i myszy
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

    // Inicjalizacja poszczególnych komponentów
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
    // Ustawienie parametrów wyœwietlania
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

            // Wywo³anie funkcji aktualizuj¹cej logikê gry
            if (updateCallback) {
                updateCallback(deltaTime);
            }

            // Czyszczenie ekranu
            clearScreen();

            // Wywo³anie funkcji renderuj¹cej
            if (renderCallback) {
                renderCallback();
            }

            // Zamiana buforów
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

    // Zniszczenie zasobów Allegro w odwrotnej kolejnoœci od utworzenia
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

    // Deinicjalizacja dodatków Allegro
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();

    logger.info("Engine shutdown completed");
    logger.close();
}

// Obs³uga urz¹dzeñ wejœcia
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