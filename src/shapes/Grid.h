#ifndef GRID_H
#define GRID_H

#include "Shape.h"
#include "Line.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <vector>

class Grid : public Shape {
public:
    Grid(float size, int divisions, const glm::vec3& color);

    void Draw(Shader& shader);
    void Update(float deltaTime) override {}
    void SetPosition(const glm::vec3& pos) override;
    void SetColor(const glm::vec3& col) override;
    bool CheckHover(const glm::vec3& rayOrigin, const glm::vec3& rayDir) override { return false; }

private:
    std::vector<Line*> lines;
    float gridSize;
    int gridDivisions;
    glm::vec3 gridColor;
};

#endif
