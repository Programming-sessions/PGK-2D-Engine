/**
* @file Engine.h
* @brief G��wny silnik gry wykorzystuj�cy bibliotek� Allegro 5
*
* Klasa Engine implementuje wzorzec Singleton i zapewnia podstawow�
* funkcjonalno�� silnika gry, w tym zarz�dzanie oknem, obs�ug�
* wej�cia/wyj�cia oraz g��wn� p�tl� gry.
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
 * @brief G��wna klasa silnika gry
 *
 * Zapewnia podstawow� funkcjonalno�� silnika, w tym:
 * - Zarz�dzanie oknem gry
 * - Obs�ug� wej�cia (klawiatura, mysz)
 * - P�tl� g��wn� gry
 * - Podstawowe operacje graficzne
 */
class Engine {
private:
private:
    static Engine* instance;    ///< Instancja Singletona
    Engine();                   ///< Prywatny konstruktor (Singleton)

    // Parametry podstawowe
    int screenWidth;           ///< Szeroko�� okna w pikselach
    int screenHeight;          ///< Wysoko�� okna w pikselach
    bool fullscreen;           ///< Tryb pe�noekranowy
    int fps;                   ///< Liczba klatek na sekund�
    std::string title;         ///< Tytu� okna
    ALLEGRO_COLOR clearColor;  ///< Kolor czyszczenia ekranu

    // Zasoby Allegro
    ALLEGRO_DISPLAY* display;      ///< Wska�nik na okno
    ALLEGRO_EVENT_QUEUE* eventQueue; ///< Kolejka zdarze�
    ALLEGRO_TIMER* timer;          ///< Timer do kontroli FPS

    // Stan wej�cia
    bool keys[ALLEGRO_KEY_MAX];    ///< Tablica stanu klawiszy
    bool mouseButtons[3];          ///< Stan przycisk�w myszy
    int mouseX, mouseY;            ///< Pozycja kursora myszy

    // Stan silnika
    bool running;                  ///< Czy silnik jest uruchomiony
    bool redraw;                   ///< Flaga od�wie�ania ekranu

    Logger logger;                 ///< System logowania

    /**
    * @brief Inicjalizuje bibliotek� Allegro
    * @return true je�li inicjalizacja si� powiod�a
    */
    bool initAllegro();

    /**
    * @brief Inicjalizuje okno wy�wietlania
    * @return true je�li inicjalizacja si� powiod�a
    */
    bool initDisplay();

    /**
    * @brief Inicjalizuje obs�ug� wej�cia
    * @return true je�li inicjalizacja si� powiod�a
    */
    bool initInput();

    /**
    * @brief Inicjalizuje timery
    * @return true je�li inicjalizacja si� powiod�a
    */
    bool initTimers();


public:
    /**
    * @brief Pobiera instancj� silnika (Singleton)
    * @return Wska�nik na instancj� silnika
    */
    static Engine* getInstance();

    /**
    * @brief Destruktor
    */
    ~Engine();

    /**
    * @brief Inicjalizuje silnik
    * @return true je�li inicjalizacja si� powiod�a
    */
    bool init();

    /**
    * @brief Zamyka silnik i zwalnia zasoby
    */
    void shutdown();

    // Konfiguracja
    /**
    * @brief Ustawia rozmiar okna
    * @param width Szeroko�� okna w pikselach
    * @param height Wysoko�� okna w pikselach
    */
    void setScreenSize(int width, int height);

    /**
    * @brief Prze��cza tryb pe�noekranowy
    * @param fullscreen true dla trybu pe�noekranowego
    */
    void setFullscreen(bool fullscreen);

    /**
    * @brief Ustawia liczb� klatek na sekund�
    * @param fps Docelowa liczba FPS
    */
    void setFPS(int fps);

    /**
    * @brief Ustawia tytu� okna
    * @param title Nowy tytu� okna
    */
    void setTitle(const std::string& title);

    /**
    * @brief Ustawia kolor czyszczenia ekranu
    * @param color Kolor w formacie ALLEGRO_COLOR
    */
    void setClearColor(ALLEGRO_COLOR color);

    /**
    * @brief Uruchamia g��wn� p�tl� gry
    * @param updateCallback Funkcja aktualizuj�ca logik� gry
    * @param renderCallback Funkcja renderuj�ca grafik�
    *
    * Funkcja updateCallback otrzymuje czas delta (w sekundach) jako parametr.
    * Funkcja renderCallback jest wywo�ywana po aktualizacji logiki.
    */
    void startGameLoop(std::function<void(float)> updateCallback,
        std::function<void()> renderCallback);

    /**
    * @brief Przetwarza zdarzenia wej�cia/wyj�cia
    */
    void processEvents();

    // Obs�uga urz�dze� wej�cia
    /**
    * @brief Sprawdza czy klawisz jest wci�ni�ty
    * @param keycode Kod klawisza Allegro
    * @return true je�li klawisz jest wci�ni�ty
    */
    bool isKeyDown(int keycode);

    /**
    * @brief Sprawdza czy przycisk myszy jest wci�ni�ty
    * @param button Numer przycisku (0-lewy, 1-prawy, 2-�rodkowy)
    * @return true je�li przycisk jest wci�ni�ty
    */
    bool isMouseButtonDown(int button);

    /**
    * @brief Pobiera pozycj� X kursora myszy
    * @return Wsp�rz�dna X kursora
    */
    int getMouseX();

    /**
    * @brief Pobiera pozycj� Y kursora myszy
    * @return Wsp�rz�dna Y kursora
    */
    int getMouseY();

    // Operacje graficzne
    /**
    * @brief Czy�ci ekran kolorem t�a
    */
    void clearScreen();

    /**
    * @brief Czy�ci bitmap� zadanym kolorem
    * @param bitmap Bitmapa do wyczyszczenia
    * @param color Kolor czyszczenia
    */
    void clearBitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color);

    /**
    * @brief Zamienia bufory wy�wietlania
    */
    void flipDisplay();

    /**
     * @brief Pobiera szeroko�� ekranu
     * @return Szeroko�� w pikselach
     */
    int getScreenWidth() const;

    /**
     * @brief Pobiera wysoko�� ekranu
     * @return Wysoko�� w pikselach
     */
    int getScreenHeight() const;

    /**
     * @brief Sprawdza czy silnik jest uruchomiony
     * @return true je�li silnik dzia�a
     */
    bool isRunning() const;

    /**
     * @brief Pobiera czas mi�dzy klatkami
     * @return Czas w sekundach
     */
    float getDeltaTime() const;

    /**
     * @brief Pobiera wska�nik na okno Allegro
     * @return Wska�nik na ALLEGRO_DISPLAY
     */
    ALLEGRO_DISPLAY* getDisplay();

    /**
     * @brief Pobiera referencj� do loggera
     * @return Referencja do obiektu Logger
     */
    Logger& getLogger() { return logger; }
};

#endif // ENGINE_H