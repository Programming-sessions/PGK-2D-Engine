#include "Primitives.h"
#include <algorithm>

// LineSegment Implementation
LineSegment::LineSegment() : start(0.0f, 0.0f), end(0.0f, 0.0f) {}
LineSegment::LineSegment(const glm::vec2& start, const glm::vec2& end) : start(start), end(end) {}
LineSegment::LineSegment(float x1, float y1, float x2, float y2) : start(x1, y1), end(x2, y2) {}

// Triangle Implementation
Triangle::Triangle() : p1(0.0f, 0.0f), p2(0.0f, 0.0f), p3(0.0f, 0.0f), filled(false) {}
Triangle::Triangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, bool filled)
    : p1(p1), p2(p2), p3(p3), filled(filled) {}
Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled)
    : p1(x1, y1), p2(x2, y2), p3(x3, y3), filled(filled) {}

// Rectangle Implementation
Rectangle::Rectangle() : position(0.0f, 0.0f), size(0.0f, 0.0f), rotation(0.0f), filled(false) {}
Rectangle::Rectangle(const glm::vec2& pos, const glm::vec2& size, bool filled, float rotation)
    : position(pos), size(size), filled(filled), rotation(rotation) {}
Rectangle::Rectangle(float x, float y, float width, float height, bool filled, float rotation)
    : position(x, y), size(width, height), filled(filled), rotation(rotation) {}

std::vector<glm::vec2> Rectangle::getCorners() const {
    std::vector<glm::vec2> corners;
    glm::vec2 halfSize = size / 2.0f;
    corners.push_back(glm::vec2(-halfSize.x, -halfSize.y));
    corners.push_back(glm::vec2(halfSize.x, -halfSize.y));
    corners.push_back(glm::vec2(halfSize.x, halfSize.y));
    corners.push_back(glm::vec2(-halfSize.x, halfSize.y));

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    for (auto& corner : corners) {
        corner = glm::vec2(transform * glm::vec4(corner, 0.0f, 1.0f));
    }
    return corners;
}

bool Rectangle::contains(const glm::vec2& point) const {
    glm::vec2 point_relative = point - position;

    float r = glm::radians(-rotation);
    float c = cos(r);
    float s = sin(r);

    float point_x_rotated = point_relative.x * c - point_relative.y * s;
    float point_y_rotated = point_relative.x * s + point_relative.y * c;

    glm::vec2 halfSize = size / 2.0f;

    return (point_x_rotated >= -halfSize.x && point_x_rotated <= halfSize.x &&
        point_y_rotated >= -halfSize.y && point_y_rotated <= halfSize.y);
}

bool Rectangle::intersects(const Rectangle& other) const {
    auto getAxes = [](const Rectangle& rect) {
        std::vector<glm::vec2> axes;
        std::vector<glm::vec2> corners = rect.getCorners();
        for (int i = 0; i < 4; i++) {
            glm::vec2 p1 = corners[i];
            glm::vec2 p2 = corners[(i + 1) % 4];
            glm::vec2 edge = p2 - p1;
            glm::vec2 normal = glm::normalize(glm::vec2(-edge.y, edge.x));
            axes.push_back(normal);
        }
        return axes;
    };

    auto project = [](const Rectangle& rect, const glm::vec2& axis) {
        float min = glm::dot(rect.getCorners()[0], axis);
        float max = min;
        for (int i = 1; i < 4; i++) {
            float p = glm::dot(rect.getCorners()[i], axis);
            if (p < min) {
                min = p;
            }
            else if (p > max) {
                max = p;
            }
        }
        return std::make_pair(min, max);
    };

    std::vector<glm::vec2> axes1 = getAxes(*this);
    std::vector<glm::vec2> axes2 = getAxes(other);

    for (const auto& axis : axes1) {
        auto p1 = project(*this, axis);
        auto p2 = project(other, axis);
        if (p1.second < p2.first || p2.second < p1.first) {
            return false;
        }
    }

    for (const auto& axis : axes2) {
        auto p1 = project(*this, axis);
        auto p2 = project(other, axis);
        if (p1.second < p2.first || p2.second < p1.first) {
            return false;
        }
    }

    return true;
}

// Circle Implementation
Circle::Circle() : center(0.0f, 0.0f), radius(0.0f), filled(false) {}
Circle::Circle(const glm::vec2& center, float radius, bool filled)
    : center(center), radius(radius), filled(filled) {}
Circle::Circle(float x, float y, float radius, bool filled)
    : center(x, y), radius(radius), filled(filled) {}

bool Circle::contains(const glm::vec2& point) const {
    return glm::distance(point, center) <= radius;
}