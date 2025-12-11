#include "PetriGraph.h"
#include "PetriPlace.h"
#include "PetriTransition.h"
#include "PetriArc.h"
#include "PetriToken.h"     // ← обязательно!
#include <algorithm>
#include <cmath>

PetriGraph::~PetriGraph()
{
    // удаляем всё, чем владели
    for (PetriPlace* p : places)        delete p;
    for (PetriTransition* t : transitions) delete t;
    for (PetriArc* a : arcs)            delete a;
}

void PetriGraph::addPlace(const glm::vec3& pos, int tokens)
{
    places.push_back(new PetriPlace(pos, tokens));
}

void PetriGraph::addTransition(const glm::vec3& pos)
{
    transitions.push_back(new PetriTransition(pos));
}

void PetriGraph::addArc(int placeIdx, int transIdx, ArcType type)
{
    glm::vec3 from = places[placeIdx]->getPosition();
    glm::vec3 to   = transitions[transIdx]->getPosition();

    if (type == ArcType::Output)
        std::swap(from, to);   // от перехода к месту

    arcs.push_back(new PetriArc(from, to));
}

void PetriGraph::update(float dt)
{
    for (PetriArc* arc : arcs)
        arc->update(dt);
}

void PetriGraph::draw(Shader& shader)
{
    for (PetriPlace* p : places)        p->draw(shader);
    for (PetriTransition* t : transitions) t->draw(shader);
    for (PetriArc* a : arcs)            a->draw(shader);
}

void PetriGraph::fireTransition(int transIdx)
{
    // ищем входную дугу (та, что заканчивается в этом переходе)
    for (PetriArc* arc : arcs)
    {
        if (glm::distance(arc->getEndPos(), transitions[transIdx]->getPosition()) < 0.1f)
        {
            // ищем место, из которого идёт эта дуга
            for (PetriPlace* place : places)
            {
                if (glm::distance(place->getPosition(), arc->getStartPos()) < 0.1f)
                {
                    PetriToken* token = place->takeToken();
                    if (token != nullptr)
                    {
                        arc->setToken(token);
                        return; // только один токен за раз
                    }
                }
            }
        }
    }
}