#include "PetriToken.h"
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"
#include <vector>
#include <cmath>

PetriToken::PetriToken(glm::vec3 pos) : position(pos)
{
    setup();
}

PetriToken::~PetriToken()
{
    delete VAOcircle;
    delete VBOcircle;
}

void PetriToken::setup()
{
    const int segments = 20;
    std::vector<float> vertices;
    float radius = 0.06f;

    // центр
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // ободок
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * 3.1415926535f * i / segments;
        vertices.push_back(radius * cosf(angle));
        vertices.push_back(0.01f);  // чуть выше, чтобы не было z-fighting
        vertices.push_back(radius * sinf(angle));
    }

    VAOcircle = new VAO();
    VBOcircle = new VBO(vertices.data(), vertices.size() * sizeof(float));

    VAOcircle->Bind();
    VAOcircle->LinkAttrib(*VBOcircle, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
    VAOcircle->Unbind();
}

void PetriToken::draw(Shader& shader)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(1.0f, 0.9f, 0.0f));  // ярко-жёлтый

    VAOcircle->Bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 22);  // 1 центр + 21 точка
    VAOcircle->Unbind();
}