#ifndef GRID_H
#define GRID_H

#include "Shape.h"
#include "Line.h"
#include <vector>
#include <glm/glm.hpp>

class Grid : public Shape {
public:
    Grid(float size, int divisions, const glm::vec3& color);

    static Grid* Create(float size, int divisions, int r = 200, int g = 200, int b = 200)
    {
        return new Grid(size, divisions, glm::vec3(r/255.0f, g/255.0f, b/255.0f));
    }

    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time) override;
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
