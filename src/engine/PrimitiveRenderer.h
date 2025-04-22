/**
 * @file PrimitiveRenderer.h
 * @brief Klasa odpowiedzialna za renderowanie podstawowych figur geometrycznych 2D
 *
 * PrimitiveRenderer implementuje wzorzec projektowy Singleton i zapewnia
 * metody do rysowania podstawowych figur geometrycznych przy u¿yciu
 * biblioteki Allegro 5.
 */
#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <allegro5/allegro.h>
#include "Primitives.h"

 /**
  * @brief Klasa renderuj¹ca prymitywy geometryczne
  *
  * Klasa implementuje wzorzec Singleton i dostarcza metody do
  * rysowania punktów, linii, trójk¹tów, prostok¹tów i okrêgów.
  * Wykorzystuje bibliotekê Allegro 5 do renderowania grafiki.
  */
class PrimitiveRenderer {
private:
    static PrimitiveRenderer* instance;    ///< Instancja Singletona
    ALLEGRO_COLOR currentColor;            ///< Aktualny kolor rysowania
    ALLEGRO_DISPLAY* display;              ///< WskaŸnik na wyœwietlacz Allegro

    /**
    * @brief Prywatny konstruktor (wzorzec Singleton)
    * @param display WskaŸnik na wyœwietlacz Allegro
    */
    PrimitiveRenderer(ALLEGRO_DISPLAY* display);

    /**
    * @brief Prywatny destruktor (wzorzec Singleton)
    */
    ~PrimitiveRenderer();

    // Zablokowanie kopiowania
    PrimitiveRenderer(const PrimitiveRenderer&) = delete;
    PrimitiveRenderer& operator=(const PrimitiveRenderer&) = delete;

    /**
      * @brief Rysuje pojedynczy piksel
      * @param x Wspó³rzêdna X piksela
      * @param y Wspó³rzêdna Y piksela
      * @param color Kolor piksela
      */
    void putPixel(int x, int y, ALLEGRO_COLOR color);

    /**
     * @brief Implementacja algorytmu Bresenhama do rysowania linii
     * @param x1 Wspó³rzêdna X pocz¹tku linii
     * @param y1 Wspó³rzêdna Y pocz¹tku linii
     * @param x2 Wspó³rzêdna X koñca linii
     * @param y2 Wspó³rzêdna Y koñca linii
     * @param color Kolor linii
     */
    void bresenhamLine(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color);

    /**
     * @brief Wype³nia liniê skanowania w algorytmie wype³niania
     * @param y Wspó³rzêdna Y linii skanowania
     * @param x1 Pocz¹tek odcinka do wype³nienia
     * @param x2 Koniec odcinka do wype³nienia
     */
    void scanlineFill(int y, int x1, int x2);

    /**
     * @brief Wype³nia trójk¹t kolorem
     * @param x1,y1 Wspó³rzêdne pierwszego wierzcho³ka
     * @param x2,y2 Wspó³rzêdne drugiego wierzcho³ka
     * @param x3,y3 Wspó³rzêdne trzeciego wierzcho³ka
     */
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

    /**
     * @brief Rysuje punkty okrêgu w algorytmie Bresenhama
     * @param centerX,centerY Wspó³rzêdne œrodka okrêgu
     * @param x,y Wspó³rzêdne punktu na okrêgu
     */
    void plotCirclePoints(int centerX, int centerY, int x, int y);

    /**
     * @brief Wype³nia okr¹g kolorem
     * @param centerX,centerY Wspó³rzêdne œrodka okrêgu
     * @param radius Promieñ okrêgu
     */
    void fillCircle(int centerX, int centerY, int radius);

public:
    /**
     * @brief Zwraca instancjê renderera
     * @return WskaŸnik na jedyn¹ instancjê klasy
     */
    static PrimitiveRenderer* getInstance();

    /**
     * @brief Inicjalizuje renderer
     * @param display WskaŸnik na wyœwietlacz Allegro
     */
    static void initialize(ALLEGRO_DISPLAY* display);

    /**
     * @brief Zwalnia instancjê renderera
     */
    static void releaseInstance();

    /**
     * @brief Ustawia aktualny kolor rysowania
     * @param color Kolor w formacie Allegro
     */
    void setColor(ALLEGRO_COLOR color);

    /**
     * @brief Rysuje punkt
     * @param point Punkt do narysowania
     */
    void drawPoint(const Point2D& point);

    /**
     * @brief Rysuje liniê
     * @param line Obiekt linii do narysowania
     */
    void drawLine(const LineSegment& line);

    /**
     * @brief Rysuje liniê miêdzy dwoma punktami
     * @param start Punkt pocz¹tkowy
     * @param end Punkt koñcowy
     */
    void drawLine(const Point2D& start, const Point2D& end);

    /**
     * @brief Rysuje trójk¹t
     * @param triangle Obiekt trójk¹ta do narysowania
     */
    void drawTriangle(const Triangle& triangle);

    /**
     * @brief Rysuje trójk¹t zdefiniowany trzema punktami
     * @param p1 Pierwszy wierzcho³ek
     * @param p2 Drugi wierzcho³ek
     * @param p3 Trzeci wierzcho³ek
     * @param filled Czy trójk¹t ma byæ wype³niony
     */
    void drawTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled);

    /**
     * @brief Rysuje prostok¹t
     * @param rectangle Obiekt prostok¹ta do narysowania
     */
    void drawRectangle(const Rectangle& rectangle);

    /**
     * @brief Rysuje prostok¹t zdefiniowany punktem i wymiarami
     * @param topLeft Lewy górny róg prostok¹ta
     * @param width Szerokoœæ prostok¹ta
     * @param height Wysokoœæ prostok¹ta
     * @param filled Czy prostok¹t ma byæ wype³niony
     */
    void drawRectangle(const Point2D& topLeft, float width, float height, bool filled);

    /**
     * @brief Rysuje okr¹g
     * @param circle Obiekt okrêgu do narysowania
     */
    void drawCircle(const Circle& circle);

    /**
     * @brief Rysuje okr¹g zdefiniowany œrodkiem i promieniem
     * @param center Œrodek okrêgu
     * @param radius Promieñ okrêgu
     * @param filled Czy okr¹g ma byæ wype³niony
     */
    void drawCircle(const Point2D& center, float radius, bool filled);
};

#endif
