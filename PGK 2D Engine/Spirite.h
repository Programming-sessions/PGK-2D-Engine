// Sprite.h
#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>
#include "TextureManager.h"
#include "Point2D.h"

class Sprite {
private:
    ALLEGRO_BITMAP* texture;
    Point2D position;
    Point2D scale;
    float rotation;
    float alpha;

    // èrÛd≥owy prostokπt dla sprite sheet
    ALLEGRO_BITMAP* sourceRect;
    int sourceX;
    int sourceY;
    int sourceWidth;
    int sourceHeight;

public:
    Sprite();
    ~Sprite();

    // Podstawowe ustawienia
    bool loadTexture(const std::string& path);
    void setPosition(float x, float y);
    void setRotation(float angle);
    void setScale(float scaleX, float scaleY);
    void setAlpha(float alpha);  // 0.0f - 1.0f

    // Obs≥uga sprite sheet
    void setSourceRect(int x, int y, int width, int height);
    void resetSourceRect();  // Uøywa ca≥ej tekstury

    // Gettery
    Point2D getPosition() const;
    Point2D getScale() const;
    float getRotation() const;
    float getAlpha() const;
    int getWidth() const;
    int getHeight() const;

    // Rysowanie
    void draw();
};

#endif // SPRITE_H
