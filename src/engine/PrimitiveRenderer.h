// PrimitiveRenderer.h
#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <allegro5/allegro.h>
#include "Primitives.h"

class PrimitiveRenderer {
private:
    // Singleton instance
    static PrimitiveRenderer* instance;

    ALLEGRO_COLOR currentColor;
    ALLEGRO_DISPLAY* display;

    // Prywatny konstruktor i destruktor (singleton)
    PrimitiveRenderer(ALLEGRO_DISPLAY* display);
    ~PrimitiveRenderer();

    // Zablokowanie kopiowania
    PrimitiveRenderer(const PrimitiveRenderer&) = delete;
    PrimitiveRenderer& operator=(const PrimitiveRenderer&) = delete;

    // Metody pomocnicze
    void putPixel(int x, int y, ALLEGRO_COLOR color);
    void bresenhamLine(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color);
    void scanlineFill(int y, int x1, int x2);
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void plotCirclePoints(int centerX, int centerY, int x, int y);
    void fillCircle(int centerX, int centerY, int radius);

public:
    // Metody zarz¹dzania singletonem
    static PrimitiveRenderer* getInstance();
    static void initialize(ALLEGRO_DISPLAY* display);
    static void releaseInstance();

    // Metody publiczne
    void setColor(ALLEGRO_COLOR color);
    void drawPoint(const Point2D& point);
    void drawLine(const LineSegment& line);
    void drawLine(const Point2D& start, const Point2D& end);
    void drawTriangle(const Triangle& triangle);
    void drawTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled);
    void drawRectangle(const Rectangle& rectangle);
    void drawRectangle(const Point2D& topLeft, float width, float height, bool filled);
    void drawCircle(const Circle& circle);
    void drawCircle(const Point2D& center, float radius, bool filled);
};

#endif
