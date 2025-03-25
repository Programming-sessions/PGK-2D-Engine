// W PrimitiveRenderer.h:
#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <allegro5/allegro.h>
#include "Primitives.h"

class PrimitiveRenderer {
private:
    ALLEGRO_COLOR currentColor;
    ALLEGRO_DISPLAY* display;

    // Metoda pomocnicza do rysowania pojedynczego piksela
    void putPixel(int x, int y, ALLEGRO_COLOR color);

    // Implementacja algorytmu Bresenhama dla linii
    void bresenhamLine(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color);

    // Metody pomocnicze do wype³niania
    void scanlineFill(int y, int x1, int x2);
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

public:
    PrimitiveRenderer(ALLEGRO_DISPLAY* display);

    // Ustawienie koloru rysowania
    void setColor(ALLEGRO_COLOR color);

    // Metody rysuj¹ce
    void drawPoint(const Point2D& point);
    void drawLine(const LineSegment& line);
    void drawLine(const Point2D& start, const Point2D& end);

    // Nowe metody do rysowania trójk¹tów
    void drawTriangle(const Triangle& triangle);
    void drawTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled);

    // Nowe metody do rysowania prostok¹tów
    void drawRectangle(const Rectangle& rectangle);
    void drawRectangle(const Point2D& topLeft, float width, float height, bool filled);
};

#endif // PRIMITIVE_RENDERER_H