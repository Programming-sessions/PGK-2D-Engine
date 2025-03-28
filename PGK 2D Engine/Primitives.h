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

class Triangle {
private:
    Point2D p1;
    Point2D p2;
    Point2D p3;
    bool filled;

public:
    // Constructors
    Triangle();
    Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled = false);
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);

    // Getters
    Point2D getP1() const;
    Point2D getP2() const;
    Point2D getP3() const;
    bool isFilled() const;

    // Setters
    void setP1(const Point2D& point);
    void setP2(const Point2D& point);
    void setP3(const Point2D& point);
    void setPoints(const Point2D& p1, const Point2D& p2, const Point2D& p3);
    void setPoints(float x1, float y1, float x2, float y2, float x3, float y3);
    void setFilled(bool fill);
};

class Rectangle {
private:
    Point2D topLeft;
    float width;
    float height;
    bool filled;

public:
    // Constructors
    Rectangle();
    Rectangle(const Point2D& topLeft, float width, float height, bool filled = false);
    Rectangle(float x, float y, float width, float height, bool filled = false);

    // Getters
    Point2D getTopLeft() const;
    Point2D getTopRight() const;
    Point2D getBottomLeft() const;
    Point2D getBottomRight() const;
    float getWidth() const;
    float getHeight() const;
    bool isFilled() const;

    // Setters
    void setTopLeft(const Point2D& point);
    void setTopLeft(float x, float y);
    void setSize(float width, float height);
    void setFilled(bool fill);

    // Additional methods
    bool contains(const Point2D& point) const;
    bool intersects(const Rectangle& other) const;
};

class Circle {
private:
    Point2D center;
    float radius;
    bool filled;

public:
    // Konstruktory
    Circle();
    Circle(const Point2D& center, float radius, bool filled = false);
    Circle(float x, float y, float radius, bool filled = false);

    // Gettery
    Point2D getCenter() const;
    float getRadius() const;
    bool isFilled() const;

    // Settery
    void setCenter(const Point2D& point);
    void setCenter(float x, float y);
    void setRadius(float newRadius);
    void setFilled(bool fill);

    // Dodatkowe metody
    bool contains(const Point2D& point) const;
};


#endif // PRIMITIVES_H
