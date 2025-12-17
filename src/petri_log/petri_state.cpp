#include "petri_state.h"
#include <stdexcept>

PetriState::PetriState(const PetriNet& n)
    : net(n), marking(n.PlaceCount(), 0)
{}

const Marking& PetriState::GetMarking() const
{
    return marking;
}

void PetriState::SetMarking(const Marking& m)
{
    if (m.size() != marking.size())
        throw std::runtime_error("Invalid marking size");
    marking = m;
}

void PetriState::AddTokens(PlaceID place, int count)
{
    marking[place] += count;
}

bool PetriState::IsEnabled(TransitionID t) const
{
    const auto& tr = net.GetTransition(t);
    for (const auto& arc : tr.input)
    {
        if (marking[arc.place] < arc.weight)
            return false;
    }
    return true;
}

bool PetriState::Fire(TransitionID t)
{
    if (!IsEnabled(t))
        return false;

    const auto& tr = net.GetTransition(t);

    for (const auto& arc : tr.input)
        marking[arc.place] -= arc.weight;

    for (const auto& arc : tr.output)
        marking[arc.place] += arc.weight;

    return true;
}

bool PetriState::Step()
{
    for (int t = 0; t < net.TransitionCount(); ++t)
    {
        if (IsEnabled(t))
        {
            Fire(t);
            return true;
        }
    }
    return false;
}

void PetriState::RunUntilStable()
{
    while (Step()) {}
}
