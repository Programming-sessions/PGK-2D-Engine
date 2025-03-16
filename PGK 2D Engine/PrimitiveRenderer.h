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

public:
    PrimitiveRenderer(ALLEGRO_DISPLAY* display);

    // Ustawienie koloru rysowania
    void setColor(ALLEGRO_COLOR color);

    // Metody rysuj¹ce
    void drawPoint(const Point2D& point);
    void drawLine(const LineSegment& line);
    void drawLine(const Point2D& start, const Point2D& end);
};

#endif // PRIMITIVE_RENDERER_H