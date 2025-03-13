# README.md - Implementacja klasy Engine (engine.cpp)

## Spis treści
- [Wprowadzenie](#wprowadzenie)
- [Wzorzec projektowy Singleton](#wzorzec-projektowy-singleton)
- [Struktura klasy Engine](#struktura-klasy-engine)
- [Inicjalizacja biblioteki graficznej](#inicjalizacja-biblioteki-graficznej)
- [Główna pętla gry](#główna-pętla-gry)
- [Obsługa urządzeń wejścia](#obsługa-urządzeń-wejścia)
- [Zarządzanie zasobami graficznymi](#zarządzanie-zasobami-graficznymi)
- [Obsługa błędów](#obsługa-błędów)
- [Parametryzacja silnika](#parametryzacja-silnika)
- [Przykłady użycia](#przykłady-użycia)
- [Optymalizacje i dobre praktyki](#optymalizacje-i-dobre-praktyki)
- [Rozszerzenia](#możliwe-rozszerzenia)

## Wprowadzenie

Plik `engine.cpp` zawiera implementację klasy `Engine`, która stanowi fundament silnika gry 2D. Jest to główny komponent odpowiedzialny za zarządzanie podstawowymi funkcjonalnościami niezbędnymi do działania gry wideo, takimi jak:

- Inicjalizacja i konfiguracja biblioteki graficznej
- Zarządzanie główną pętlą gry
- Obsługa urządzeń wejścia (klawiatura, mysz)
- Zarządzanie zasobami graficznymi
- Obsługa błędów i diagnostyka
- Zamykanie aplikacji i zwalnianie zasobów

Klasa `Engine` została zaprojektowana zgodnie z wzorcem projektowym **Singleton**, co zapewnia istnienie tylko jednej instancji silnika w trakcie działania programu.

## Wzorzec projektowy Singleton

Implementacja wzorca Singleton w klasie `Engine` opiera się na następujących elementach:

```cpp
class Engine {
private:
    // Jedyna instancja klasy Engine
    static Engine* instance;
    
    // Prywatny konstruktor uniemożliwiający tworzenie wielu instancji
    Engine();
    
public:
    // Metoda dostępowa do jedynej instancji
    static Engine* getInstance();
    
    // Usunięty konstruktor kopiujący i operator przypisania
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    
    // Destruktor
    ~Engine();
    
    // Pozostałe metody...
};

// Implementacja w engine.cpp
Engine* Engine::instance = nullptr;

Engine* Engine::getInstance() {
    if (instance == nullptr) {
        instance = new Engine();
    }
    return instance;
}
```

Dzięki takiemu podejściu zapewniona jest:
- Jednoczesna egzystencja tylko jednej instancji klasy `Engine`
- Globalny dostęp do tej instancji poprzez metodę `getInstance()`
- Kontrolowany czas życia obiektu

## Struktura klasy Engine

Klasa `Engine` posiada następującą strukturę wewnętrzną:

1. **Atrybuty konfiguracyjne**:
   - Parametry ekranu (szerokość, wysokość, tryb pełnoekranowy)
   - Liczba klatek na sekundę (FPS)
   - Tytuł okna
   - Kolor tła

2. **Zasoby biblioteki graficznej**:
   - Uchwyt do okna wyświetlania
   - Kolejka zdarzeń
   - Czasomierz (timer)
   - Czcionki
   - Inne zasoby specyficzne dla używanej biblioteki

3. **Stan urządzeń wejścia**:
   - Tablica stanów klawiszy
   - Tablica stanów przycisków myszy
   - Pozycja kursora myszy
   - Flagi stanu urządzeń wejścia

4. **Stan silnika**:
   - Flaga działania silnika
   - Flaga odświeżania ekranu
   - Czas delta (czas pomiędzy klatkami)
   - Licznik FPS

5. **Obsługa błędów**:
   - Integracja z systemem logowania
   - Mechanizmy przechwytywania i obsługi wyjątków

## Inicjalizacja biblioteki graficznej

Funkcja inicjalizacji biblioteki graficznej jest jednym z kluczowych elementów silnika:

```cpp
bool Engine::initAllegro() {
    // Inicjalizacja głównej biblioteki
    if (!al_init()) {
        logger.fatal("Nie można zainicjować biblioteki Allegro");
        return false;
    }
    
    // Inicjalizacja podsystemów
    if (!al_init_primitives_addon()) {
        logger.fatal("Nie można zainicjować dodatku Allegro Primitives");
        return false;
    }
    
    if (!al_init_image_addon()) {
        logger.fatal("Nie można zainicjować dodatku Allegro Image");
        return false;
    }
    
    if (!al_init_font_addon() || !al_init_ttf_addon()) {
        logger.fatal("Nie można zainicjować dodatku Allegro Font/TTF");
        return false;
    }
    
    logger.info("Biblioteka graficzna została pomyślnie zainicjowana");
    return true;
}
```

Metoda `init()` koordynuje cały proces inicjalizacji, wywołując sekwencyjnie metody inicjujące poszczególne komponenty:

```cpp
bool Engine::init() {
    // Inicjalizacja loggera
    if (!logger.init("engine.log")) {
        return false;
    }
    
    logger.info("Inicjalizacja silnika Engine");
    
    // Inicjalizacja biblioteki graficznej i jej podsystemów
    if (!initAllegro()) {
        return false;
    }
    
    // Inicjalizacja wyświetlania
    if (!initDisplay()) {
        return false;
    }
    
    // Inicjalizacja obsługi urządzeń wejścia
    if (!initInput()) {
        return false;
    }
    
    // Inicjalizacja czasomierzy
    if (!initTimers()) {
        return false;
    }
    
    logger.info("Silnik Engine został pomyślnie zainicjowany");
    running = true;
    return true;
}
```

## Główna pętla gry

Główna pętla gry implementowana jest w metodzie `startGameLoop()`:

```cpp
void Engine::startGameLoop(std::function<void(float)> updateCallback, 
                          std::function<void()> renderCallback) {
    if (!running) {
        logger.error("Próba uruchomienia pętli gry na niezainicjowanym silniku");
        return;
    }
    
    logger.info("Rozpoczęcie głównej pętli gry");
    
    float deltaTime = 0.0f;
    double lastTime = al_get_time();
    
    al_start_timer(timer);
    
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);
        
        // Obsługa zdarzeń
        if (event.type == ALLEGRO_EVENT_TIMER) {
            // Obliczenie czasu delta
            double currentTime = al_get_time();
            deltaTime = (float)(currentTime - lastTime);
            lastTime = currentTime;
            
            // Aktualizacja logiki gry
            if (updateCallback) {
                updateCallback(deltaTime);
            }
            
            redraw = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else {
            // Obsługa innych zdarzeń (klawiatura, mysz)
            processInputEvent(event);
        }
        
        // Renderowanie
        if (redraw && al_is_event_queue_empty(eventQueue)) {
            redraw = false;
            
            clearScreen();
            
            // Wywołanie funkcji renderującej
            if (renderCallback) {
                renderCallback();
            }
            
            flipDisplay();
        }
    }
    
    logger.info("Zakończenie głównej pętli gry");
}
```

### Kluczowe elementy pętli gry

1. **Czas Delta**:
   - Obliczanie czasu, który upłynął od poprzedniej klatki
   - Wykorzystanie go do aktualizacji logiki gry niezależnej od liczby FPS

2. **Obsługa zdarzeń**:
   - Zdarzenia czasomierza (timer)
   - Zdarzenia zamknięcia okna
   - Zdarzenia urządzeń wejścia

3. **Aktualizacja logiki**:
   - Wywoływanie funkcji przekazanej jako callback `updateCallback`
   - Przekazywanie czasu delta do aktualizacji

4. **Renderowanie**:
   - Czyszczenie ekranu
   - Wywołanie funkcji renderującej `renderCallback`
   - Wyświetlenie zbuforowanej zawartości

## Obsługa urządzeń wejścia

### Implementacja obsługi klawiatury

```cpp
bool Engine::initInput() {
    if (!al_install_keyboard()) {
        logger.fatal("Nie można zainicjować obsługi klawiatury");
        return false;
    }
    
    if (!al_install_mouse()) {
        logger.fatal("Nie można zainicjować obsługi myszy");
        return false;
    }
    
    // Zerowanie stanów klawiszy i przycisków myszy
    memset(keys, 0, sizeof(keys));
    memset(mouseButtons, 0, sizeof(mouseButtons));
    mouseX = mouseY = 0;
    
    logger.info("Obsługa urządzeń wejścia została pomyślnie zainicjowana");
    return true;
}

void Engine::processInputEvent(const ALLEGRO_EVENT& event) {
    // Obsługa klawiatury
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        keys[event.keyboard.keycode] = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        keys[event.keyboard.keycode] = false;
    }
    // Obsługa myszy
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.mouse.button <= 3) {
            mouseButtons[event.mouse.button - 1] = true;
        }
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (event.mouse.button <= 3) {
            mouseButtons[event.mouse.button - 1] = false;
        }
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        mouseX = event.mouse.x;
        mouseY = event.mouse.y;
    }
}

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
```

## Zarządzanie zasobami graficznymi

```cpp
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
```

## Obsługa błędów

Obsługa błędów w klasie `Engine` jest realizowana poprzez integrację z klasą `Logger`:

```cpp
bool Engine::initDisplay() {
    // Ustawienie trybu wyświetlania
    if (fullscreen) {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    }
    
    // Utworzenie okna
    display = al_create_display(screenWidth, screenHeight);
    if (!display) {
        logger.fatal("Nie można utworzyć okna wyświetlania");
        return false;
    }
    
    // Ustawienie tytułu okna
    al_set_window_title(display, title.c_str());
    
    // Rejestracja źródła zdarzeń
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    
    logger.info("Okno wyświetlania zostało pomyślnie utworzone");
    return true;
}
```

## Parametryzacja silnika

Silnik oferuje szereg metod umożliwiających jego konfigurację:

```cpp
void Engine::setScreenSize(int width, int height) {
    this->screenWidth = width;
    this->screenHeight = height;
    
    // Jeśli okno już istnieje, należy je zmodyfikować
    if (display) {
        al_resize_display(display, width, height);
    }
}

void Engine::setFullscreen(bool fullscreen) {
    this->fullscreen = fullscreen;
    
    // Jeśli okno już istnieje, należy zmienić jego tryb
    if (display) {
        if (fullscreen) {
            al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
        } else {
            al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
        }
    }
}

void Engine::setFPS(int fps) {
    this->fps = fps;
    
    // Jeśli timer już istnieje, należy go zmodyfikować
    if (timer) {
        al_destroy_timer(timer);
        timer = al_create_timer(1.0 / fps);
        al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    }
}

void Engine::setTitle(const std::string& title) {
    this->title = title;
    
    // Jeśli okno już istnieje, należy zaktualizować jego tytuł
    if (display) {
        al_set_window_title(display, title.c_str());
    }
}

void Engine::setClearColor(ALLEGRO_COLOR color) {
    this->clearColor = color;
}
```

## Przykłady użycia

### Przykład prostej aplikacji

```cpp
#include "Engine.h"
#include <allegro5/allegro_primitives.h>

// Zmienne globalne dla demonstracji
float playerX = 400.0f;
float playerY = 300.0f;
float playerSpeed = 200.0f;

// Funkcje callbackowe
void update(float deltaTime);
void render();

int main() {
    // Pobranie instancji silnika
    Engine* engine = Engine::getInstance();
    
    // Konfiguracja silnika
    engine->setScreenSize(800, 600);
    engine->setFullscreen(false);
    engine->setFPS(60);
    engine->setTitle("Demonstracja silnika");
    engine->setClearColor(al_map_rgb(0, 0, 0));
    
    // Inicjalizacja silnika
    if (!engine->init()) {
        return -1;
    }
    
    // Uruchomienie głównej pętli gry
    engine->startGameLoop(update, render);
    
    // Sprzątanie
    engine->shutdown();
    
    return 0;
}

// Funkcja aktualizująca logikę gry
void update(float deltaTime) {
    Engine* engine = Engine::getInstance();
    
    // Obsługa klawiatury
    if (engine->isKeyDown(ALLEGRO_KEY_UP)) {
        playerY -= playerSpeed * deltaTime;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_DOWN)) {
        playerY += playerSpeed * deltaTime;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_LEFT)) {
        playerX -= playerSpeed * deltaTime;
    }
    if (engine->isKeyDown(ALLEGRO_KEY_RIGHT)) {
        playerX += playerSpeed * deltaTime;
    }
    
    // Obsługa wyjścia
    if (engine->isKeyDown(ALLEGRO_KEY_ESCAPE)) {
        engine->shutdown();
    }
}

// Funkcja renderująca
void render() {
    // Rysowanie gracza
    al_draw_filled_circle(playerX, playerY, 20, al_map_rgb(255, 0, 0));
    
    // Rysowanie pozycji myszy
    Engine* engine = Engine::getInstance();
    int mouseX = engine->getMouseX();
    int mouseY = engine->getMouseY();
    
    if (engine->isMouseButtonDown(0)) {
        al_draw_filled_circle(mouseX, mouseY, 10, al_map_rgb(0, 255, 0));
    } else {
        al_draw_circle(mouseX, mouseY, 10, al_map_rgb(255, 255, 255), 2);
    }
}
```

## Optymalizacje i dobre praktyki

1. **Zarządzanie zasobami**:
   - Wszystkie zasoby tworzone są w metodach inicjalizacyjnych
   - Wszystkie zasoby są zwalniane w metodzie `shutdown()` lub destruktorze

2. **Obsługa wyjątków**:
   - Metody silnika sprawdzają poprawność operacji
   - Błędy są logowane za pomocą klasy `Logger`
   - W przypadku krytycznych błędów silnik informuje o nich i może się bezpiecznie zamknąć

3. **Niezależność od czasu**:
   - Wykorzystanie czasu delta do aktualizacji logiki
   - Zapewnienie stałej prędkości gry niezależnie od liczby FPS

4. **Zabezpieczenia**:
   - Sprawdzanie zakresów indeksów tablic (np. klawiszy, przycisków myszy)
   - Weryfikacja poprawności wskaźników przed ich użyciem
   - Zabezpieczenie przed wielokrotną inicjalizacją lub deinicjalizacją

## Możliwe rozszerzenia

Klasa `Engine` może być rozszerzona o dodatkowe funkcjonalności:

1. **System zasobów**:
   - Menedżer tekstur
   - Menedżer dźwięków
   - Menedżer czcionek

2. **System scen/stanów**:
   - Przełączanie między różnymi stanami gry (menu, rozgrywka, pauza)
   - Obsługa przejść między scenami

3. **System cząsteczek**:
   - Efekty wizualne
   - Systemy cząsteczkowe

4. **Fizyka**:
   - Wykrywanie kolizji
   - Symulacja fizyki

5. **Networking**:
   - Obsługa gry wieloosobowej
   - Synchronizacja stanu gry

6. **Wsparcie dla kontrolerów**:
   - Obsługa padów
   - Konfigurowanie sterowania

---

© 2025 | Opracowano na potrzeby projektu silnika 2D dla Politechniki Świętokrzyskiej 
