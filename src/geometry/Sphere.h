#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <vector>

class Sphere : public Shape {
public:
    Sphere(float radius, const glm::vec3& pos, const glm::vec3& col);
    ~Sphere();

    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time) override;
    bool CheckHover(const glm::vec3& rayOrigin, const glm::vec3& rayDir) override;

    void SetRadius(float r) { radius = r; }
    float GetRadius() const { return radius; }

        // Фабрика с привычными int-значениями
    static Sphere* Create(float radius, float x, float y, float z,
                          int r = 255, int g = 255, int b = 255);

private:
    void generateMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices);

    float radius;
    VAO vao;
    VBO* vbo = nullptr;
    EBO* ebo = nullptr;
    unsigned int indexCount = 0;
};

#endif
