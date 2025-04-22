#include "DeadBody.h"

DeadBody::DeadBody(const Point2D& position, float rotation, const std::string& entityTag)
    : position(position)
    , rotation(rotation)
    , entityType(entityTag)
{
    loadAppropriateTexture();
}

void DeadBody::loadAppropriateTexture() {
    if (entityType == "Enemy") {
        texture = TextureManager::getInstance()->loadTexture("assets/textures/enemy_dead.png");
    }
    else if (entityType == "Player") {
        texture = TextureManager::getInstance()->loadTexture("assets/textures/hero_dead.png");
    }
    // Mo�na doda� wi�cej typ�w w przysz�o�ci
}

void DeadBody::draw() {
    if (!texture) return;

    ALLEGRO_TRANSFORM currentTransform;
    al_copy_transform(&currentTransform, al_get_current_transform());

    float screenX = position.getX() * currentTransform.m[0][0] + currentTransform.m[2][0];
    float screenY = position.getY() * currentTransform.m[1][1] + currentTransform.m[2][1];

    // Rysuj tekstur� z uwzgl�dnieniem �rodka
    al_draw_rotated_bitmap(texture,
        90, 95,  // Zak�adamy �e wszystkie tekstury maj� ten sam rozmiar, mo�na to te� parametryzowa�
        screenX, screenY,
        rotation,
        0
    );
}
