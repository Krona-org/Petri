#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Scene.h"

class Window
{
public:
    int width, height;
    GLFWwindow* window;

    Window(int w, int h, const std::string& title);
    ~Window();

    bool ShouldClose();
    void SwapBuffers();
    void PollEvents();

    void SetFramebufferSizeCallback(GLFWframebuffersizefun callback);
};
