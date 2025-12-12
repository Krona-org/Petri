#pragma once
#include <vector>
#include "Shape.h"
#include "Grid.h"
#include "Camera.h"
#include "shaderClass.h"
#include "TimeGL.h"

class Scene {
public:
    Scene();
    ~Scene();

    // Управление объектами
    void AddShape(Shape* shape);
    void RemoveShape(Shape* shape);

    // Grid
    void SetGrid(Grid* g);

    // Обновление сцены
    void Update();                    // использует таймер внутри
    void Update(float deltaTime);     // перегрузка: можно передать deltaTime извне

    // Отрисовка
    void Draw(Shader& shader, Camera& camera);

    // Доступ к таймеру
    float GetTotalTime() const;
    float GetDeltaTime() const;

private:
    std::vector<Shape*> objects;
    Grid* grid;
    TimeGL timer;
};
