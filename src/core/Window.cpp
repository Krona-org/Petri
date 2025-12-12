#include "Window.h"
#include <glad/glad.h>
#include <stdexcept>

Window::Window(int w, int h, const std::string& t)
    : width(w), height(h), title(t), window(nullptr) {}

Window::~Window()
{
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::Init()
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void Window::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SwapBuffers() { glfwSwapBuffers(window); }
void Window::PollEvents()  { glfwPollEvents(); }
bool Window::ShouldClose() { return glfwWindowShouldClose(window); }
GLFWwindow* Window::GetGLFWwindow() { return window; }
