# **PGK 2D Engine**

![Game Preview](./.github/assets/preview.gif)
A simple 2D engine created in C++ using the Allegro 5 library for the Computer Graphics Fundamentals course. The engine allows the creation of simple 2D games, supporting basic mechanisms such as graphics rendering, input handling, resource management, and animations. The repository also includes a Top-Down Shooter demo game ("Arena Combat Game") showcasing the engine's capabilities.

## **📋 Table of Contents**

* [✨ Features](#-features)
* [🛠️ Tech Stack](#️-tech-stack)
* [🚀 Setup and Launch](#-setup-and-launch)
* [🕹️ Demo Game Controls](#️-demo-game-controls)
* [📂 Project Structure](#-project-structure)
* [💡 Future Development](#-future-development)

## **✨ Features**

* **2D Graphics Rendering System:**  
  * Support for geometric primitives (points, lines, triangles, rectangles, circles).  
  * Filling shapes with color.  
  * Object transformations (translation, rotation, scaling).  
* **Resource Management:**  
  * Texture manager (TextureManager) preventing multiple loads of the same texture.  
  * Support for Sprite objects with their own transformations.  
* **Animation System:**  
  * Frame-based animations using sprite sheets.  
  * Playback control (play, pause, stop, loop).  
* **Input Handling:**  
  * Reading keyboard state (isKeyDown).  
  * Reading mouse position and button states (getMouseX, getMouseY, isMouseButtonDown).  
* **Basic Physics and Collisions:**  
  * Simple collision system (CollisionManager) supporting shapes: circle, rectangle, triangle.  
  * Basic collision detection between objects (walls, entities, projectiles).  
* **Additional Components:**  
  * Logging system (Logger) with different levels (INFO, WARNING, ERROR, FATAL) and file logging (engine\_log.txt).  
  * Camera (Camera) that follows the player and manages the view.  
  * Bullet management system (BulletManager).  
  * "Dead body" management system (BodyManager) for visual effects.  
* **Demo Game:**  
  * Implementation of the "Arena Combat Game" Top-Down Shooter.  
  * Player logic (Player), enemies (Enemy) with simple AI.  
  * Map system (Map) with predefined obstacles.

## **🛠️ Tech Stack**

* **C++:** Main programming language.  
* **Allegro 5:** Multimedia library for handling 2D graphics, window, input, events.  
  * allegro\_primitives  
  * allegro\_image  
  * allegro\_font  
  * allegro\_ttf  
* **Visual Studio 2022:** Development environment (project configured for VS).

## **🚀 Setup and Launch**

### **System Requirements**

* Operating System: Windows 10/11  
* Visual Studio 2022  
* Allegro 5 library installed (recommended via NuGet)  
* Minimum 4GB RAM  
* Graphics card supporting DirectX 11

### **Environment Setup**

1. Clone the project repository:  
   git clone \<repository\_URL\>  
   cd \<repository\_folder\_name\>

2. Open the solution file PGK-Engine-Reorganized.sln in Visual Studio 2022\.  
3. Ensure you have the Allegro 5 library installed. The easiest way is using the NuGet Package Manager in Visual Studio:  
   * Right-click on the solution in Solution Explorer.  
   * Select "Manage NuGet Packages for Solution...".  
   * In the "Browse" tab, search for and install Allegro and AllegroDeps. They will be automatically downloaded for the project.

### **Compiling and Running the Demo Game**

1. In Visual Studio, select the configuration (e.g., Debug or Release) and architecture (x64).  
2. Compile and run the project by pressing Ctrl+F5 (Start Without Debugging) or F5 (Start With Debugging).

## **🕹️ Demo Game Controls ("Arena Combat Game")**

* **W, A, S, D:** Move character  
* **Mouse:** Aim  
* **LMB (Left Mouse Button):** Shoot  
* **R:** Reload weapon  
* **ESC:** Exit game

## **📂 Project Structure**

The project is divided into two main parts within the src/ folder:  
src/  
├── engine/         \# Engine core \- independent modules  
│   ├── Animation.h/.cpp  
│   ├── Engine.h/.cpp       \# Main engine class (Singleton)  
│   ├── Logger.h/.cpp  
│   ├── PrimitiveRenderer.h/.cpp  
│   ├── Primitives.h/.cpp   \# Shape definitions (Point2D, Rectangle, etc.)  
│   ├── Sprite.h/.cpp  
│   ├── TextureManager.h/.cpp  
│   └── Transform.h/.cpp  
│  
└── game/           \# Components specific to the demo game  
    ├── BodyManager.h/.cpp  
    ├── Bullet.h/.cpp  
    ├── BulletManager.h/.cpp  
    ├── Camera.h/.cpp  
    ├── CollisionManager.h/.cpp  
    ├── DeadBody.h/.cpp  
    ├── Enemy.h/.cpp  
    ├── Entity.h/.cpp       \# Base class for player and enemies  
    ├── Map.h/.cpp  
    └── Player.h/.cpp

assets/             \# Graphics resources (textures)  
PGK 2D Engine.cpp   \# Main entry point (main function)  
...                 \# Visual Studio configuration files (.sln, .vcxproj)

* **engine/**: Contains the core engine code, potentially reusable in other projects. Classes here are designed to be as general as possible.  
* **game/**: Contains code specific to the "Arena Combat Game" demo, implementing game logic, characters, map, etc., using components from engine/.

## **💡 Future Development**

The engine provides a foundation that can be further developed. Potential directions for expansion:

* **Enhanced Graphics:** Adding support for particle effects, lighting, shadows.  
* **Optimization:** Implementing object culling (not rendering off-screen objects), optimizing resource management.  
* **Physics:** Expanding the collision system, adding more advanced physics (e.g., forces, friction).  
* **Audio:** Integrating with Allegro's audio system (allegro\_audio, allegro\_acodec).  
* **User Interface (UI):** Creating a system for easily building UI elements (buttons, health bars, menus).  
* **Controller Support:** Adding gamepad input handling.  
* **Level Editor:** A tool for visually creating maps and placing objects.