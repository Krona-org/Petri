#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class PetriPlace;
class PetriTransition;
class PetriArc;
class Shader;

enum class ArcType { Input, Output };

class PetriGraph
{
public:
    PetriGraph() = default;
    ~PetriGraph();                         // деструктор в .cpp

    void addPlace(const glm::vec3& pos, int tokens = 0);
    void addTransition(const glm::vec3& pos);
    void addArc(int placeIdx, int transIdx, ArcType type);

    void update(float dt);
    void draw(Shader& shader);

    // Запуск перехода по индексу (вызывается по пробелу)
    void fireTransition(int transitionIndex);

private:
    std::vector<PetriPlace*>      places;
    std::vector<PetriTransition*> transitions;
    std::vector<PetriArc*>        arcs;
};