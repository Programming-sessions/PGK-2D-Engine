#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>
#include "Primitives.h"
#define PI 3.14159265358979323846

class Transform {
public:
    static void translate(Point2D& point, float dx, float dy);
    static void scale(Point2D& point, float sx, float sy, const Point2D& center = Point2D(0, 0));
    static void rotate(Point2D& point, float angle, const Point2D& center = Point2D(0, 0));
};

#endif
