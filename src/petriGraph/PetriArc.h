#pragma once
#include <glm/glm.hpp>

class PetriToken;   // forward declaration — достаточно
class VAO;
class VBO;
class Shader;

class PetriArc
{
public:
    PetriArc(glm::vec3 start, glm::vec3 end);
    ~PetriArc();                           // деструктор будет в .cpp

    void setToken(PetriToken* token);     // raw pointer — безопасно
    void update(float dt);
    void draw(Shader& shader);

    glm::vec3 getStartPos() const { return startPos; }
    glm::vec3 getEndPos()   const { return endPos;   }

private:
    glm::vec3 startPos;
    glm::vec3 endPos;

    VAO* VAOline = nullptr;   // raw указатели
    VBO* VBOline = nullptr;

    PetriToken* movingToken = nullptr;
    float t = 0.0f;
    float speed = 1.8f;
};