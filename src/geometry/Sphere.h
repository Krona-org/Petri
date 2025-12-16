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
    ~Sphere() override;

    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time) override;
    void Update(float totalTime, float deltaTime) override;
    bool CheckHover(const glm::vec3& rayOrigin, const glm::vec3& rayDir) override;

    // Геометрия
    void SetRadius(float r) { radius = r; }
    float GetRadius() const { return radius; }

    // Физика
    void SetVelocity(const glm::vec3& v) { velocity = v; }
    glm::vec3 GetVelocity() const { return velocity; }
    glm::vec3& VelocityRef() { return velocity; }

    void SetGravity(const glm::vec3& g) { acceleration = g; }         // например (0, -9.81, 0)
    glm::vec3 GetGravity() const { return acceleration; }

    void SetRestitution(float e) { restitution = e; }                 // 0..1
    float GetRestitution() const { return restitution; }

    void SetMass(float m)
    {
        mass = (m > 0.0f) ? m : 0.0f;
        invMass = (mass > 0.0f) ? 1.0f / mass : 0.0f;                 // 0 -> "неподвижный"
    }
    float GetMass() const { return mass; }
    float GetInvMass() const { return invMass; }

    bool useWorldBounds = true;

    void SetUseWorldBounds(bool v) { useWorldBounds = v; }


    // Фабрика
    static Sphere* Create(float radius, float x, float y, float z,
                          int r = 255, int g = 255, int b = 255);

private:
    void generateMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices);

    float radius;

    // GPU
    VAO vao;
    VBO* vbo = nullptr;
    EBO* ebo = nullptr;
    unsigned int indexCount = 0;

    // Physics state
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    float restitution = 0.85f;

    float mass = 1.0f;
    float invMass = 1.0f;

    // Простые границы мира (чтобы не улетали)
    float floorY = 0.0f;
    float minX = -200.0f, maxX = 200.0f;
    float minZ = -200.0f, maxZ = 200.0f;
};

#endif
