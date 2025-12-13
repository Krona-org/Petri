#include "Scene.h"
#include <algorithm>

Scene::Scene() : grid(nullptr) {}

Scene::~Scene() {
    for (auto obj : objects)
        delete obj;
    if (grid)
        delete grid;
}

void Scene::AddShape(Shape* shape) {
    objects.push_back(shape);
}

void Scene::RemoveShape(Shape* shape) {
    auto it = std::find(objects.begin(), objects.end(), shape);
    if (it != objects.end()) {
        delete *it;
        objects.erase(it);
    }
}

void Scene::SetGrid(Grid* g) {
    grid = g;
}

void Scene::Update() {
    timer.Update();
    float dt = timer.GetDeltaTime();
    float t  = timer.GetTotalTime();

    for (auto obj : objects)
        obj->Update(t, dt);
}

void Scene::Update(float deltaTime) {
    float t = timer.GetTotalTime() + deltaTime;

    for (auto obj : objects)
        obj->Update(t, deltaTime);
}

void Scene::Draw(Shader& shader, Camera& camera) {
    glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)camera.width / camera.height, 0.1f, 1000.0f);

    if (grid)
        grid->Draw(shader, view, projection, timer.GetTotalTime());

    for (auto obj : objects)
        obj->Draw(shader, view, projection, timer.GetTotalTime());
}

float Scene::GetTotalTime() const { return timer.GetTotalTime(); }
float Scene::GetDeltaTime() const { return timer.GetDeltaTime(); }
