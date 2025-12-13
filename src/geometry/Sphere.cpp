#include "Sphere.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

Sphere::Sphere(float rad, const glm::vec3& pos, const glm::vec3& col)
    : radius(rad)
{
    position = pos;
    color = col;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    generateMesh(vertices, indices);
    indexCount = static_cast<unsigned int>(indices.size());

    vao.Bind();
    vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));
    ebo = new EBO(indices.data(), indices.size() * sizeof(unsigned int));

    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);                     // позиция
    vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));   // цвет (пока оставляем)
    vao.LinkAttrib(*vbo, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));   // нормаль
    vao.Unbind();
}

Sphere::~Sphere()
{
    if (vbo) { vbo->Delete(); delete vbo; vbo = nullptr; }
    if (ebo) { ebo->Delete(); delete ebo; ebo = nullptr; }
    vao.Delete();
}

void Sphere::generateMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    const int sectorCount = 36;
    const int stackCount = 18;
    const float PI = glm::pi<float>();

    for (int i = 0; i <= stackCount; ++i)
    {
        float stackAngle = PI / 2.0f - i * PI / stackCount;
        float xy = radius * cosf(stackAngle);
        float yCoord = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * 2.0f * PI / sectorCount;

            float xCoord = xy * cosf(sectorAngle);
            float zCoord = xy * sinf(sectorAngle);

            // Позиция
            vertices.push_back(xCoord);
            vertices.push_back(yCoord);
            vertices.push_back(zCoord);

            // Цвет (можно потом убрать и оставить только objectColor)
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);

            // Нормаль
            glm::vec3 normal = glm::normalize(glm::vec3(xCoord, yCoord, zCoord));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }

    for (int i = 0; i < stackCount; ++i)
    {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time)
{
    shader.Activate();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::rotate(model, glm::radians(time * 30.0f), glm::vec3(0.2f, 1.0f, 0.0f));

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // время для шейдера
    shader.setFloat("time", time);

    // цвет сферы задаём единым униформом (и поддерживаем подсветку selected)
    glm::vec3 renderColor = selected ? glm::vec3(1.0f, 1.0f, 0.3f) : color;
    shader.setBool("useObjectColor", true);
    shader.setVec3("objectColor", renderColor);

    vao.Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

bool Sphere::CheckHover(const glm::vec3& rayOrigin, const glm::vec3& rayDir)
{
    glm::vec3 center = position;
    glm::vec3 oc = rayOrigin - center;

    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(oc, rayDir);
    float c = glm::dot(oc, oc) - radius * radius;
    float disc = b * b - 4.0f * a * c;

    if (disc < 0.0f) return false;

    float sqrtDisc = glm::sqrt(disc);
    float t1 = (-b - sqrtDisc) / (2.0f * a);
    float t2 = (-b + sqrtDisc) / (2.0f * a);

    selected = t1 > 0.0f || t2 > 0.0f;
    return selected;
}

Sphere* Sphere::Create(float radius, float x, float y, float z, int r, int g, int b)
{
    glm::vec3 pos(x, y, z);
    glm::vec3 col(r / 255.0f, g / 255.0f, b / 255.0f);
    return new Sphere(radius, pos, col);
}
