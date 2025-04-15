#include "Sprite.h"
#include "TextureManager.h"

Sprite::Sprite()
    : texture(nullptr)
    , position(0.0f, 0.0f)
    , scale(1.0f, 1.0f)
    , rotation(0.0f)
    , alpha(1.0f)
    , currentAnimation(nullptr)
    , sourceX(0)
    , sourceY(0)
    , sourceWidth(0)
    , sourceHeight(0)
{
}

Sprite::~Sprite() {
    // Czyszczenie animacji
    for (auto& pair : animations) {
        delete pair.second;
    }
    animations.clear();
}

bool Sprite::loadTexture(const std::string& path) {
    texture = TextureManager::getInstance()->loadTexture(path);
    if (texture) {
        sourceWidth = al_get_bitmap_width(texture);
        sourceHeight = al_get_bitmap_height(texture);
        return true;
    }
    return false;
}

void Sprite::setPosition(float x, float y) {
    position.setPosition(x, y);
}

void Sprite::setRotation(float angle) {
    rotation = angle;
}

void Sprite::setScale(float scaleX, float scaleY) {
    scale.setPosition(scaleX, scaleY);
}

void Sprite::setAlpha(float newAlpha) {
    alpha = newAlpha < 0.0f ? 0.0f : (newAlpha > 1.0f ? 1.0f : newAlpha);
}

void Sprite::setSourceRect(int x, int y, int width, int height) {
    sourceX = x;
    sourceY = y;
    sourceWidth = width;
    sourceHeight = height;
}

void Sprite::resetSourceRect() {
    if (texture) {
        sourceX = sourceY = 0;
        sourceWidth = al_get_bitmap_width(texture);
        sourceHeight = al_get_bitmap_height(texture);
    }
}

void Sprite::addAnimation(Animation* animation) {
    if (animation) {
        // Jeœli animacja o tej nazwie ju¿ istnieje, usuñ j¹
        auto it = animations.find(animation->getName());
        if (it != animations.end()) {
            delete it->second;
        }

        animations[animation->getName()] = animation;

        // Jeœli to pierwsza animacja, ustaw j¹ jako aktualn¹
        if (!currentAnimation) {
            currentAnimation = animation;
        }
    }
}

void Sprite::playAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (currentAnimation != it->second) {
            if (currentAnimation) {
                currentAnimation->stop();
            }
            currentAnimation = it->second;
        }
        currentAnimation->play();
    }
}

void Sprite::stopAnimation() {
    if (currentAnimation) {
        currentAnimation->stop();
    }
}

void Sprite::pauseAnimation() {
    if (currentAnimation) {
        currentAnimation->pause();
    }
}

void Sprite::resumeAnimation() {
    if (currentAnimation) {
        currentAnimation->play();
    }
}

void Sprite::updateAnimation(float deltaTime) {
    if (currentAnimation) {
        currentAnimation->update(deltaTime);

        // Aktualizacja Ÿród³owego prostok¹ta na podstawie aktualnej klatki
        const AnimationFrame& frame = currentAnimation->getCurrentFrame();
        sourceX = frame.x;
        sourceY = frame.y;
        sourceWidth = frame.width;
        sourceHeight = frame.height;
    }
}

Animation* Sprite::getCurrentAnimation() const {
    return currentAnimation;
}

Point2D Sprite::getPosition() const {
    return position;
}

Point2D Sprite::getScale() const {
    return scale;
}

float Sprite::getRotation() const {
    return rotation;
}

float Sprite::getAlpha() const {
    return alpha;
}

int Sprite::getWidth() const {
    return sourceWidth;
}

int Sprite::getHeight() const {
    return sourceHeight;
}

void Sprite::draw() {
    if (!texture) return;

    // Zachowanie obecnego stanu transformacji
    ALLEGRO_TRANSFORM transform;
    al_copy_transform(&transform, al_get_current_transform());

    // Ustawienie nowej transformacji dla sprite'a
    ALLEGRO_TRANSFORM spriteTransform;
    al_identity_transform(&spriteTransform);
    al_translate_transform(&spriteTransform, -sourceWidth / 2.0f, -sourceHeight / 2.0f);
    al_scale_transform(&spriteTransform, scale.getX(), scale.getY());
    al_rotate_transform(&spriteTransform, rotation);
    al_translate_transform(&spriteTransform, position.getX(), position.getY());
    al_compose_transform(&spriteTransform, &transform);

    // Zastosowanie transformacji
    al_use_transform(&spriteTransform);

    // Rysowanie z uwzglêdnieniem przezroczystoœci i aktualnej klatki animacji
    al_draw_tinted_bitmap_region(
        texture,
        al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha),
        sourceX, sourceY,
        sourceWidth, sourceHeight,
        0, 0,
        0
    );

    // Przywrócenie poprzedniej transformacji
    al_use_transform(&transform);
}
