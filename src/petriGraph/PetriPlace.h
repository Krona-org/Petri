#pragma once
#include <glm/glm.hpp>
#include <vector>

class PetriToken;
class VAO;
class VBO;
class Shader;

class PetriPlace
{
public:
    PetriPlace(glm::vec3 pos, int initialTokens = 0);
    ~PetriPlace();                         // деструктор в .cpp

    void draw(Shader& shader);

    // Для PetriGraph — чтобы забрать токен
    PetriToken* takeToken();               // возвращает raw pointer и удаляет из вектора

    const glm::vec3& getPosition() const { return position; }

private:
    glm::vec3 position;
    std::vector<PetriToken*> tokens;       // raw указатели — владение здесь!

    VAO* VAOcircle = nullptr;
    VBO* VBOcircle = nullptr;

    void setupCircle();
};