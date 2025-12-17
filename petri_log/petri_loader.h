#pragma once
#include "petri_net.h"
#include "petri_state.h"
#include <string>

struct LoadedPetri
{
    PetriNet net;
    PetriState state;
};

class PetriLoader
{
public:
    static LoadedPetri Load(const std::string& filename);
};
