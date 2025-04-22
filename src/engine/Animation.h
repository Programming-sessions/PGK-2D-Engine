/**
 * @file Animation.h
 * @brief Klasa zarz¹dzaj¹ca animacj¹ 2D wykorzystuj¹c¹ sprite sheet
 *
 * Implementuje system animacji oparty na klatkach (frames) pobieranych
 * ze sprite sheetu. Umo¿liwia kontrolê odtwarzania, zapêtlanie
 * oraz zarz¹dzanie czasem trwania poszczególnych klatek.
 */

#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>

 /**
  * @brief Struktura przechowuj¹ca dane pojedynczej klatki animacji
  *
  * Zawiera informacje o po³o¿eniu klatki w sprite sheet,
  * jej wymiarach oraz czasie trwania.
  */
struct AnimationFrame {
    int x;          ///< Pozycja X klatki w sprite sheet
    int y;          ///< Pozycja Y klatki w sprite sheet
    int width;      ///< Szerokoœæ klatki
    int height;     ///< Wysokoœæ klatki
    float duration; ///< Czas trwania klatki w sekundach

    /**
     * @brief Konstruktor struktury AnimationFrame
     * @param x Pozycja X klatki w sprite sheet
     * @param y Pozycja Y klatki w sprite sheet
     * @param width Szerokoœæ klatki
     * @param height Wysokoœæ klatki
     * @param duration Czas trwania klatki w sekundach
     */
    AnimationFrame(int x, int y, int width, int height, float duration)
        : x(x), y(y), width(width), height(height), duration(duration) {
    }
};

/**
 * @brief Klasa zarz¹dzaj¹ca animacj¹ 2D
 *
 * Umo¿liwia tworzenie i kontrolowanie animacji sk³adaj¹cej siê
 * z sekwencji klatek. Obs³uguje odtwarzanie, pauzowanie, zatrzymywanie
 * oraz zapêtlanie animacji.
 */
class Animation {
private:
    std::string name;                    ///< Nazwa animacji
    std::vector<AnimationFrame> frames;  ///< Wektor klatek animacji
    float totalDuration;                 ///< Ca³kowity czas trwania animacji
    bool isLooping;                      ///< Flaga okreœlaj¹ca czy animacja jest zapêtlona

    int currentFrameIndex;               ///< Indeks aktualnie wyœwietlanej klatki
    float currentFrameTime;              ///< Czas spêdzony na aktualnej klatce
    bool isPlaying;                      ///< Flaga okreœlaj¹ca czy animacja jest odtwarzana

public:
    /**
     * @brief Konstruktor klasy Animation
     * @param name Nazwa animacji
     * @param looping Czy animacja ma byæ zapêtlona (domyœlnie true)
     */
    Animation(const std::string& name, bool looping = true);

    /**
     * @brief Dodaje now¹ klatkê do animacji
     * @param x Pozycja X klatki w sprite sheet
     * @param y Pozycja Y klatki w sprite sheet
     * @param width Szerokoœæ klatki
     * @param height Wysokoœæ klatki
     * @param duration Czas trwania klatki w sekundach
     */
    void addFrame(int x, int y, int width, int height, float duration);

    /**
     * @brief Pobiera aktualnie wyœwietlan¹ klatkê
     * @return Referencja do aktualnej klatki animacji
     */
    const AnimationFrame& getCurrentFrame() const;

    /**
     * @brief Rozpoczyna odtwarzanie animacji
     */
    void play();

    /**
     * @brief Wstrzymuje odtwarzanie animacji
     */
    void pause();

    /**
     * @brief Zatrzymuje animacjê i resetuje do pierwszej klatki
     */
    void stop();

    /**
     * @brief Resetuje animacjê do pierwszej klatki bez zatrzymywania
     */
    void reset();

    /**
     * @brief Aktualizuje stan animacji
     * @param deltaTime Czas (w sekundach) od ostatniej aktualizacji
     */
    void update(float deltaTime);

    /**
     * @brief Sprawdza czy animacja zosta³a zakoñczona
     * @return true jeœli animacja siê zakoñczy³a, false w przeciwnym razie
     * @note Dla zapêtlonych animacji zawsze zwraca false
     */
    bool isFinished() const;

    /**
     * @brief Pobiera nazwê animacji
     * @return Nazwa animacji
     */
    std::string getName() const;

    /**
     * @brief Sprawdza czy animacja jest zapêtlona
     * @return true jeœli animacja jest zapêtlona, false w przeciwnym razie
     */
    bool getIsLooping() const;

    /**
     * @brief Sprawdza czy animacja jest obecnie odtwarzana
     * @return true jeœli animacja jest odtwarzana, false w przeciwnym razie
     */
    bool getIsPlaying() const;

    /**
     * @brief Ustawia tryb zapêtlenia animacji
     * @param loop true aby w³¹czyæ zapêtlenie, false aby wy³¹czyæ
     */
    void setLooping(bool loop);
};

#endif // ANIMATION_H


