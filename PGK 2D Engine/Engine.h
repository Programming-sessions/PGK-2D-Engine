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
    bool isKeyPressed(int keycode);
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