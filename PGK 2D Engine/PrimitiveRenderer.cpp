// W PrimitiveRenderer.cpp:
#include "PrimitiveRenderer.h"
#include <cmath>
#include <algorithm>

PrimitiveRenderer::PrimitiveRenderer(ALLEGRO_DISPLAY* display)
    : display(display), currentColor(al_map_rgb(255, 255, 255)) {
}

void PrimitiveRenderer::setColor(ALLEGRO_COLOR color) {
    currentColor = color;
}

void PrimitiveRenderer::putPixel(int x, int y, ALLEGRO_COLOR color) {
    ALLEGRO_BITMAP* target = al_get_target_bitmap();
    if (x >= 0 && x < al_get_bitmap_width(target) &&
        y >= 0 && y < al_get_bitmap_height(target)) {
        al_draw_pixel(x, y, color);
    }
}

void PrimitiveRenderer::drawPoint(const Point2D& point) {
    putPixel(static_cast<int>(point.getX()),
        static_cast<int>(point.getY()),
        currentColor);
}

void PrimitiveRenderer::bresenhamLine(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        putPixel(x1, y1, color);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void PrimitiveRenderer::drawLine(const LineSegment& line) {
    drawLine(line.getStart(), line.getEnd());
}

void PrimitiveRenderer::drawLine(const Point2D& start, const Point2D& end) {
    bresenhamLine(
        static_cast<int>(start.getX()),
        static_cast<int>(start.getY()),
        static_cast<int>(end.getX()),
        static_cast<int>(end.getY()),
        currentColor
    );
}

void PrimitiveRenderer::scanlineFill(int y, int x1, int x2) {
    // Upewniamy siê, ¿e x1 < x2
    if (x1 > x2) std::swap(x1, x2);

    // Rysujemy liniê poziom¹ punkt po punkcie
    for (int x = x1; x <= x2; x++) {
        putPixel(x, y, currentColor);
    }
}

void PrimitiveRenderer::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    // Sortujemy punkty wed³ug y
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
    if (y2 > y3) { std::swap(x2, x3); std::swap(y2, y3); }
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }

    if (y2 == y3) { // Trójk¹t z p³ask¹ podstaw¹ na dole
        float slope1 = (float)(x2 - x1) / (y2 - y1);
        float slope2 = (float)(x3 - x1) / (y3 - y1);

        float curX1 = x1;
        float curX2 = x1;

        for (int y = y1; y <= y2; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope1;
            curX2 += slope2;
        }
    }
    else if (y1 == y2) { // Trójk¹t z p³ask¹ podstaw¹ na górze
        float slope1 = (float)(x3 - x1) / (y3 - y1);
        float slope2 = (float)(x3 - x2) / (y3 - y2);

        float curX1 = x1;
        float curX2 = x2;

        for (int y = y1; y <= y3; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope1;
            curX2 += slope2;
        }
    }
    else { // Standardowy przypadek
        float slope1 = (float)(x2 - x1) / (y2 - y1);
        float slope2 = (float)(x3 - x1) / (y3 - y1);
        float slope3 = (float)(x3 - x2) / (y3 - y2);

        float curX1 = x1;
        float curX2 = x1;

        // Pierwsza czêœæ trójk¹ta
        for (int y = y1; y <= y2; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope1;
            curX2 += slope2;
        }

        // Druga czêœæ trójk¹ta
        curX1 = x2;
        for (int y = y2; y <= y3; y++) {
            scanlineFill(y, (int)curX1, (int)curX2);
            curX1 += slope3;
            curX2 += slope2;
        }
    }
}

void PrimitiveRenderer::drawTriangle(const Triangle& triangle) {
    drawTriangle(triangle.getP1(), triangle.getP2(), triangle.getP3(), triangle.isFilled());
}

void PrimitiveRenderer::drawTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled) {
    int x1 = static_cast<int>(p1.getX());
    int y1 = static_cast<int>(p1.getY());
    int x2 = static_cast<int>(p2.getX());
    int y2 = static_cast<int>(p2.getY());
    int x3 = static_cast<int>(p3.getX());
    int y3 = static_cast<int>(p3.getY());

    if (filled) {
        fillTriangle(x1, y1, x2, y2, x3, y3);
    }

    // Zawsze rysujemy obrys
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p1);
}

void PrimitiveRenderer::drawRectangle(const Rectangle& rectangle) {
    drawRectangle(rectangle.getTopLeft(),
        rectangle.getWidth(),
        rectangle.getHeight(),
        rectangle.isFilled());
}

void PrimitiveRenderer::drawRectangle(const Point2D& topLeft, float width, float height, bool filled) {
    int x = static_cast<int>(topLeft.getX());
    int y = static_cast<int>(topLeft.getY());
    int w = static_cast<int>(width);
    int h = static_cast<int>(height);

    if (filled) {
        // Wype³niamy prostok¹t linia po linii
        for (int currY = y; currY <= y + h; currY++) {
            scanlineFill(currY, x, x + w);
        }
    }

    // Rysujemy obrys prostok¹ta
    Point2D topRight(x + w, y);
    Point2D bottomLeft(x, y + h);
    Point2D bottomRight(x + w, y + h);

    drawLine(topLeft, topRight);
    drawLine(topRight, bottomRight);
    drawLine(bottomRight, bottomLeft);
    drawLine(bottomLeft, topLeft);
}