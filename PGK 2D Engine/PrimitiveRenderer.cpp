// W PrimitiveRenderer.cpp:
#include "PrimitiveRenderer.h"
#include <cmath>

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