#include "PetriPlace.h"
#include "PetriToken.h"     // обязательно!
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"
#include <vector>
#include <cmath>

PetriPlace::PetriPlace(glm::vec3 pos, int initialTokens)
    : position(pos)
{
    // создаём начальные токены
    for (int i = 0; i < initialTokens; ++i)
    {
        tokens.push_back(new PetriToken(position));
    }

    setupCircle();
}

PetriPlace::~PetriPlace()
{
    // удаляем все токены, которыми владеем
    for (PetriToken* token : tokens)
        delete token;

    delete VAOcircle;
    delete VBOcircle;
}

void PetriPlace::setupCircle()
{
    const int segments = 32;
    std::vector<float> vertices;
    float radius = 0.25f;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * 3.1415926535f * i / segments;
        vertices.push_back(radius * cosf(angle));
        vertices.push_back(0.0f);
        vertices.push_back(radius * sinf(angle));
    }

    VAOcircle = new VAO();
    VBOcircle = new VBO(vertices.data(), vertices.size() * sizeof(float));

    VAOcircle->Bind();
    VAOcircle->LinkAttrib(*VBOcircle, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
    VAOcircle->Unbind();
}

void PetriPlace::draw(Shader& shader)
{
    // рисуем круг места
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(0.3f, 0.7f, 1.0f));  // голубой

    VAOcircle->Bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 34);
    VAOcircle->Unbind();

    // рисуем все токены внутри
    for (PetriToken* token : tokens)
        token->draw(shader);
}

PetriToken* PetriPlace::takeToken()
{
    if (tokens.empty())
        return nullptr;

    PetriToken* token = tokens.back();
    tokens.pop_back();
    return token;  // теперь владение переходит наружу (в дугу)
}