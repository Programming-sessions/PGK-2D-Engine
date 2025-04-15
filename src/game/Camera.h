#ifndef CAMERA_H
#define CAMERA_H

#include "../engine/Primitives.h"
#include "Entity.h"
#include <allegro5/allegro.h>

class Camera {
private:
    Point2D position;
    float zoom;
    Entity* target;
    int viewportWidth;
    int viewportHeight;

public:
    Camera();
    ~Camera();

    // Podstawowe funkcje
    void update();
    void beginScene();
    void endScene();

    // Settery
    void setPosition(float x, float y);
    void setTarget(Entity* newTarget);
    void setZoom(float newZoom);
    void setViewport(int width, int height);

    // Gettery
    Point2D getPosition() const;
    float getZoom() const;

    // Konwersja wspó³rzêdnych
    Point2D worldToScreen(const Point2D& worldPos) const;
    Point2D screenToWorld(const Point2D& screenPos) const;
};

#endif // CAMERA_H