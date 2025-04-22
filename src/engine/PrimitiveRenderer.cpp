#include "PrimitiveRenderer.h"
#include <cmath>
#include <algorithm>
#include <iostream>

PrimitiveRenderer* PrimitiveRenderer::instance = nullptr;

void PrimitiveRenderer::initialize(ALLEGRO_DISPLAY* display) {
    if (!instance) {
        instance = new PrimitiveRenderer(display);
    }
}

PrimitiveRenderer* PrimitiveRenderer::getInstance() {
    if (!instance) {
        // Logowanie b³êdu lub rzucenie wyj¹tku
        std::cerr << "Failed to create renderer!" << std::endl;
    }
    return instance;
}

void PrimitiveRenderer::releaseInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

PrimitiveRenderer::PrimitiveRenderer(ALLEGRO_DISPLAY* display)
    : display(display)
    , currentColor(al_map_rgb(255, 255, 255))
{
}

PrimitiveRenderer::~PrimitiveRenderer() {
    // Cleanup jeœli potrzebny
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

    // Normalizacja wymiarów - obs³uga ujemnych wartoœci
    if (w < 0) {
        x += w;
        w = -w;
    }
    if (h < 0) {
        y += h;
        h = -h;
    }

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
    Point2D actualTopLeft(x, y);

    drawLine(actualTopLeft, topRight);
    drawLine(topRight, bottomRight);
    drawLine(bottomRight, bottomLeft);
    drawLine(bottomLeft, actualTopLeft);
}

void PrimitiveRenderer::plotCirclePoints(int centerX, int centerY, int x, int y) {
    putPixel(centerX + x, centerY + y, currentColor);
    putPixel(centerX - x, centerY + y, currentColor);
    putPixel(centerX + x, centerY - y, currentColor);
    putPixel(centerX - x, centerY - y, currentColor);
    putPixel(centerX + y, centerY + x, currentColor);
    putPixel(centerX - y, centerY + x, currentColor);
    putPixel(centerX + y, centerY - x, currentColor);
    putPixel(centerX - y, centerY - x, currentColor);
}

void PrimitiveRenderer::fillCircle(int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        int x_val = static_cast<int>(sqrt(radius * radius - y * y));
        scanlineFill(centerY + y, centerX - x_val, centerX + x_val);
    }
}

void PrimitiveRenderer::drawCircle(const Circle& circle) {
    drawCircle(circle.getCenter(), circle.getRadius(), circle.isFilled());
}

void PrimitiveRenderer::drawCircle(const Point2D& center, float radius, bool filled) {
    int centerX = static_cast<int>(center.getX());
    int centerY = static_cast<int>(center.getY());
    int intRadius = static_cast<int>(radius);

    if (filled) {
        fillCircle(centerX, centerY, intRadius);
    }

    // Algorytm Bresenhama dla okrêgu
    int x = 0;
    int y = intRadius;
    int d = 3 - 2 * intRadius;

    plotCirclePoints(centerX, centerY, x, y);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else {
            d = d + 4 * x + 6;
        }
        plotCirclePoints(centerX, centerY, x, y);
    }
}

