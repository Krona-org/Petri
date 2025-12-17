cmake_minimum_required(VERSION 3.16)
project(OpenGLApp LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# ================= PETRI =================
file(GLOB_RECURSE PETRI_SOURCES
    petri_log/*.cpp
)

add_library(petri STATIC ${PETRI_SOURCES})

target_include_directories(petri PUBLIC
    ${CMAKE_SOURCE_DIR}/petri_log
)

# ================= APP =================
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
)

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/graphics
    ${CMAKE_SOURCE_DIR}/src/scene
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/shaders
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/core
)

# ================= ImGui =================
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui_master)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(imgui PUBLIC
    OpenGL::GL
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
)

# ================= LINK =================
target_link_libraries(app PRIVATE
    petri
    imgui
    OpenGL::GL
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
)

# ================= SHADERS =================
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:app>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        $<TARGET_FILE_DIR:app>/shaders
)
