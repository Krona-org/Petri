#pragma once
#include <vector>
#include "Shape.h"
#include "shaderClass.h"
#include "Camera.h"
#include "TimeGL.h"
#include "Grid.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void AddShape(Shape* shape);          // добавить объект
    void SetGrid(Grid* grid);             // добавить сетку
    void Update();                        // обновление сцены
    void Draw(Shader& shader, Camera& camera); // отрисовка сцены

private:
    std::vector<Shape*> objects;
    Grid* grid;
    TimeGL timer;
};
