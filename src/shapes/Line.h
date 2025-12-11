#ifndef LINE_H
#define LINE_H

#include "Shape.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <vector>

class Line : public Shape {
public:
    Line(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);

    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time) override;
    void Update(float deltaTime) override {}
    void SetPosition(const glm::vec3& pos) override;
    void SetColor(const glm::vec3& col) override;
    bool CheckHover(const glm::vec3& rayOrigin, const glm::vec3& rayDir) override { return false; }

private:
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    glm::vec3 lineColor;

    VAO vao;
    VBO* vbo = nullptr;
};

#endif
