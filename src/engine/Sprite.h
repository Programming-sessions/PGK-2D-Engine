/**
 * @file Sprite.h
 * @brief Class representing a 2D graphic object with animation support.
 *
 * The Sprite class manages a texture, position, rotation, scaling, and animations.
 * It allows for displaying both static and animated 2D objects
 * using the Allegro 5 library.
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include <memory>
#include "Primitives.h"
#include "Animation.h"

 /**
  * @brief Class representing a sprite (2D graphic object).
  *
  * Allows for:
  * - Loading and displaying textures
  * - Transformations (position, rotation, scale)
  * - Managing transparency
  * - Support for sprite sheets
  * - Managing animations
  */
class Sprite {
private:
    ALLEGRO_BITMAP* texture;   ///< Pointer to the texture
    glm::vec2 position;          ///< Position of the sprite
    glm::vec2 scale;             ///< Scale on the X and Y axes
    float rotation;            ///< Angle of rotation in radians
    float alpha;               ///< Transparency (0.0 - 1.0)

    std::map<std::string, std::unique_ptr<Animation>> animations;  ///< Map of available animations
    Animation* currentAnimation;                                 ///< Pointer to the current animation

    // Source area parameters for sprite sheet
    int sourceX;        ///< X position in the source texture
    int sourceY;        ///< Y position in the source texture
    int sourceWidth;    ///< Width of the source area
    int sourceHeight;   ///< Height of the source area

public:
    /**
     * @brief Default constructor.
     *
     * Initializes the sprite with default values:
     * - Position (0,0)
     * - Scale (1,1)
     * - Rotation 0
     * - Alpha 1.0
     * - No texture or animation
     */
    Sprite();

    /**
     * @brief Destructor.
     *
     * Frees animation resources; the texture is managed by TextureManager.
     */
    ~Sprite();

    /**
     * @brief Loads a texture from a file.
     * @param path Path to the texture file.
     * @return true if loaded successfully.
     */
    bool loadTexture(const std::string& path);

    /**
     * @brief Sets the sprite's position.
     * @param x X coordinate.
     * @param y Y coordinate.
     */
    void setPosition(float x, float y);
    void setPosition(const glm::vec2& pos);

    /**
     * @brief Sets the rotation angle.
     * @param angle Angle in radians.
     */
    void setRotation(float angle);

    /**
     * @brief Sets the sprite's scale.
     * @param scaleX Scale on the X axis.
     * @param scaleY Scale on the Y axis.
     */
    void setScale(float scaleX, float scaleY);
    void setScale(const glm::vec2& newScale);

    /**
     * @brief Sets the transparency.
     * @param alpha Alpha value (0.0 - fully transparent, 1.0 - opaque).
     */
    void setAlpha(float alpha);

    /**
     * @brief Sets the source area in a sprite sheet.
     * @param x X position in the texture.
     * @param y Y position in the texture.
     * @param width Width of the area.
     * @param height Height of the area.
     */
    void setSourceRect(int x, int y, int width, int height);

    /**
     * @brief Resets the source area to the full size of the texture.
     */
    void resetSourceRect();

    /**
     * @brief Adds a new animation.
     * @param animation A unique_ptr to the animation object.
     *
     * If an animation with the same name already exists, it will be replaced.
     * The Sprite takes ownership of the animation.
     */
    void addAnimation(std::unique_ptr<Animation> animation);

    /**
     * @brief Starts playing an animation.
     * @param name Name of the animation.
     */
    void playAnimation(const std::string& name);

    /**
     * @brief Stops the current animation and rewinds to the first frame.
     */
    void stopAnimation();

    /**
     * @brief Pauses the current animation.
     */
    void pauseAnimation();

    /**
     * @brief Resumes a paused animation.
     */
    void resumeAnimation();

    /**
     * @brief Updates the animation state.
     * @param deltaTime Time since the last update in seconds.
     */
    void updateAnimation(float deltaTime);

    /**
     * @brief Gets a pointer to the current animation.
     * @return Pointer to the current animation or nullptr if none.
     */
    Animation* getCurrentAnimation() const;

    /**
     * @brief Gets the current position.
     * @return glm::vec2 object with the position.
     */
    glm::vec2 getPosition() const;

    /**
     * @brief Gets the current scale.
     * @return glm::vec2 object with the X and Y scale.
     */
    glm::vec2 getScale() const;

    /**
     * @brief Gets the current rotation angle.
     * @return Angle in radians.
     */
    float getRotation() const;

    /**
     * @brief Gets the current transparency.
     * @return Alpha value (0.0 - 1.0).
     */
    float getAlpha() const;

    /**
     * @brief Gets the width of the sprite.
     * @return Width in pixels.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the sprite.
     * @return Height in pixels.
     */
    int getHeight() const;

    /**
     * @brief Draws the sprite on the screen.
     *
     * Takes into account all transformations (position, rotation, scale),
     * transparency, and the current animation frame if active.
     */
    void draw();
};

#endif // SPRITE_H
