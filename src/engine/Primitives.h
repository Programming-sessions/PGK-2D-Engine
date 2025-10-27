#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <cmath>

/**
 * @brief Klasa reprezentująca odcinek w przestrzeni 2D
 */
class LineSegment {
public:
    glm::vec2 start;    ///< Punkt początkowy odcinka
    glm::vec2 end;      ///< Punkt końcowy odcinka

    LineSegment();
    LineSegment(const glm::vec2& start, const glm::vec2& end);
    LineSegment(float x1, float y1, float x2, float y2);
};

/**
 * @brief Klasa reprezentująca trójkąt w przestrzeni 2D
 */
class Triangle {
public:
    glm::vec2 p1;      ///< Pierwszy wierzchołek trójkąta
    glm::vec2 p2;      ///< Drugi wierzchołek trójkąta
    glm::vec2 p3;      ///< Trzeci wierzchołek trójkąta
    bool filled;     ///< Flaga wypełnienia trójkąta

    Triangle();
    Triangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, bool filled = false);
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);
};

/**
 * @brief Klasa reprezentująca prostokąt w przestrzeni 2D
 */
class Rectangle {
public:
    glm::vec2 position;
    glm::vec2 size;
    float rotation; // In degrees
    bool filled;

    Rectangle();
    Rectangle(const glm::vec2& pos, const glm::vec2& size, bool filled = false, float rotation = 0.0f);
    Rectangle(float x, float y, float width, float height, bool filled = false, float rotation = 0.0f);

    std::vector<glm::vec2> getCorners() const;
    bool contains(const glm::vec2& point) const;
    bool intersects(const Rectangle& other) const;
};


/**
 * @brief Klasa reprezentująca okrąg w przestrzeni 2D
 */
class Circle {
public:
    glm::vec2 center; ///< Środek okręgu
    float radius;   ///< Promień okręgu
    bool filled;    ///< Flaga wypełnienia okręgu

    Circle();
    Circle(const glm::vec2& center, float radius, bool filled = false);
    Circle(float x, float y, float radius, bool filled = false);

    bool contains(const glm::vec2& point) const;
};

#endif // PRIMITIVES_H