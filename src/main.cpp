#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>

#include "Window.h"
#include "shaderClass.h"
#include "Camera.h"

#include "Sphere.h"
#include "Grid.h"
#include "Scene.h"
#include "ContainerSphere.h"   // ВАЖНО

const unsigned int width  = 1280;
const unsigned int height = 800;

int main()
{
    srand((unsigned int)time(nullptr));

    try
    {
        Window win(width, height, "Sphere Container Test");

        Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

        Camera camera(
            width,
            height,
            glm::vec3(0.0f, 10.0f, 18.0f)   // позиция камеры
        );
        camera.Orientation = glm::normalize(
            glm::vec3(0.0f, -0.4f, -1.0f)
        );

        Scene scene;
        // сетка (ориентир)
        scene.SetGrid(Grid::Create(500.0f, 100));

        auto* container = new ContainerSphere(
            6.0f,                            // радиус
            glm::vec3(0.0f, 6.0f, 0.0f),      // центр
            glm::vec3(0.2f, 0.8f, 1.0f)       // цвет
        );
        container->SetUseWorldBounds(false);
        container->SetMass(0.0f);

        container->SetAlpha(0.25f);          // прозрачность
        scene.AddShape(container);

        // МАЛЕНЬКИЕ ШАРЫ ВНУТРИ

        const int sphereCount = 1;
        const float innerR = 6.0f - 0.6f;    // контейнер - радиус шара

        for (int i = 0; i < sphereCount; i++)
        {
            float rx = ((rand() % 200) / 100.0f - 1.0f) * innerR * 0.5f;
            float ry = ((rand() % 200) / 100.0f - 1.0f) * innerR * 0.5f;
            float rz = ((rand() % 200) / 100.0f - 1.0f) * innerR * 0.5f;

            auto* s = Sphere::Create(
                5.7f,
                rx,
                6.0f + ry,
                rz,
                rand() % 256,
                rand() % 256,
                rand() % 256
            );

            s->SetMass(0.1f);
            s->SetAlpha(1.0f);
            s->SetUseWorldBounds(false);

            s->SetVelocity(glm::vec3(
                (rand() % 200 - 100) * 0.04f,
                (rand() % 200 - 100) * 0.04f,
                (rand() % 200 - 100) * 0.04f
            ));

            scene.AddShape(s);
        }

        while (!win.ShouldClose())
        {
            camera.Inputs(win.window);

            scene.Update();

            glClearColor(0.05f, 0.06f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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