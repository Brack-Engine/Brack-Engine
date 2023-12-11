cmake_minimum_required(VERSION 3.25.1)
project(Brack_Engine)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

if (WIN32 OR WIN64)
    set(SDL2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL")
    set(SDL2_TTF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL")
    set(SDL2_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL")

    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_DIR})

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
else ()
    find_package(PkgConfig REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS})
endif ()

add_library(${PROJECT_NAME} SHARED
        outfacingInterfaces/Components/IComponent.hpp
        outfacingInterfaces/ISystem.hpp
        src/Managers/Entities/EntityManager.cpp
        src/includes/EntityManager.hpp
        src/includes/ComponentStore.hpp
        src/Managers/Systems/SystemManager.cpp
        src/includes/SystemManager.hpp
        src/Wrappers/RenderWrapper.cpp
        outfacingInterfaces/Components/ChildComponent.hpp
        outfacingInterfaces/Components/ParentComponent.hpp
        outfacingInterfaces/Components/ObjectInfoComponent.hpp
        outfacingInterfaces/Components/RigidBodyComponent.hpp
        outfacingInterfaces/Components/AIComponent.hpp
        outfacingInterfaces/Components/TransformComponent.hpp
        src/Helpers/Vector2.cpp
        outfacingInterfaces/Components/VelocityComponent.hpp
        outfacingInterfaces/Components/BoxCollisionComponent.hpp
        outfacingInterfaces/Components/CircleCollisionComponent.hpp
        outfacingInterfaces/Components/CameraComponent.hpp
        outfacingInterfaces/Helpers/Color.hpp
        outfacingInterfaces/Components/SpriteComponent.hpp
        outfacingInterfaces/Components/AnimationComponent.hpp
        outfacingInterfaces/Components/ParticleComponent.hpp
        outfacingInterfaces/Components/ParticleEmitterComponent.hpp
        outfacingInterfaces/Components/InputTag.hpp
        outfacingInterfaces/Components/PersistenceTag.hpp
        src/Systems/RenderingSystem.cpp
        src/Systems/AnimationSystem.cpp
        src/Systems/MovementSystem.cpp
        src/Systems/BehaviourScriptSystem.cpp
        src/Systems/ParticleSystem.cpp
        src/Systems/AISystem.cpp
        src/Systems/LoadSystem.cpp
        src/Systems/SaveSystem.cpp
        src/Systems/InputSystem.cpp
        src/Wrappers/InputWrapper.cpp
        src/Systems/CollisionSystem.cpp
        src/Systems/PhysicsSystem.cpp
        src/Systems/TimerSystem.cpp
        src/Systems/AudioSystem.cpp
        src/Wrappers/AudioWrapper.cpp
        src/Wrappers/PhysicsWrapper.cpp
        outfacingInterfaces/Components/UIComponent.hpp
        outfacingInterfaces/Components/ClickableComponent.hpp
        outfacingInterfaces/Components/TextComponent.hpp
        src/Objects/GameObject.cpp
        src/Objects/Camera.cpp
        src/Objects/Scene.cpp
        src/Objects/Button.cpp
        src/Objects/Text.cpp
        src/EngineManagers/SceneManager.cpp
        src/EngineManagers/InputManager.cpp
        outfacingInterfaces/Helpers/InputState.hpp
        src/BrackEngine.cpp
        src/GameObjectConverter.cpp
        src/Logger.hpp
        src/Logger.cpp
        src/ConfigSingleton.cpp
        src/Config.cpp
        outfacingInterfaces/Helpers/KeyMap.hpp
        outfacingInterfaces/Helpers/MouseMap.hpp
        src/FPSSingleton.cpp
        src/Wrappers/FPSWrapper.cpp
        src/Systems/ClickSystem.cpp
        src/Systems/ClickSystem.hpp
        outfacingInterfaces/Components/Archetypes/AudioArchetype.hpp
        src/Helpers/UserLogger.cpp
        outfacingInterfaces/Helpers/UserLogger.hpp
        outfacingInterfaces/Components/RectangleComponent.hpp
        outfacingInterfaces/Components/SoundTrackComponent.hpp
        outfacingInterfaces/Components/SoundEffectComponent.hpp
        src/Managers/Entities/ComponentStore.cpp
        outfacingInterfaces/Helpers/CollisionType.hpp
        src/Systems/ReplaySystem.hpp
        src/Systems/ReplaySystem.cpp
        src/Managers/ReplayManager.cpp
        outfacingInterfaces/EngineManagers/ReplayManager.hpp
        outfacingInterfaces/Milliseconds.hpp
        outfacingInterfaces/Components/Archetypes/CollisionArchetype.hpp
        outfacingInterfaces/Components/TileMapComponent.hpp
        src/Managers/Entities/BehaviourScriptStore.cpp
        outfacingInterfaces/BehaviourScripts/BehaviourScript.cpp
        outfacingInterfaces/BehaviourScripts/IBehaviourScript.hpp
)

