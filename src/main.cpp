#include <iostream>
#include <vector>
#include <cmath>
#include <exception>

#include "Window.h"       // новый класс окна
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "TimeGL.h"

#include "Sphere.h"
#include "Line.h"
#include "Grid.h"

const unsigned int width = 1920;
const unsigned int height = 1080;

glm::vec3 getRayFromMouse(double mouseX, double mouseY, int width, int height,
                          const glm::mat4& projection, const glm::mat4& view)
{
    float x = (2.0f * mouseX) / width - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / height;
    float z = 1.0f;

    glm::vec3 ray_nds(x, y, z);
    glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
    return glm::normalize(ray_wor);
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

int main()
{
    try
    {
        Window win(width, height, "Animated Sphere");  // создаем окно
        win.SetFramebufferSizeCallback(framebuffer_size_callback);

        // Шейдер и камера
        Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
        Camera camera(width, height, glm::vec3(0.0f, 1.0f, 3.0f));

        // Таймер
        TimeGL timer;

        // Создаем сферы
        std::vector<Shape*> spheres;
        spheres.push_back(Sphere::Create(1.0f, 0, 2, 0, 125, 0, 0));
        spheres.push_back(Sphere::Create(1.0f, 3, 2, 1, 125, 0, 0));

        while (!win.ShouldClose())
        {
            timer.Update();
            float dt = timer.GetDeltaTime();
            float t  = timer.GetTotalTime();

            glClearColor(
                0.1f + 0.05f * sin(t),
                0.1f + 0.05f * cos(t),
                0.2f + 0.05f * sin(t * 1.3f),
                1.0f
            );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.Inputs(win.window);

            glm::mat4 view = glm::lookAt(
                camera.Position,
                camera.Position + camera.Orientation,
                camera.Up
            );

            glm::mat4 projection = glm::perspective(
                glm::radians(45.0f),
                (float)width / height,
                0.1f,
                100.0f
            );

            double mouseX, mouseY;
            glfwGetCursorPos(win.window, &mouseX, &mouseY);
            glm::vec3 rayDir = getRayFromMouse(mouseX, mouseY, width, height, projection, view);

            for (auto s : spheres)
            {
                s->Draw(shaderProgram, view, projection, t);
            }

            win.SwapBuffers();
            win.PollEvents();
        }

        for (auto s : spheres) delete s;
        shaderProgram.Delete();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}