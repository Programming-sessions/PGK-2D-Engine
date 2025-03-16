#ifndef PRIMITIVES_H
#define PRIMITIVES_H

class Point2D {
private:
    float x;
    float y;

public:
    // Constructors
    Point2D();
    Point2D(float x, float y);

    // Getters
    float getX() const;
    float getY() const;

    // Setters
    void setX(float newX);
    void setY(float newY);
    void setPosition(float newX, float newY);

    // Other Functions
    void clamp(float minX, float minY, float maxX, float maxY);
};

class LineSegment {
private:
    Point2D start;
    Point2D end;

public:
    // Konstruktory
    LineSegment();
    LineSegment(const Point2D& start, const Point2D& end);
    LineSegment(float x1, float y1, float x2, float y2);

    // Gettery
    Point2D getStart() const;
    Point2D getEnd() const;

    // Settery
    void setStart(const Point2D& point);
    void setEnd(const Point2D& point);
    void setStart(float x, float y);
    void setEnd(float x, float y);
    void setLine(const Point2D& newStart, const Point2D& newEnd);
    void setLine(float x1, float y1, float x2, float y2);
};

#endif // PRIMITIVES_H
