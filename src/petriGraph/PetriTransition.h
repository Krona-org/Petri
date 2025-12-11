#pragma once
#include <glm/glm.hpp>

class VAO;
class VBO;
class Shader;

class PetriTransition
{
public:
    PetriTransition(glm::vec3 pos);
    ~PetriTransition();                     // деструктор в .cpp

    void draw(Shader& shader);

    const glm::vec3& getPosition() const { return position; }

private:
    glm::vec3 position;

    VAO* VAOrect = nullptr;
    VBO* VBOrect = nullptr;

    void setupRectangle();
};