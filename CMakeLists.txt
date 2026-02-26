cmake_minimum_required(VERSION 3.29)
project(game-engine)

set(CMAKE_CXX_STANDARD 20)

#Set SDL3 include and lib paths

if(WIN32)
    set(SDL3_PATH "C:/Program Files/SDL/sdl3")
    set(SDL3_IMAGE_PATH "C:/Program Files/SDL/sdl3_image")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/src/utils
            ${PROJECT_SOURCE_DIR}/src/ecs
            ${PROJECT_SOURCE_DIR}/src/ecs/system
            ${PROJECT_SOURCE_DIR}/src/vendor
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
    )
endif()

add_executable(game-engine src/main.cpp
        src/Game.cpp
        src/TextureManager.hpp
        src/TextureManager.cpp
        src/Game.hpp
        src/Map.hpp
        src/Map.cpp
        src/GameObject.hpp
        src/GameObject.cpp
        src/ecs/ComponentType.hpp
        src/ecs/Entity.hpp
        src/ecs/Component.hpp
        src/ecs/system/Movement.hpp
        src/ecs/system/RenderSystem.hpp
        src/ecs/World.hpp
        src/utils/Vector2D.hpp
        src/ecs/system/KeyboardInputSystem.hpp
        src/utils/Vector2D.cpp
        src/utils/Collision.hpp
        src/utils/Collision.cpp
        src/ecs/system/CollisionSystem.hpp
        src/ecs/system/CollisionSystem.cpp
        src/ecs/EventManager.hpp
        src/ecs/World.cpp
        src/vendor/tinyxml2.h
        src/vendor/tinyxml2.cpp
        src/ecs/system/AnimationClip.hpp
        src/manager/AssetManager.hpp
        src/manager/AssetManager.cpp
        src/ecs/system/AnimationSystem.hpp
        src/ecs/system/CameraSystem.hpp
        src/ecs/system/SpawnTimerSystem.hpp
        src/ecs/system/DestructionSystem.hpp
        src/manager/Scene.hpp
        src/manager/Scene.cpp
        src/manager/SceneManager.hpp
)

target_link_libraries(game-engine
    SDL3 SDL3_IMAGE
)