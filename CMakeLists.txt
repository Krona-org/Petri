cmake_minimum_required(VERSION 3.16)
project(OpenGLApp LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Исходники ===
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
)

add_executable(app ${SOURCES})

# === Include пути ===
target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/inc        # glad, GLFW, glm
    ${CMAKE_SOURCE_DIR}/src        # main.cpp и др.
    ${CMAKE_SOURCE_DIR}/src/graphics    # shaderClass.h, VAO.h, VBO.h, EBO.h, Camera.h 
    ${CMAKE_SOURCE_DIR}/src/scene
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/shaders
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/core
)

# === OpenGL ===
find_package(OpenGL REQUIRED)

# === Линковка библиотек ===
target_link_libraries(app PRIVATE
    OpenGL::GL
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
)

# === Копирование шейдеров после сборки === 
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:app>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        $<TARGET_FILE_DIR:app>/shaders
)
