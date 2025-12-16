#pragma once

#include <vector>
#include "Shape.h"
#include "Grid.h"
#include "Camera.h"
#include "shaderClass.h"
#include "TimeGL.h"
#include "ContainerSphere.h"

class Scene
{
public:
    Scene();
    ~Scene();

    // управление объектами
    void AddShape(Shape* shape);
    void RemoveShape(Shape* shape);

    // grid
    void SetGrid(Grid* g);

    // обновление
    void Update();                // использует внутренний таймер
    void Update(float deltaTime); // если хочешь управлять dt вручную

    // рендер
    void Draw(Shader& shader, Camera& camera);
    void ResolveSphereInsideContainer(ContainerSphere* container, const std::vector<Sphere*>& spheres);
    // таймер
    float GetTotalTime() const;
    float GetDeltaTime() const;

private:
    std::vector<Shape*> objects;
    Grid* grid = nullptr;
    TimeGL timer;
};
