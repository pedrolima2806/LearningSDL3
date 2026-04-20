cmake_minimum_required(VERSION 3.16)
project(SDL_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)

find_package(OpenGL REQUIRED)

add_executable(SDL_learning
        src/bmpImageDisplay.cpp
        src/textDisplay.cpp
        src/healthBar.cpp
        src/animatedSprites.cpp
        src/gameState.cpp
        src/camera.cpp
        src/noiseTerrain.cpp
        src/mixerTest.cpp
        src/cube.cpp
)

target_include_directories(SDL_learning PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL_image/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL_ttf/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL_mixer/include"
)

target_link_libraries(SDL_learning PRIVATE
        OpenGL::GL
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
)