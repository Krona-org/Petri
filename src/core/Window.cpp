#include "Window.h"
#include <iostream>
#include <stdexcept>

Window::Window(int w, int h, const std::string& title)
{
    width = w;
    height = h;

    if (!glfwInit())
        throw std::runtime_error("GLFW init failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Window creation failed");
    }

    glfwMakeContextCurrent(window);

    // VSync по умолчанию (можно выключить позже через glfwSwapInterval(0))
    glfwSwapInterval(1);

    // Чтобы коллбэки могли менять поля Window
    glfwSetWindowUserPointer(window, this);

    // Коллбэк ресайза по умолчанию: обновляем viewport и размеры окна
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h)
    {
        if (auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win)))
        {
            self->width = w;
            self->height = h;
        }
        glViewport(0, 0, w, h);
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("GLAD init failed");
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SetFramebufferSizeCallback(GLFWframebuffersizefun callback)
{
    glfwSetFramebufferSizeCallback(window, callback);
}
