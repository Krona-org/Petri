#include <iostream>
#include <exception>

#include "Window.h"
#include "shaderClass.h"
#include "Camera.h"
#include "TimeGL.h"

#include "Sphere.h"
#include "Line.h"
#include "Grid.h"
#include "Scene.h"  // новый класс сцены

const unsigned int width = 1920;
const unsigned int height = 1080;

int main()
{
    try
    {
        // Создаём окно через наш класс Window
        Window win(width, height, "Animated Sphere");

        // Шейдер
        Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

        // Камера
        Camera camera(width, height, glm::vec3(0.0f, 1.0f, 3.0f));

        // Сцена
        Scene scene;

        // Добавляем объекты в сцену
        scene.AddShape(Sphere::Create(1.0f, 0, 2, 0, 125, 0, 0));
        scene.AddShape(Sphere::Create(1.0f, 3, 2, 1, 125, 0, 0));
        scene.SetGrid(Grid::Create(500.0f, 100));  // сетка

        // Главный цикл
        while (!win.ShouldClose())
        {
            // Обновляем камеру (обработка WASD и мыши)
            camera.Inputs(win.window);

            // Обновляем сцену (таймер, анимации объектов)
            scene.Update();

            // Очистка экрана с плавным изменением цвета по времени
            float t = scene.GetTotalTime();  // можно добавить метод GetTime() в Scene, чтобы возвращать totalTime
            glClearColor(
                0.1f + 0.05f * sin(t),
                0.1f + 0.05f * cos(t),
                0.2f + 0.05f * sin(t * 1.3f),
                1.0f
            );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Рендер сцены с текущей камерой
            scene.Draw(shaderProgram, camera);

            win.SwapBuffers(); // обмен буферов
            win.PollEvents();  // обработка событий
        }

        // Очистка — Scene удаляет свои объекты
        shaderProgram.Delete();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
