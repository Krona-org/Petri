#include <iostream>
#include <vector>
#include <cmath>
#include <exception>

#include "Window.h"       // заменили прямое использование GLFWwindow* на наш класс Window
#include "shaderClass.h"
#include "Camera.h"
#include "TimeGL.h"
#include "Scene.h"

#include "Sphere.h"
#include "Line.h"
#include "Grid.h"

const unsigned int width = 1920;
const unsigned int height = 1080;

// генерация луча из позиции мыши (без изменений)
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
// теперь используем Window, а не GLFW напрямую
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

int main()
{
    try
    {
        // создаём окно
        Window win(width, height, "Animated Sphere");

        // callback для изменения размеров окна
        win.SetFramebufferSizeCallback([](GLFWwindow* w, int newW, int newH)
        {
            glViewport(0, 0, newW, newH);
        });

        Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
        Camera camera(width, height, glm::vec3(0.0f, 1.0f, 3.0f));

        // создаём сцену и добавляем объекты
        Scene scene;
        scene.AddShape(Sphere::Create(1.0f, 0, 2, 0, 125, 0, 0));
        scene.AddShape(Sphere::Create(1.0f, 3, 2, 1, 125, 0, 0));
        scene.SetGrid(Grid::Create(500.0f, 100));
        scene.AddShape(Sphere::Create(0.5, 2, 2, 0, 0, 255, 0));
        // главный цикл
        while (!win.ShouldClose())
        {
            // обновляем всю сцену (внутри таймер)
            scene.Update();

            glClearColor(
                0.1f + 0.05f * sin(scene.GetTotalTime()),
                0.1f + 0.05f * cos(scene.GetTotalTime()),
                0.2f + 0.05f * sin(scene.GetTotalTime() * 1.3f),
                1.0f
            );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.Inputs(win.window); // управление камерой

            // отрисовка сцены
            scene.Draw(shaderProgram, camera);

            win.SwapBuffers();
            win.PollEvents();
        }

        shaderProgram.Delete();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}