if (WIN32 OR WIN64)
    # Use the bundled Box2D for Windows
    set(BOX2D_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/Box2d")
    add_subdirectory(${BOX2D_PATH})
    include_directories(${BOX2D_PATH})
    target_link_libraries(${PROJECT_NAME} PRIVATE box2d)

elseif (APPLE)

    find_library(BOX2D_LIBRARY NAMES box2d)
    find_path(BOX2D_INCLUDE_DIR Box2D/Box2D.h)

    message(STATUS "Box2D Library: ${BOX2D_LIBRARY}")
    message(STATUS "Box2D Include: ${BOX2D_INCLUDE_DIR}")

    target_link_libraries(${PROJECT_NAME} PRIVATE ${BOX2D_LIBRARY})
    target_include_directories(${PROJECT_NAME} PRIVATE ${BOX2D_INCLUDE_DIR})
elseif (UNIX)
    set(BOX2D_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/Box2d")
    add_subdirectory(${BOX2D_PATH})
    target_link_libraries(${PROJECT_NAME} PRIVATE box2d)
endif ()

option(USER_LOG_TO_FILE "Enable user logging to a file" OFF)

if (USER_LOG_TO_FILE)
    target_compile_definitions(Brack_Engine PRIVATE USER_LOG_TO_FILE)
endif ()

if (NOT DEFINED USER_CURRENT_LOG_LEVEL)
    # Define a default log level based on the build type
    if (CMAKE_BUILD_TYPE STREQUAL "Info")
        set(USER_CURRENT_LOG_LEVEL "LOG_LEVEL_INFO")
    elseif (CMAKE_BUILD_TYPE STREQUAL "None")
        set(USER_CURRENT_LOG_LEVEL "LOG_LEVEL_NONE")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Warning")
        set(USER_CURRENT_LOG_LEVEL "LOG_LEVEL_WARNING")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(USER_CURRENT_LOG_LEVEL "LOG_LEVEL_ERROR")
    else ()
        set(USER_CURRENT_LOG_LEVEL "LOG_LEVEL_DEBUG" CACHE STRING "Set the current log level")
    endif ()
endif ()

add_compile_definitions(USER_CURRENT_LOG_LEVEL=${USER_CURRENT_LOG_LEVEL})

# Define an option for logging to a file
option(LOG_TO_FILE "Enable logging to a file" OFF)

# Check if LOG_TO_FILE option is set to ON and add a compile definition accordingly
if (LOG_TO_FILE)
    target_compile_definitions(Brack_Engine PRIVATE LOG_TO_FILE)
endif ()

if (NOT DEFINED CURRENT_LOG_LEVEL)
    # Define a default log level based on the build type
    if (CMAKE_BUILD_TYPE STREQUAL "Info")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_INFO")
    elseif (CMAKE_BUILD_TYPE STREQUAL "None")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_NONE")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Warning")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_WARNING")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_ERROR")
    else ()
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_DEBUG" CACHE STRING "Set the current log level")
    endif ()
endif ()

# Now, add the definition for the CURRENT_LOG_LEVEL
add_compile_definitions(CURRENT_LOG_LEVEL=${CURRENT_LOG_LEVEL})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/FMod/include)

target_include_directories(Brack_Engine PUBLIC outfacingInterfaces)

set(LIBRARY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/dll)


if (WIN32 OR WIN64)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})

    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIBRARY})

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBRARY_FILES}
            ${CMAKE_BINARY_DIR}
    )
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIBRARIES})
endif ()

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/FMod)

if (WIN32 OR WIN64)
    include_directories(${CMAKE_SOURCE_DIR}/lib/FMod/include)
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/FMod)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmod)

elseif (APPLE)
    find_library(FMOD_LIBRARY NAMES fmod PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/FMod")
    find_path(FMOD_INCLUDE_DIR fmod.h PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/FMod/include")

    message(STATUS "FMOD Library: ${FMOD_LIBRARY}")
    message(STATUS "FMOD Include: ${FMOD_INCLUDE_DIR}")

    target_link_libraries(${PROJECT_NAME} PRIVATE ${FMOD_LIBRARY})
    target_include_directories(${PROJECT_NAME} PRIVATE ${FMOD_INCLUDE_DIR})
elseif (UNIX)
    find_library(FMOD_LIBRARY NAMES libfmod.so.13 PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/FMod")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FMOD_LIBRARY})
endif ()


target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_TTF_LIBRARIES})


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/outfacingInterfaces)
