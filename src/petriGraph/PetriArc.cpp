#include "PetriArc.h"
#include "VAO.h"
#include "VBO.h"
#include "PetriToken.h"     // ← ЭТО ОБЯЗАТЕЛЬНО! Здесь тип уже полный
#include "shaderClass.h"

PetriArc::PetriArc(glm::vec3 start, glm::vec3 end)
    : startPos(start), endPos(end)
{
    float vertices[] = {
        startPos.x, startPos.y, startPos.z,
        endPos.x,   endPos.y,   endPos.z
    };

    VAOline = new VAO();
    VBOline = new VBO(vertices, sizeof(vertices));

    VAOline->Bind();
    VAOline->LinkAttrib(*VBOline, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
    VAOline->Unbind();
}

PetriArc::~PetriArc()
{
    delete VAOline;
    delete VBOline;
    // movingToken принадлежит PetriPlace — мы его НЕ удаляем!
}

void PetriArc::setToken(PetriToken* token)
{
    movingToken = token;
    t = 0.0f;
}

void PetriArc::update(float dt)
{
    if (!movingToken) return;

    t += dt * speed;

    if (t >= 1.0f)
    {
        movingToken->setPosition(endPos);
        movingToken = nullptr;
        t = 0.0f;
    }
    else
    {
        glm::vec3 pos = glm::mix(startPos, endPos, t);
        movingToken->setPosition(pos);
    }
}

void PetriArc::draw(Shader& shader)
{
    glm::mat4 model(1.0f);
    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(0.7f, 0.7f, 0.7f));  // серый

    VAOline->Bind();
    glDrawArrays(GL_LINES, 0, 2);
    VAOline->Unbind();

    if (movingToken)
        movingToken->draw(shader);
}