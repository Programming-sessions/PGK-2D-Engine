/**
 * @file Sprite.h
 * @brief Klasa reprezentuj�ca obiekt graficzny 2D z obs�ug� animacji
 *
 * Sprite zarz�dza tekstur�, pozycj�, rotacj�, skalowaniem oraz animacjami.
 * Umo�liwia wy�wietlanie zar�wno statycznych jak i animowanych obiekt�w 2D
 * z wykorzystaniem biblioteki Allegro 5.
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Primitives.h"
#include "Animation.h"

 /**
  * @brief Klasa reprezentuj�ca sprite (obiekt graficzny 2D)
  *
  * Umo�liwia:
  * - �adowanie i wy�wietlanie tekstur
  * - Transformacje (pozycja, rotacja, skala)
  * - Zarz�dzanie przezroczysto�ci�
  * - Obs�ug� sprite sheet'�w
  * - Zarz�dzanie animacjami
  */
class Sprite {
private:
    ALLEGRO_BITMAP* texture;   ///< Wska�nik na tekstur�
    Point2D position;          ///< Pozycja sprite'a
    Point2D scale;             ///< Skala w osiach X i Y
    float rotation;            ///< K�t obrotu w radianach
    float alpha;               ///< Przezroczysto�� (0.0 - 1.0)

    std::map<std::string, Animation*> animations;  ///< Mapa dost�pnych animacji
    Animation* currentAnimation;                   ///< Wska�nik na aktualn� animacj�

    // Parametry obszaru �r�d�owego dla sprite sheet
    int sourceX;        ///< Pozycja X w teksturze �r�d�owej
    int sourceY;        ///< Pozycja Y w teksturze �r�d�owej
    int sourceWidth;    ///< Szeroko�� obszaru �r�d�owego
    int sourceHeight;   ///< Wysoko�� obszaru �r�d�owego

public:
    /**
     * @brief Konstruktor domy�lny
     *
     * Inicjalizuje sprite z domy�lnymi warto�ciami:
     * - Pozycja (0,0)
     * - Skala (1,1)
     * - Rotacja 0
     * - Alpha 1.0
     * - Brak tekstury i animacji
     */
    Sprite();

    /**
     * @brief Destruktor
     *
     * Zwalnia zasoby animacji, tekstura jest zarz�dzana przez TextureManager
     */
    ~Sprite();

    /**
     * @brief �aduje tekstur� z pliku
     * @param path �cie�ka do pliku tekstury
     * @return true je�li za�adowano pomy�lnie
     */
    bool loadTexture(const std::string& path);

    /**
     * @brief Ustawia pozycj� sprite'a
     * @param x Wsp�rz�dna X
     * @param y Wsp�rz�dna Y
     */
    void setPosition(float x, float y);

    /**
     * @brief Ustawia k�t obrotu
     * @param angle K�t w radianach
     */
    void setRotation(float angle);

    /**
     * @brief Ustawia skal� sprite'a
     * @param scaleX Skala w osi X
     * @param scaleY Skala w osi Y
     */
    void setScale(float scaleX, float scaleY);

    /**
     * @brief Ustawia przezroczysto��
     * @param alpha Warto�� alpha (0.0 - ca�kowicie przezroczysty, 1.0 - nieprzezroczysty)
     */
    void setAlpha(float alpha);

    /**
     * @brief Ustawia obszar �r�d�owy w sprite sheet
     * @param x Pozycja X w teksturze
     * @param y Pozycja Y w teksturze
     * @param width Szeroko�� obszaru
     * @param height Wysoko�� obszaru
     */
    void setSourceRect(int x, int y, int width, int height);

    /**
     * @brief Resetuje obszar �r�d�owy do pe�nego rozmiaru tekstury
     */
    void resetSourceRect();

    /**
     * @brief Dodaje now� animacj�
     * @param animation Wska�nik na obiekt animacji
     *
     * Je�li animacja o danej nazwie ju� istnieje, zostanie zast�piona.
     * Przejmuje w�asno�� wska�nika animation.
     */
    void addAnimation(Animation* animation);

    /**
     * @brief Rozpoczyna odtwarzanie animacji
     * @param name Nazwa animacji
     */
    void playAnimation(const std::string& name);

    /**
     * @brief Zatrzymuje aktualn� animacj� i cofa do pierwszej klatki
     */
    void stopAnimation();

    /**
     * @brief Wstrzymuje aktualn� animacj�
     */
    void pauseAnimation();

    /**
     * @brief Wznawia wstrzyman� animacj�
     */
    void resumeAnimation();

    /**
     * @brief Aktualizuje stan animacji
     * @param deltaTime Czas od ostatniej aktualizacji w sekundach
     */
    void updateAnimation(float deltaTime);

    /**
     * @brief Pobiera wska�nik na aktualn� animacj�
     * @return Wska�nik na aktualn� animacj� lub nullptr je�li brak
     */
    Animation* getCurrentAnimation() const;

    /**
     * @brief Pobiera aktualn� pozycj�
     * @return Obiekt Point2D z pozycj�
     */
    Point2D getPosition() const;

    /**
     * @brief Pobiera aktualn� skal�
     * @return Obiekt Point2D ze skal� X i Y
     */
    Point2D getScale() const;

    /**
     * @brief Pobiera aktualny k�t obrotu
     * @return K�t w radianach
     */
    float getRotation() const;

    /**
     * @brief Pobiera aktualn� przezroczysto��
     * @return Warto�� alpha (0.0 - 1.0)
     */
    float getAlpha() const;

    /**
     * @brief Pobiera szeroko�� sprite'a
     * @return Szeroko�� w pikselach
     */
    int getWidth() const;

    /**
     * @brief Pobiera wysoko�� sprite'a
     * @return Wysoko�� w pikselach
     */
    int getHeight() const;

    /**
     * @brief Rysuje sprite na ekranie
     *
     * Uwzgl�dnia wszystkie transformacje (pozycja, rotacja, skala),
     * przezroczysto�� oraz aktualn� klatk� animacji je�li jest aktywna.
     */
    void draw();
};

#endif // SPRITE_H
