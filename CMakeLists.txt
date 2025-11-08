cmake_minimum_required(VERSION 3.10)
project(OpenGLClion)

# Add glad source files
add_library(glad dependencies/include/glad/glad.c)
target_include_directories(glad PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/include")

# Your executable
add_executable(OpenGLClion src/main.cpp
        src/classes/shader.cpp
        src/classes/shader.h
        src/classes/game.cpp
        src/classes/game.h
        src/classes/grid.h
        src/classes/player.cpp
        src/classes/player.h
        src/classes/staticObject.cpp
        src/classes/staticObject.h
        src/classes/renderer.cpp
        src/classes/renderer.h
        src/classes/enemy.cpp
        src/classes/enemy.h
        src/classes/attributes.h)

# Include directories
target_include_directories(OpenGLClion PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/include")

# Link libraries statically
target_link_libraries(OpenGLClion
        glad
        "${CMAKE_SOURCE_DIR}/dependencies/dlls/libglfw3.a"  # Use .a not .lib
        opengl32
        gdi32
        user32
        kernel32
        -static-libgcc
        -static-libstdc++
        -static
)
