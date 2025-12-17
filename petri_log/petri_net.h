#pragma once
#include <vector>
#include <string>

using PlaceID = int;
using TransitionID = int;

struct Arc
{
    PlaceID place;
    int weight;
};

struct Place
{
    std::string name;
};

struct Transition
{
    std::string name;
    std::vector<Arc> input;
    std::vector<Arc> output;
};

class PetriNet
{
public:
    int PlaceCount() const { return (int)places.size(); }
    int TransitionCount() const { return (int)transitions.size(); }

    const Place& GetPlace(PlaceID id) const { return places[id]; }
    const Transition& GetTransition(TransitionID id) const { return transitions[id]; }

private:
    friend class PetriLoader;

    std::vector<Place> places;
    std::vector<Transition> transitions;
};
