/**
 * @file PrimitiveRenderer.h
 * @brief Klasa odpowiedzialna za renderowanie podstawowych figur geometrycznych 2D
 *
 * PrimitiveRenderer implementuje wzorzec projektowy Singleton i zapewnia
 * metody do rysowania podstawowych figur geometrycznych przy u�yciu
 * biblioteki Allegro 5.
 */
#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <allegro5/allegro.h>
#include "Primitives.h"

 /**
  * @brief Klasa renderuj�ca prymitywy geometryczne
  *
  * Klasa implementuje wzorzec Singleton i dostarcza metody do
  * rysowania punkt�w, linii, tr�jk�t�w, prostok�t�w i okr�g�w.
  * Wykorzystuje bibliotek� Allegro 5 do renderowania grafiki.
  */
class PrimitiveRenderer {
private:
    static PrimitiveRenderer* instance;    ///< Instancja Singletona
    ALLEGRO_COLOR currentColor;            ///< Aktualny kolor rysowania
    ALLEGRO_DISPLAY* display;              ///< Wska�nik na wy�wietlacz Allegro

    /**
    * @brief Prywatny konstruktor (wzorzec Singleton)
    * @param display Wska�nik na wy�wietlacz Allegro
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
      * @param x Wsp�rz�dna X piksela
      * @param y Wsp�rz�dna Y piksela
      * @param color Kolor piksela
      */
    void putPixel(int x, int y, ALLEGRO_COLOR color);

    /**
     * @brief Implementacja algorytmu Bresenhama do rysowania linii
     * @param x1 Wsp�rz�dna X pocz�tku linii
     * @param y1 Wsp�rz�dna Y pocz�tku linii
     * @param x2 Wsp�rz�dna X ko�ca linii
     * @param y2 Wsp�rz�dna Y ko�ca linii
     * @param color Kolor linii
     */
    void bresenhamLine(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color);

    /**
     * @brief Wype�nia lini� skanowania w algorytmie wype�niania
     * @param y Wsp�rz�dna Y linii skanowania
     * @param x1 Pocz�tek odcinka do wype�nienia
     * @param x2 Koniec odcinka do wype�nienia
     */
    void scanlineFill(int y, int x1, int x2);

    /**
     * @brief Wype�nia tr�jk�t kolorem
     * @param x1,y1 Wsp�rz�dne pierwszego wierzcho�ka
     * @param x2,y2 Wsp�rz�dne drugiego wierzcho�ka
     * @param x3,y3 Wsp�rz�dne trzeciego wierzcho�ka
     */
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

    /**
     * @brief Rysuje punkty okr�gu w algorytmie Bresenhama
     * @param centerX,centerY Wsp�rz�dne �rodka okr�gu
     * @param x,y Wsp�rz�dne punktu na okr�gu
     */
    void plotCirclePoints(int centerX, int centerY, int x, int y);

    /**
     * @brief Wype�nia okr�g kolorem
     * @param centerX,centerY Wsp�rz�dne �rodka okr�gu
     * @param radius Promie� okr�gu
     */
    void fillCircle(int centerX, int centerY, int radius);

public:
    /**
     * @brief Zwraca instancj� renderera
     * @return Wska�nik na jedyn� instancj� klasy
     */
    static PrimitiveRenderer* getInstance();

    /**
     * @brief Inicjalizuje renderer
     * @param display Wska�nik na wy�wietlacz Allegro
     */
    static void initialize(ALLEGRO_DISPLAY* display);

    /**
     * @brief Zwalnia instancj� renderera
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
     * @brief Rysuje lini�
     * @param line Obiekt linii do narysowania
     */
    void drawLine(const LineSegment& line);

    /**
     * @brief Rysuje lini� mi�dzy dwoma punktami
     * @param start Punkt pocz�tkowy
     * @param end Punkt ko�cowy
     */
    void drawLine(const Point2D& start, const Point2D& end);

    /**
     * @brief Rysuje tr�jk�t
     * @param triangle Obiekt tr�jk�ta do narysowania
     */
    void drawTriangle(const Triangle& triangle);

    /**
     * @brief Rysuje tr�jk�t zdefiniowany trzema punktami
     * @param p1 Pierwszy wierzcho�ek
     * @param p2 Drugi wierzcho�ek
     * @param p3 Trzeci wierzcho�ek
     * @param filled Czy tr�jk�t ma by� wype�niony
     */
    void drawTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled);

    /**
     * @brief Rysuje prostok�t
     * @param rectangle Obiekt prostok�ta do narysowania
     */
    void drawRectangle(const Rectangle& rectangle);

    /**
     * @brief Rysuje prostok�t zdefiniowany punktem i wymiarami
     * @param topLeft Lewy g�rny r�g prostok�ta
     * @param width Szeroko�� prostok�ta
     * @param height Wysoko�� prostok�ta
     * @param filled Czy prostok�t ma by� wype�niony
     */
    void drawRectangle(const Point2D& topLeft, float width, float height, bool filled);

    /**
     * @brief Rysuje okr�g
     * @param circle Obiekt okr�gu do narysowania
     */
    void drawCircle(const Circle& circle);

    /**
     * @brief Rysuje okr�g zdefiniowany �rodkiem i promieniem
     * @param center �rodek okr�gu
     * @param radius Promie� okr�gu
     * @param filled Czy okr�g ma by� wype�niony
     */
    void drawCircle(const Point2D& center, float radius, bool filled);
};

#endif
