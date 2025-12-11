#pragma once
#include <glm/glm.hpp>

class VAO;
class VBO;
class Shader;

class PetriToken
{
public:
    explicit PetriToken(glm::vec3 pos);
    ~PetriToken();                          // деструктор будет в .cpp

    void setPosition(const glm::vec3& p) { position = p; }
    void draw(Shader& shader);

private:
    glm::vec3 position;

    VAO* VAOcircle = nullptr;   // raw-указатели — никаких unique_ptr в заголовке
    VBO* VBOcircle = nullptr;

    void setup();
};