# 🕵️‍♂️ Mafia Revenge

## 📁 General Systems Descriptions

Below is a list of the general folder structure used for my game engine. 

```
game-engine/
    assets/
        animations/
        fonts/
        ui/
    src/
        ecs/
            event/
            system/
                AnimationCallbacks/
                event-response-systems/
        manager/
        scene/
        utils/
        vendor/
```

- Most of the code (mainly systems) will be located in the `/ecs` folder.

### 📬 Event Response Systems
```
event-response-systems/
    ActionRsponseSystem.cpp
    CollisionRsponseSystem.cpp
    MouseUIRsponseSystem.cpp
```
I have three general events that my response systems handle: Action, Collision, and MouseUI. 

The Action Response System handles specific player events, such as car interaction logic, 
driving logic and player win logic. There are different types of actions such as PlayerAction 
and CarAction and they each have their own respective Event type, which are defined in 
`src/ecs/event/BaseEvent.hpp`. They each have unique properties and are handled uniquely to 
account for their differing logic. 

The Collision Response System handles all collision related events. These are broken down into 
three general types of collisions: OnEnter, OnStay and OnExit. Making this separate from the other
response systems allows for better readability of the code and improved separation of concern.

The MouseUI Response Systems handle all UI mouse click events. These are also broken down into 
generic types such as Pressed, Released and Cancel. Due to time constraints, I was not able to build
upon this system, which explains the lack of implementation. 

### 🎬 Animation Callbacks
Animation callbacks are vital in defining animation behaviour. They can be used to define unique 
animation behaviour depending on their animation state, velocity, etc. I store all my animation 
callbacks in `src/ecs/system/AnimationCallbacks` to make code more readable. 
This helps improve readability of code, since each animation callback would take in the same parameters
and return the same values. This makes my AnimationSystem.hpp (`src/ecs/system/AnimationSystem.hpp`) a lot cleaner and easy to use.

### 🎯 Target Component & its Systems
There are many system that use the Target component, each of which use them in many different ways
```
system/
    TargetSystem.hpp
    RotationSystem.hpp
    Scene.cpp
```
- **TargetingSystem.hpp**: Defines the deltaX and deltaY values in the Transform component. 
- **RotationSystem.hpp**: Uses the deltaX and deltaY values to rotate the sprite in a certain angle
  - Eg. Assume cartesian plane, if the player is at (0,0) and the mouse is at (1,1), it rotates the sprite 
  until the sprites front faces perpendicular to the target point.
- **Scene.cpp**: Uses the deltaX and deltaY values to create normalized vectors that is then used to determine
the direction of projectiles. 

The targeting system is generalized in a way that it can make an entity track either the mouse or track another 
entity. This is how I can use the same system to make my player track my mouse and make the enemies track the player.

### 👁️ Keyboard and Camera Focus Tags
Files involved with Focus tags:
```
system/
    KeyboardInputSystem.cpp
    CameraSystem.hpp
    event-response-systems/
        ActionResponseSystem.cpp
```

One challenge that was not clear was transitioning controls and camera focus
from player to car and vice versa. The solution I came up with was CameraFocusTags and KeyboardFocusTags. 
Essentially, these components have a bool that determines whether the keyboard should be using player controls or car controls,
and determines which entity the camera should focus on. I have defined focus swapping behaviour in Action Response System, so 
it only swaps whenever I interact with the car. 

### 👆 Interaction Component
File the use Interaction Component:

```
system/
    CameraSystem.hpp
    KeyboardInputSystem.cpp
    Scene.cpp
```

This component is basically a simple bool that helps determine whether I am able to interact with the car. The behaviour for this is defined 
in CollisionSystem.cpp. It basically just checks if the player is close enough to the car to either allow or disallow car interaction.

### 🏎️ Physics System
File involved with physics system:

```
system/
    PhysicsSystem.hpp
```
The PhysicsSystem is responsible for updating entity movement by applying acceleration, deceleration, and directional logic in a generic, component-driven way.
Its what allows my car to gradually accelerate, decelerate and brake. It basically increases or decreases the speed of the 
entity bit by bit every update to give off the gradual acceleration effect. It is designed in a way that all someone has 
to do is define the max speed in the Velocity component, the acceleration factor in the Acceleration component, and the
brake factor in the Brake component. 

This system ensures not only real-feeling car mechanics, but also uses a Direction enum to 
ensure that the car is facing in the right direction. This is how I am able to reverse as well. Without the Direction enum 
I would not be able to reverse with my car. This system can also be easily extended into a simplified gravity system
using the Acceleration component to define the gravity of an entity.  

## 🕹️ Game Engine – Setup & Run Guide

This guide explains how to clone, build, and run the Game Engine project locally.

---

### 📦 Prerequisites

Before starting, make sure you have the following installed:

- **Git**
- **CMake (3.29 or higher)**
  - Can also use Visual Studio 2022 (recommended on Windows) along with a C++20 compatible compiler
- **SDL3**
- **SDL3_image**
- **SDL3_ttf (fonts)**

Make sure SDL3 and SDL3_image are properly installed and their `include`, `lib`, and `bin` directories are available.

---

### 📥 Clone the Repository

Open a terminal and run:

```bash
git clone https://github.com/Nav-Codes/game-engine
cd your-repo-name

```

### ⚙️ Configure SDL Paths

Before building, you need to update the SDL library paths in `CMakeLists.txt` to match where SDL is installed on your system.

### 🔧 Locate the SDL path section

Open `CMakeLists.txt` and find:

```cmake
if(WIN32)
    set(SDL3_PATH "C:/path/to/your/sdl3")
    set(SDL3_IMAGE_PATH "C:/path/to/your/sdl3_image")
    set(SDL3_TTF_PATH "C:/path/to/your/sdl3_ttf")
```    
Update the paths to each of the libraries to wherever you downloaded the libraries.    

### ⚠️ Common build/run issue

The SDL3 .dll's should come with the repo when you clone it.
If you are facing issues running the game because certain .dll's are not available.

1. Locate your SDL3 installation directory.
2. Go to:

```
SDL3-devel-3.2.20-mingw\SDL3-3.2.20\x86_64-w64-mingw32\bin
```

Copy:

```
SDL3.dll
```

3. Then go to your SDL3_image directory:

```
SDL3_image-devel-3.2.4-mingw\SDL3_image-3.2.4\x86_64-w64-mingw32\bin
```

Copy:

```
SDL3_image.dll
```

4. Then go to your SDL3_ttf directory:

```
SDL3_ttf-devel-3.2.2-mingw\SDL3_ttf-3.2.2\x86_64-w64-mingw32\bin
```

Copy:

```
SDL3_ttf.dll
```

5. Paste all .dll files into:

```
cmake-build-debug/
```

Place them in the **same folder as the generated `.exe` file**.

If these DLLs are missing, the program will not start and may exit with error code `0xC0000135`.
