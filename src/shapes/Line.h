#ifndef LINE_H
#define LINE_H

#include "Shape.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <vector>

class Line : public Shape {
public:
    // Конструктор приватный, чтобы заставить использовать фабрику
    Line(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);

    static Line* Create(float x1, float y1, float z1,
                        float x2, float y2, float z2,
                        int r = 255, int g = 255, int b = 255)
    {
        return new Line(
            glm::vec3(x1, y1, z1),
            glm::vec3(x2, y2, z2),
            glm::vec3(r/255.0f, g/255.0f, b/255.0f)
        );
    }
    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time) override;
    void SetPosition(const glm::vec3& pos) override;
    void MoveBy(const glm::vec3& delta); // смещение на delta
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
