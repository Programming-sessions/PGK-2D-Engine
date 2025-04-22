#ifndef DEAD_BODY_H
#define DEAD_BODY_H

#include "../engine/TextureManager.h"
#include "../engine/Primitives.h"
#include <allegro5/allegro.h>
#include <string>

class DeadBody {
private:
    ALLEGRO_BITMAP* texture;
    Point2D position;
    float rotation;
    std::string entityType;

    void loadAppropriateTexture();

public:
    DeadBody(const Point2D& position, float rotation, const std::string& entityTag);

    void draw();
};

#endif // DEAD_BODY_H
