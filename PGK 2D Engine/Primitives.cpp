#include "Primitives.h"

// Constructors
Point2D::Point2D() : x(0.0f), y(0.0f) {}

Point2D::Point2D(float x, float y) : x(x), y(y) {}

// Getters
float Point2D::getX() const {
    return x;
}

float Point2D::getY() const {
    return y;
}

// Setters
void Point2D::setX(float newX) {
    x = newX;
}

void Point2D::setY(float newY) {
    y = newY;
}

void Point2D::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

// Other Functions
void Point2D::clamp(float minX, float minY, float maxX, float maxY) {
    if (x < minX) x = minX;
    if (y < minY) y = minY;
    if (x > maxX) x = maxX;
    if (y > maxY) y = maxY;
}

// Konstruktory
LineSegment::LineSegment() : start(0.0f, 0.0f), end(0.0f, 0.0f) {}

LineSegment::LineSegment(const Point2D& start, const Point2D& end)
    : start(start), end(end) {
}

LineSegment::LineSegment(float x1, float y1, float x2, float y2)
    : start(x1, y1), end(x2, y2) {
}

// Gettery
Point2D LineSegment::getStart() const {
    return start;
}

Point2D LineSegment::getEnd() const {
    return end;
}

// Settery
void LineSegment::setStart(const Point2D& point) {
    start = point;
}

void LineSegment::setEnd(const Point2D& point) {
    end = point;
}

void LineSegment::setStart(float x, float y) {
    start.setPosition(x, y);
}

void LineSegment::setEnd(float x, float y) {
    end.setPosition(x, y);
}

void LineSegment::setLine(const Point2D& newStart, const Point2D& newEnd) {
    start = newStart;
    end = newEnd;
}

void LineSegment::setLine(float x1, float y1, float x2, float y2) {
    start.setPosition(x1, y1);
    end.setPosition(x2, y2);
}