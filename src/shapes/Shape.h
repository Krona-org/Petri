#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include "shaderClass.h"

class Shape {
public:
    Shape() : position(0.0f), color(1.0f), selected(false) {}
    virtual ~Shape() {}

    virtual void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time) = 0;
    virtual void Update(float deltaTime) {}
    virtual bool CheckHover(const glm::vec3& rayOrigin, const glm::vec3& rayDir) = 0;

    virtual void SetPosition(const glm::vec3& pos) { position = pos; }
    virtual glm::vec3 GetPosition() const { return position; }

    virtual void SetColor(const glm::vec3& col) { color = col; }
    virtual glm::vec3 GetColor() const { return color; }

protected:
    glm::vec3 position;
    glm::vec3 color;
    bool selected;
};

#endif
