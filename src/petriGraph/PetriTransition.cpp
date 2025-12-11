#include "PetriTransition.h"
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"

PetriTransition::PetriTransition(glm::vec3 pos) : position(pos)
{
    setupRectangle();
}

PetriTransition::~PetriTransition()
{
    delete VAOrect;
    delete VBOrect;
}

void PetriTransition::setupRectangle()
{
    float w = 0.15f;  // половина ширины
    float h = 0.40f;  // половина высоты

    float vertices[] = {
        -w, 0.0f, -h,
         w, 0.0f, -h,
         w, 0.0f,  h,
        -w, 0.0f,  h
    };

    VAOrect = new VAO();
    VBOrect = new VBO(vertices, sizeof(vertices));

    VAOrect->Bind();
    VAOrect->LinkAttrib(*VBOrect, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
    VAOrect->Unbind();
}

void PetriTransition::draw(Shader& shader)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(1.0f, 0.5f, 0.2f));  // оранжевый

    VAOrect->Bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    VAOrect->Unbind();
}