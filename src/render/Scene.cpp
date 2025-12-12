#include "Scene.h"

Scene::Scene()
    : grid(nullptr)
{
}

Scene::~Scene()
{
    for (auto obj : objects)
        delete obj;
    if (grid)
        delete grid;
}

void Scene::AddShape(Shape* shape)
{
    objects.push_back(shape);
}

void Scene::SetGrid(Grid* g)
{
    grid = g;
}

void Scene::Update()
{
    timer.Update();
    float dt = timer.GetDeltaTime();
    float t  = timer.GetTotalTime();

    // Можно тут пробегать по объектам и обновлять их анимацию, если нужно
    for (auto obj : objects)
    {
        obj->Update(t, dt); // предполагаем, что Shape имеет метод Update
    }
}

void Scene::Draw(Shader& shader, Camera& camera)
{
    glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)camera.width / camera.height, 0.1f, 100.0f);

    if (grid)
        grid->Draw(shader, view, projection, timer.GetTotalTime());

    for (auto obj : objects)
        obj->Draw(shader, view, projection, timer.GetTotalTime());
}
