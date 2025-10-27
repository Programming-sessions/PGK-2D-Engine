# Refactoring and Bug Fixes

This document summarizes the recent refactoring efforts and bug fixes applied to the PGK Engine.

## 1. Primitive Types Refactoring

The core of this refactoring was to modernize the engine's primitive types and remove redundant, custom-built functionalities that are already provided by the `glm` library.

### 1.1. Deprecation of `Point2D`

The custom `Point2D` class was removed entirely from the codebase. All occurrences of `Point2D` were replaced with `glm::vec2`. This change affects a large number of files, including:

*   `src/engine/Primitives.h` & `.cpp`
*   `src/engine/Sprite.h` & `.cpp`
*   `src/game/Entity.h` & `.cpp`
*   `src/game/Player.h` & `.cpp`
*   `src/game/Enemy.h` & `.cpp`
*   `src/game/Bullet.h` & `.cpp`
*   `src/game/Camera.h` & `.cpp`
*   `src/game/CollisionManager.h` & `.cpp`
*   `src/game/Map.h` & `.cpp`
*   `src/game/DeadBody.h` & `.cpp`
*   `src/game/BodyManager.h` & `.cpp`
*   `PGK 2D Engine.cpp`

### 1.2. Simplification of Primitive Classes

The primitive shape classes (`Rectangle`, `Circle`, `Triangle`, `LineSegment`) were significantly simplified:

*   All member variables and methods that used `Point2D` now use `glm::vec2`.
*   Custom transformation methods (`translate`, `rotate`, `scale`) were removed from these classes. All transformations are now expected to be handled by `glm`'s matrix operations, which are more efficient and powerful.
*   The `Rectangle` class was redesigned to be more flexible, now storing its position, size, and rotation, rather than the coordinates of its four corners.

## 2. Removal of `PrimitiveRenderer`

The `PrimitiveRenderer` class was identified as a source of compilation errors and was found to be used in only a very limited scope. As such, it has been removed from the project.

*   `src/engine/PrimitiveRenderer.h` and `src/engine/PrimitiveRenderer.cpp` were deleted.
*   All references to `PrimitiveRenderer` in `src/game/Map.h` and `src/game/Map.cpp` were removed.
*   The drawing logic in `Map.cpp` was updated to use direct Allegro 5 drawing functions (`al_draw_filled_rectangle`, `al_draw_filled_circle`, etc.).

## 3. Bug Fixes

The refactoring process introduced a few visual bugs, which have now been addressed:

### 3.1. Floating Dead Bodies

The `DeadBody::draw` function was manually applying camera transformations to the body's position, causing them to be drawn incorrectly. This was fixed by removing the manual transformation and passing the world coordinates directly to the Allegro drawing function.

### 3.2. Incorrect Bullet Spawning

A similar issue was found in the `Bullet::draw` function, as well as in the `Player::shoot` and `Enemy::shoot` methods. The bullet's initial position and its drawing coordinates were being incorrectly transformed. These issues were resolved by:

*   Removing the manual camera transformation from `Bullet::draw`.
*   Simplifying the bullet creation logic in `Player::shoot` and `Enemy::shoot` to correctly calculate the bullet's starting position in world space.

These changes have resulted in a cleaner, more modern, and more efficient codebase that makes better use of the `glm` library.