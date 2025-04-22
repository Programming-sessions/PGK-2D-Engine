/**
 * @file Animation.h
 * @brief Klasa zarz�dzaj�ca animacj� 2D wykorzystuj�c� sprite sheet
 *
 * Implementuje system animacji oparty na klatkach (frames) pobieranych
 * ze sprite sheetu. Umo�liwia kontrol� odtwarzania, zap�tlanie
 * oraz zarz�dzanie czasem trwania poszczeg�lnych klatek.
 */

#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>

 /**
  * @brief Struktura przechowuj�ca dane pojedynczej klatki animacji
  *
  * Zawiera informacje o po�o�eniu klatki w sprite sheet,
  * jej wymiarach oraz czasie trwania.
  */
struct AnimationFrame {
    int x;          ///< Pozycja X klatki w sprite sheet
    int y;          ///< Pozycja Y klatki w sprite sheet
    int width;      ///< Szeroko�� klatki
    int height;     ///< Wysoko�� klatki
    float duration; ///< Czas trwania klatki w sekundach

    /**
     * @brief Konstruktor struktury AnimationFrame
     * @param x Pozycja X klatki w sprite sheet
     * @param y Pozycja Y klatki w sprite sheet
     * @param width Szeroko�� klatki
     * @param height Wysoko�� klatki
     * @param duration Czas trwania klatki w sekundach
     */
    AnimationFrame(int x, int y, int width, int height, float duration)
        : x(x), y(y), width(width), height(height), duration(duration) {
    }
};

/**
 * @brief Klasa zarz�dzaj�ca animacj� 2D
 *
 * Umo�liwia tworzenie i kontrolowanie animacji sk�adaj�cej si�
 * z sekwencji klatek. Obs�uguje odtwarzanie, pauzowanie, zatrzymywanie
 * oraz zap�tlanie animacji.
 */
class Animation {
private:
    std::string name;                    ///< Nazwa animacji
    std::vector<AnimationFrame> frames;  ///< Wektor klatek animacji
    float totalDuration;                 ///< Ca�kowity czas trwania animacji
    bool isLooping;                      ///< Flaga okre�laj�ca czy animacja jest zap�tlona

    int currentFrameIndex;               ///< Indeks aktualnie wy�wietlanej klatki
    float currentFrameTime;              ///< Czas sp�dzony na aktualnej klatce
    bool isPlaying;                      ///< Flaga okre�laj�ca czy animacja jest odtwarzana

public:
    /**
     * @brief Konstruktor klasy Animation
     * @param name Nazwa animacji
     * @param looping Czy animacja ma by� zap�tlona (domy�lnie true)
     */
    Animation(const std::string& name, bool looping = true);

    /**
     * @brief Dodaje now� klatk� do animacji
     * @param x Pozycja X klatki w sprite sheet
     * @param y Pozycja Y klatki w sprite sheet
     * @param width Szeroko�� klatki
     * @param height Wysoko�� klatki
     * @param duration Czas trwania klatki w sekundach
     */
    void addFrame(int x, int y, int width, int height, float duration);

    /**
     * @brief Pobiera aktualnie wy�wietlan� klatk�
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
     * @brief Zatrzymuje animacj� i resetuje do pierwszej klatki
     */
    void stop();

    /**
     * @brief Resetuje animacj� do pierwszej klatki bez zatrzymywania
     */
    void reset();

    /**
     * @brief Aktualizuje stan animacji
     * @param deltaTime Czas (w sekundach) od ostatniej aktualizacji
     */
    void update(float deltaTime);

    /**
     * @brief Sprawdza czy animacja zosta�a zako�czona
     * @return true je�li animacja si� zako�czy�a, false w przeciwnym razie
     * @note Dla zap�tlonych animacji zawsze zwraca false
     */
    bool isFinished() const;

    /**
     * @brief Pobiera nazw� animacji
     * @return Nazwa animacji
     */
    std::string getName() const;

    /**
     * @brief Sprawdza czy animacja jest zap�tlona
     * @return true je�li animacja jest zap�tlona, false w przeciwnym razie
     */
    bool getIsLooping() const;

    /**
     * @brief Sprawdza czy animacja jest obecnie odtwarzana
     * @return true je�li animacja jest odtwarzana, false w przeciwnym razie
     */
    bool getIsPlaying() const;

    /**
     * @brief Ustawia tryb zap�tlenia animacji
     * @param loop true aby w��czy� zap�tlenie, false aby wy��czy�
     */
    void setLooping(bool loop);
};

#endif // ANIMATION_H


