// src/game/Camera.cpp
#include "Camera.h"
#include "../engine/Engine.h"

Camera::Camera()
    : position(0.0f, 0.0f)
    , zoom(1.0f)
    , target(nullptr)
    , viewportWidth(800)
    , viewportHeight(600)
{
}

Camera::~Camera() {
}

void Camera::update() {
    if (target) {
        // P³ynne œledzenie celu (gracza)
        Point2D targetPos = target->getPosition();
        position = targetPos;
    }
}

void Camera::beginScene() {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);

    // Przesuniêcie do œrodka ekranu
    al_translate_transform(&transform,
        viewportWidth / 2.0f - position.getX() * zoom,
        viewportHeight / 2.0f - position.getY() * zoom);

    // Skalowanie (zoom)
    al_scale_transform(&transform, zoom, zoom);

    al_use_transform(&transform);
}

void Camera::endScene() {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_use_transform(&transform);
}

void Camera::setPosition(float x, float y) {
    position.setPosition(x, y);
}

void Camera::setTarget(Entity* newTarget) {
    target = newTarget;
}

void Camera::setZoom(float newZoom) {
    zoom = newZoom;
}

void Camera::setViewport(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}

Point2D Camera::getPosition() const {
    return position;
}

float Camera::getZoom() const {
    return zoom;
}

Point2D Camera::worldToScreen(const Point2D& worldPos) const {
    float screenX = (worldPos.getX() - position.getX()) * zoom + viewportWidth / 2.0f;
    float screenY = (worldPos.getY() - position.getY()) * zoom + viewportHeight / 2.0f;
    return Point2D(screenX, screenY);
}

Point2D Camera::screenToWorld(const Point2D& screenPos) const {
    float worldX = (screenPos.getX() - viewportWidth / 2.0f) / zoom + position.getX();
    float worldY = (screenPos.getY() - viewportHeight / 2.0f) / zoom + position.getY();
    return Point2D(worldX, worldY);
}
