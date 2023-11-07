cmake_minimum_required(VERSION 3.25.1)
project(Brack_Engine)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")


if (WIN32 OR WIN64)
    set(SDL2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
else ()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS})
endif ()

add_library(${PROJECT_NAME} SHARED
        outfacingInterfaces/Components/IComponent.hpp
        outfacingInterfaces/ISystem.hpp
        src/Managers/Entities/EntityManager.cpp
        src/includes/EntityManager.hpp
        src/Managers/Components/ComponentStore.cpp
        src/includes/ComponentStore.hpp
        src/Managers/Systems/SystemManager.cpp
        src/includes/SystemManager.hpp
        src/Managers/Components/SparseSet.cpp
        src/includes/SparseSet.hpp
        outfacingInterfaces/IComponentStorage.hpp
        src/Wrappers/RenderWrapper.cpp
        outfacingInterfaces/Components/ChildComponent.hpp
        outfacingInterfaces/Components/ParentComponent.hpp
        outfacingInterfaces/Components/ObjectInfoComponent.hpp
        outfacingInterfaces/Components/RigidBodyComponent.hpp
        outfacingInterfaces/Components/AIComponent.hpp
        outfacingInterfaces/Components/AudioComponent.hpp
        outfacingInterfaces/Components/TransformComponent.hpp
        src/Helpers/Vector2.cpp
        outfacingInterfaces/Components/VelocityComponent.hpp
        outfacingInterfaces/Components/CollisionComponent.hpp
        outfacingInterfaces/Components/BoxCollisionComponent.hpp
        outfacingInterfaces/Components/CircleCollisionComponent.hpp
        outfacingInterfaces/Components/CameraComponent.hpp
        outfacingInterfaces/Helpers/Color.hpp
        outfacingInterfaces/Components/SpriteComponent.hpp
        outfacingInterfaces/Components/AnimationComponent.hpp
        outfacingInterfaces/Components/ParticleComponent.hpp
        outfacingInterfaces/Components/ParticleEmitterComponent.hpp
        outfacingInterfaces/Components/BehaviourScript.hpp
        outfacingInterfaces/Components/InputTag.hpp
        outfacingInterfaces/Components/PersistenceTag.hpp
        src/Systems/RenderingSystem.cpp
        src/Systems/AnimationSystem.cpp
        src/Systems/MovementSystem.cpp
        src/Systems/BehaviourScriptSystem.cpp
        src/Systems/ParticleSystem.cpp
        src/Systems/ReplaySystem.cpp
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
        outfacingInterfaces/Components/ButtonComponent.hpp
        outfacingInterfaces/Components/TextComponent.hpp
        src/Objects/GameObject.cpp
        src/Objects/Camera.cpp
        src/Objects/Scene.cpp
        src/Objects/UIObject.cpp
        src/Objects/Button.cpp
        src/Objects/Text.cpp
        src/EngineManagers/SceneManager.cpp
        src/EngineManagers/ReplayManager.cpp
        src/EngineManagers/InputManager.cpp
        outfacingInterfaces/Helpers/InputState.hpp
        src/BrackEngine.cpp
        src/GameObjectConverter.cpp
        src/Logger.hpp
        src/Logger.cpp
        src/ConfigSingleton.cpp
        src/Config.cpp
        src/FPSSingleton.cpp
        src/Wrappers/FPSWrapper.cpp
)

# Define an option for logging to a file
option(LOG_TO_FILE "Enable logging to a file" OFF)

# Check if LOG_TO_FILE option is set to ON and add a compile definition accordingly
if(LOG_TO_FILE)
    target_compile_definitions(Brack_Engine PRIVATE LOG_TO_FILE)
endif()

if(NOT DEFINED CURRENT_LOG_LEVEL)
    # Define a default log level based on the build type
    if(CMAKE_BUILD_TYPE STREQUAL "Info")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_INFO")
    elseif(CMAKE_BUILD_TYPE STREQUAL "None")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_NONE")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Warning")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_WARNING")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_ERROR")
    else ()
        set(CURRENT_LOG_LEVEL "LOG_LEVEL_DEBUG" CACHE STRING "Set the current log level")
    endif()
endif()

# Now, add the definition for the CURRENT_LOG_LEVEL
add_compile_definitions(CURRENT_LOG_LEVEL=${CURRENT_LOG_LEVEL})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/FMod/include)

target_include_directories(Brack_Engine PUBLIC outfacingInterfaces)

set(LIBRARY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/dll)


if (WIN32 OR WIN64)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})
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
endif ()

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/FMod)
target_link_libraries(${PROJECT_NAME} PRIVATE fmod)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/outfacingInterfaces)
