#include "Grid.h"
#include <glm/glm.hpp>

Grid::Grid(float size, int divisions, const glm::vec3& color)
    : gridSize(size), gridDivisions(divisions), gridColor(color)
{
    float step = gridSize / divisions;
    float half = gridSize / 2.0f;

    // линии по X
    for (int i = 0; i <= divisions; ++i)
    {
        float pos = -half + i * step;
        lines.push_back(new Line(glm::vec3(pos, 0, -half), glm::vec3(pos, 0, half), gridColor));
    }

    // линии по Z
    for (int i = 0; i <= divisions; ++i)
    {
        float pos = -half + i * step;
        lines.push_back(new Line(glm::vec3(-half, 0, pos), glm::vec3(half, 0, pos), gridColor));
    }
}

void Grid::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time)
{
    for (auto l : lines)
        l->Draw(shader, NULL, NULL, NULL);
}

void Grid::SetPosition(const glm::vec3& pos)
{
    for (auto l : lines)
        l->SetPosition(pos);
}

void Grid::SetColor(const glm::vec3& col)
{
    for (auto l : lines)
        l->SetColor(col);
}
