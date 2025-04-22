/**
 * @file Sprite.h
 * @brief Klasa reprezentuj¹ca obiekt graficzny 2D z obs³ug¹ animacji
 *
 * Sprite zarz¹dza tekstur¹, pozycj¹, rotacj¹, skalowaniem oraz animacjami.
 * Umo¿liwia wyœwietlanie zarówno statycznych jak i animowanych obiektów 2D
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
  * @brief Klasa reprezentuj¹ca sprite (obiekt graficzny 2D)
  *
  * Umo¿liwia:
  * - £adowanie i wyœwietlanie tekstur
  * - Transformacje (pozycja, rotacja, skala)
  * - Zarz¹dzanie przezroczystoœci¹
  * - Obs³ugê sprite sheet'ów
  * - Zarz¹dzanie animacjami
  */
class Sprite {
private:
    ALLEGRO_BITMAP* texture;   ///< WskaŸnik na teksturê
    Point2D position;          ///< Pozycja sprite'a
    Point2D scale;             ///< Skala w osiach X i Y
    float rotation;            ///< K¹t obrotu w radianach
    float alpha;               ///< Przezroczystoœæ (0.0 - 1.0)

    std::map<std::string, Animation*> animations;  ///< Mapa dostêpnych animacji
    Animation* currentAnimation;                   ///< WskaŸnik na aktualn¹ animacjê

    // Parametry obszaru Ÿród³owego dla sprite sheet
    int sourceX;        ///< Pozycja X w teksturze Ÿród³owej
    int sourceY;        ///< Pozycja Y w teksturze Ÿród³owej
    int sourceWidth;    ///< Szerokoœæ obszaru Ÿród³owego
    int sourceHeight;   ///< Wysokoœæ obszaru Ÿród³owego

public:
    /**
     * @brief Konstruktor domyœlny
     *
     * Inicjalizuje sprite z domyœlnymi wartoœciami:
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
     * Zwalnia zasoby animacji, tekstura jest zarz¹dzana przez TextureManager
     */
    ~Sprite();

    /**
     * @brief £aduje teksturê z pliku
     * @param path Œcie¿ka do pliku tekstury
     * @return true jeœli za³adowano pomyœlnie
     */
    bool loadTexture(const std::string& path);

    /**
     * @brief Ustawia pozycjê sprite'a
     * @param x Wspó³rzêdna X
     * @param y Wspó³rzêdna Y
     */
    void setPosition(float x, float y);

    /**
     * @brief Ustawia k¹t obrotu
     * @param angle K¹t w radianach
     */
    void setRotation(float angle);

    /**
     * @brief Ustawia skalê sprite'a
     * @param scaleX Skala w osi X
     * @param scaleY Skala w osi Y
     */
    void setScale(float scaleX, float scaleY);

    /**
     * @brief Ustawia przezroczystoœæ
     * @param alpha Wartoœæ alpha (0.0 - ca³kowicie przezroczysty, 1.0 - nieprzezroczysty)
     */
    void setAlpha(float alpha);

    /**
     * @brief Ustawia obszar Ÿród³owy w sprite sheet
     * @param x Pozycja X w teksturze
     * @param y Pozycja Y w teksturze
     * @param width Szerokoœæ obszaru
     * @param height Wysokoœæ obszaru
     */
    void setSourceRect(int x, int y, int width, int height);

    /**
     * @brief Resetuje obszar Ÿród³owy do pe³nego rozmiaru tekstury
     */
    void resetSourceRect();

    /**
     * @brief Dodaje now¹ animacjê
     * @param animation WskaŸnik na obiekt animacji
     *
     * Jeœli animacja o danej nazwie ju¿ istnieje, zostanie zast¹piona.
     * Przejmuje w³asnoœæ wskaŸnika animation.
     */
    void addAnimation(Animation* animation);

    /**
     * @brief Rozpoczyna odtwarzanie animacji
     * @param name Nazwa animacji
     */
    void playAnimation(const std::string& name);

    /**
     * @brief Zatrzymuje aktualn¹ animacjê i cofa do pierwszej klatki
     */
    void stopAnimation();

    /**
     * @brief Wstrzymuje aktualn¹ animacjê
     */
    void pauseAnimation();

    /**
     * @brief Wznawia wstrzyman¹ animacjê
     */
    void resumeAnimation();

    /**
     * @brief Aktualizuje stan animacji
     * @param deltaTime Czas od ostatniej aktualizacji w sekundach
     */
    void updateAnimation(float deltaTime);

    /**
     * @brief Pobiera wskaŸnik na aktualn¹ animacjê
     * @return WskaŸnik na aktualn¹ animacjê lub nullptr jeœli brak
     */
    Animation* getCurrentAnimation() const;

    /**
     * @brief Pobiera aktualn¹ pozycjê
     * @return Obiekt Point2D z pozycj¹
     */
    Point2D getPosition() const;

    /**
     * @brief Pobiera aktualn¹ skalê
     * @return Obiekt Point2D ze skal¹ X i Y
     */
    Point2D getScale() const;

    /**
     * @brief Pobiera aktualny k¹t obrotu
     * @return K¹t w radianach
     */
    float getRotation() const;

    /**
     * @brief Pobiera aktualn¹ przezroczystoœæ
     * @return Wartoœæ alpha (0.0 - 1.0)
     */
    float getAlpha() const;

    /**
     * @brief Pobiera szerokoœæ sprite'a
     * @return Szerokoœæ w pikselach
     */
    int getWidth() const;

    /**
     * @brief Pobiera wysokoœæ sprite'a
     * @return Wysokoœæ w pikselach
     */
    int getHeight() const;

    /**
     * @brief Rysuje sprite na ekranie
     *
     * Uwzglêdnia wszystkie transformacje (pozycja, rotacja, skala),
     * przezroczystoœæ oraz aktualn¹ klatkê animacji jeœli jest aktywna.
     */
    void draw();
};

#endif // SPRITE_H
