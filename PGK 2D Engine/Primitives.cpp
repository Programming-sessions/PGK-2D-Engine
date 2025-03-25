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

// Triangle Implementation
Triangle::Triangle() : p1(0.0f, 0.0f), p2(0.0f, 0.0f), p3(0.0f, 0.0f), filled(false) {}

Triangle::Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled)
    : p1(p1), p2(p2), p3(p3), filled(filled) {
}

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled)
    : p1(x1, y1), p2(x2, y2), p3(x3, y3), filled(filled) {
}

Point2D Triangle::getP1() const { return p1; }
Point2D Triangle::getP2() const { return p2; }
Point2D Triangle::getP3() const { return p3; }
bool Triangle::isFilled() const { return filled; }

void Triangle::setP1(const Point2D& point) { p1 = point; }
void Triangle::setP2(const Point2D& point) { p2 = point; }
void Triangle::setP3(const Point2D& point) { p3 = point; }

void Triangle::setPoints(const Point2D& newP1, const Point2D& newP2, const Point2D& newP3) {
    p1 = newP1;
    p2 = newP2;
    p3 = newP3;
}

void Triangle::setPoints(float x1, float y1, float x2, float y2, float x3, float y3) {
    p1.setPosition(x1, y1);
    p2.setPosition(x2, y2);
    p3.setPosition(x3, y3);
}

void Triangle::setFilled(bool fill) { filled = fill; }

// Rectangle Implementation
Rectangle::Rectangle()
    : topLeft(0.0f, 0.0f), width(0.0f), height(0.0f), filled(false) {
}

Rectangle::Rectangle(const Point2D& topLeft, float width, float height, bool filled)
    : topLeft(topLeft), width(width), height(height), filled(filled) {
}

Rectangle::Rectangle(float x, float y, float width, float height, bool filled)
    : topLeft(x, y), width(width), height(height), filled(filled) {
}

Point2D Rectangle::getTopLeft() const { return topLeft; }

Point2D Rectangle::getTopRight() const {
    return Point2D(topLeft.getX() + width, topLeft.getY());
}

Point2D Rectangle::getBottomLeft() const {
    return Point2D(topLeft.getX(), topLeft.getY() + height);
}

Point2D Rectangle::getBottomRight() const {
    return Point2D(topLeft.getX() + width, topLeft.getY() + height);
}

float Rectangle::getWidth() const { return width; }
float Rectangle::getHeight() const { return height; }
bool Rectangle::isFilled() const { return filled; }

void Rectangle::setTopLeft(const Point2D& point) { topLeft = point; }

void Rectangle::setTopLeft(float x, float y) {
    topLeft.setPosition(x, y);
}

void Rectangle::setSize(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}

void Rectangle::setFilled(bool fill) { filled = fill; }

bool Rectangle::contains(const Point2D& point) const {
    float px = point.getX();
    float py = point.getY();
    float left = topLeft.getX();
    float top = topLeft.getY();

    return (px >= left && px <= left + width &&
        py >= top && py <= top + height);
}

bool Rectangle::intersects(const Rectangle& other) const {
    float left1 = topLeft.getX();
    float top1 = topLeft.getY();
    float right1 = left1 + width;
    float bottom1 = top1 + height;

    float left2 = other.getTopLeft().getX();
    float top2 = other.getTopLeft().getY();
    float right2 = left2 + other.getWidth();
    float bottom2 = top2 + other.getHeight();

    return !(right1 < left2 || left1 > right2 ||
        bottom1 < top2 || top1 > bottom2);
}