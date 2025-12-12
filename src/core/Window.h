#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
    GLFWwindow* window;
    int width, height;
    std::string title;

public:
    Window(int w, int h, const std::string& t);
    ~Window();

    bool Init();                 // Создание окна и инициализация GLAD
    void Clear(float r, float g, float b, float a);
    void SwapBuffers();
    void PollEvents();
    bool ShouldClose();
    GLFWwindow* GetGLFWwindow();
};
