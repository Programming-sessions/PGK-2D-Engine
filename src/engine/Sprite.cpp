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
    // Animations are now managed by unique_ptr, no manual deletion needed.
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
    position.x = x;
    position.y = y;
}

void Sprite::setPosition(const glm::vec2& pos) {
    position = pos;
}

void Sprite::setRotation(float angle) {
    rotation = angle;
}

void Sprite::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

void Sprite::setScale(const glm::vec2& newScale) {
    scale = newScale;
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

void Sprite::addAnimation(std::unique_ptr<Animation> animation) {
    if (animation) {
        const std::string& name = animation->getName();
        // If an animation with this name already exists, it will be replaced.
        animations[name] = std::move(animation);

        // If this is the first animation, set it as the current one
        if (!currentAnimation) {
            currentAnimation = animations[name].get();
        }
    }
}

void Sprite::playAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it == animations.end()) {
        return; // Animation not found
    }

    Animation* newAnimation = it->second.get();
    if (currentAnimation == newAnimation) {
        currentAnimation->play(); // Already the current animation, just ensure it's playing
        return;
    }

    if (currentAnimation) {
        currentAnimation->stop();
    }

    currentAnimation = newAnimation;
    currentAnimation->play();
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

        // Update the source rectangle based on the current frame
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

glm::vec2 Sprite::getPosition() const {
    return position;
}

glm::vec2 Sprite::getScale() const {
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

    al_draw_tinted_scaled_rotated_bitmap_region(
        texture,
        sourceX, sourceY, sourceWidth, sourceHeight,
        al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha),
        sourceWidth / 2.0f, sourceHeight / 2.0f,
        position.x, position.y,
        scale.x, scale.y,
        rotation,
        0
    );
}
