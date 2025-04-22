/**
* @file Engine.h
* @brief G³ówny silnik gry wykorzystuj¹cy bibliotekê Allegro 5
*
* Klasa Engine implementuje wzorzec Singleton i zapewnia podstawow¹
* funkcjonalnoœæ silnika gry, w tym zarz¹dzanie oknem, obs³ugê
* wejœcia/wyjœcia oraz g³ówn¹ pêtlê gry.
*/
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

 /**
 * @brief G³ówna klasa silnika gry
 *
 * Zapewnia podstawow¹ funkcjonalnoœæ silnika, w tym:
 * - Zarz¹dzanie oknem gry
 * - Obs³ugê wejœcia (klawiatura, mysz)
 * - Pêtlê g³ówn¹ gry
 * - Podstawowe operacje graficzne
 */
class Engine {
private:
private:
    static Engine* instance;    ///< Instancja Singletona
    Engine();                   ///< Prywatny konstruktor (Singleton)

    // Parametry podstawowe
    int screenWidth;           ///< Szerokoœæ okna w pikselach
    int screenHeight;          ///< Wysokoœæ okna w pikselach
    bool fullscreen;           ///< Tryb pe³noekranowy
    int fps;                   ///< Liczba klatek na sekundê
    std::string title;         ///< Tytu³ okna
    ALLEGRO_COLOR clearColor;  ///< Kolor czyszczenia ekranu

    // Zasoby Allegro
    ALLEGRO_DISPLAY* display;      ///< WskaŸnik na okno
    ALLEGRO_EVENT_QUEUE* eventQueue; ///< Kolejka zdarzeñ
    ALLEGRO_TIMER* timer;          ///< Timer do kontroli FPS

    // Stan wejœcia
    bool keys[ALLEGRO_KEY_MAX];    ///< Tablica stanu klawiszy
    bool mouseButtons[3];          ///< Stan przycisków myszy
    int mouseX, mouseY;            ///< Pozycja kursora myszy

    // Stan silnika
    bool running;                  ///< Czy silnik jest uruchomiony
    bool redraw;                   ///< Flaga odœwie¿ania ekranu

    Logger logger;                 ///< System logowania

    /**
    * @brief Inicjalizuje bibliotekê Allegro
    * @return true jeœli inicjalizacja siê powiod³a
    */
    bool initAllegro();

    /**
    * @brief Inicjalizuje okno wyœwietlania
    * @return true jeœli inicjalizacja siê powiod³a
    */
    bool initDisplay();

    /**
    * @brief Inicjalizuje obs³ugê wejœcia
    * @return true jeœli inicjalizacja siê powiod³a
    */
    bool initInput();

    /**
    * @brief Inicjalizuje timery
    * @return true jeœli inicjalizacja siê powiod³a
    */
    bool initTimers();


public:
    /**
    * @brief Pobiera instancjê silnika (Singleton)
    * @return WskaŸnik na instancjê silnika
    */
    static Engine* getInstance();

    /**
    * @brief Destruktor
    */
    ~Engine();

    /**
    * @brief Inicjalizuje silnik
    * @return true jeœli inicjalizacja siê powiod³a
    */
    bool init();

    /**
    * @brief Zamyka silnik i zwalnia zasoby
    */
    void shutdown();

    // Konfiguracja
    /**
    * @brief Ustawia rozmiar okna
    * @param width Szerokoœæ okna w pikselach
    * @param height Wysokoœæ okna w pikselach
    */
    void setScreenSize(int width, int height);

    /**
    * @brief Prze³¹cza tryb pe³noekranowy
    * @param fullscreen true dla trybu pe³noekranowego
    */
    void setFullscreen(bool fullscreen);

    /**
    * @brief Ustawia liczbê klatek na sekundê
    * @param fps Docelowa liczba FPS
    */
    void setFPS(int fps);

    /**
    * @brief Ustawia tytu³ okna
    * @param title Nowy tytu³ okna
    */
    void setTitle(const std::string& title);

    /**
    * @brief Ustawia kolor czyszczenia ekranu
    * @param color Kolor w formacie ALLEGRO_COLOR
    */
    void setClearColor(ALLEGRO_COLOR color);

    /**
    * @brief Uruchamia g³ówn¹ pêtlê gry
    * @param updateCallback Funkcja aktualizuj¹ca logikê gry
    * @param renderCallback Funkcja renderuj¹ca grafikê
    *
    * Funkcja updateCallback otrzymuje czas delta (w sekundach) jako parametr.
    * Funkcja renderCallback jest wywo³ywana po aktualizacji logiki.
    */
    void startGameLoop(std::function<void(float)> updateCallback,
        std::function<void()> renderCallback);

    /**
    * @brief Przetwarza zdarzenia wejœcia/wyjœcia
    */
    void processEvents();

    // Obs³uga urz¹dzeñ wejœcia
    /**
    * @brief Sprawdza czy klawisz jest wciœniêty
    * @param keycode Kod klawisza Allegro
    * @return true jeœli klawisz jest wciœniêty
    */
    bool isKeyDown(int keycode);

    /**
    * @brief Sprawdza czy przycisk myszy jest wciœniêty
    * @param button Numer przycisku (0-lewy, 1-prawy, 2-œrodkowy)
    * @return true jeœli przycisk jest wciœniêty
    */
    bool isMouseButtonDown(int button);

    /**
    * @brief Pobiera pozycjê X kursora myszy
    * @return Wspó³rzêdna X kursora
    */
    int getMouseX();

    /**
    * @brief Pobiera pozycjê Y kursora myszy
    * @return Wspó³rzêdna Y kursora
    */
    int getMouseY();

    // Operacje graficzne
    /**
    * @brief Czyœci ekran kolorem t³a
    */
    void clearScreen();

    /**
    * @brief Czyœci bitmapê zadanym kolorem
    * @param bitmap Bitmapa do wyczyszczenia
    * @param color Kolor czyszczenia
    */
    void clearBitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color);

    /**
    * @brief Zamienia bufory wyœwietlania
    */
    void flipDisplay();

    /**
     * @brief Pobiera szerokoœæ ekranu
     * @return Szerokoœæ w pikselach
     */
    int getScreenWidth() const;

    /**
     * @brief Pobiera wysokoœæ ekranu
     * @return Wysokoœæ w pikselach
     */
    int getScreenHeight() const;

    /**
     * @brief Sprawdza czy silnik jest uruchomiony
     * @return true jeœli silnik dzia³a
     */
    bool isRunning() const;

    /**
     * @brief Pobiera czas miêdzy klatkami
     * @return Czas w sekundach
     */
    float getDeltaTime() const;

    /**
     * @brief Pobiera wskaŸnik na okno Allegro
     * @return WskaŸnik na ALLEGRO_DISPLAY
     */
    ALLEGRO_DISPLAY* getDisplay();

    /**
     * @brief Pobiera referencjê do loggera
     * @return Referencja do obiektu Logger
     */
    Logger& getLogger() { return logger; }
};

#endif // ENGINE_H