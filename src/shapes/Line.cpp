#include "Line.h"
#include <vector>

Line::Line(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color)
    : startPoint(start), endPoint(end), lineColor(color)
{
    std::vector<float> vertices = {
        startPoint.x, startPoint.y, startPoint.z, lineColor.r, lineColor.g, lineColor.b,
        endPoint.x, endPoint.y, endPoint.z, lineColor.r, lineColor.g, lineColor.b
    };

    vao.Bind();
    vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));
    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.Unbind();
}

void Line::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time)
{
    shader.Activate();
    vao.Bind();
    glDrawArrays(GL_LINES, 0, 2);
    vao.Unbind();
}

void Line::SetPosition(const glm::vec3& pos)
{
    // смещаем обе точки на pos
    startPoint += pos;
    endPoint += pos;
    // нужно обновить VBO, если хотим динамическое перемещение
}

void Line::SetColor(const glm::vec3& col)
{
    lineColor = col;
    // для обновления VBO можно перезаписать данные вершин
}
