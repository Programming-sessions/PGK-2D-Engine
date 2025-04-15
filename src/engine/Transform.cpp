#include "Transform.h"

void Transform::translate(Point2D& point, float dx, float dy) {
    point.setX(point.getX() + dx);
    point.setY(point.getY() + dy);
}

void Transform::scale(Point2D& point, float sx, float sy, const Point2D& center) {
    // Przesuniêcie do punktu (0,0)
    float x = point.getX() - center.getX();
    float y = point.getY() - center.getY();

    // Skalowanie
    x *= sx;
    y *= sy;

    // Przesuniêcie z powrotem
    point.setX(x + center.getX());
    point.setY(y + center.getY());
}

void Transform::rotate(Point2D& point, float angle, const Point2D& center) {
    // Konwersja k¹ta na radiany
    float radians = angle * PI / 180.0f;

    // Przesuniêcie do punktu (0,0)
    float x = point.getX() - center.getX();
    float y = point.getY() - center.getY();

    // Rotacja
    float newX = x * cos(radians) - y * sin(radians);
    float newY = x * sin(radians) + y * cos(radians);

    // Przesuniêcie z powrotem
    point.setX(newX + center.getX());
    point.setY(newY + center.getY());
}
