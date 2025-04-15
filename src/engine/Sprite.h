// Sprite.h
#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Primitives.h"
#include "Animation.h"

class Sprite {
private:
    ALLEGRO_BITMAP* texture;
    Point2D position;
    Point2D scale;
    float rotation;
    float alpha;

    // Mapy animacji i stanu animacji
    std::map<std::string, Animation*> animations;
    Animation* currentAnimation;

    // èrÛd≥owy prostokπt dla sprite sheet
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
    void setAlpha(float alpha);
    void setSourceRect(int x, int y, int width, int height);
    void resetSourceRect();

    // Zarzπdzanie animacjami 
    void addAnimation(Animation* animation);
    void playAnimation(const std::string& name);
    void stopAnimation();
    void pauseAnimation();
    void resumeAnimation();
    void updateAnimation(float deltaTime);
    Animation* getCurrentAnimation() const;

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
