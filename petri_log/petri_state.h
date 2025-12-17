#pragma once
#include "petri_net.h"
#include <vector>

using Marking = std::vector<int>;

class PetriState
{
public:
    explicit PetriState(const PetriNet& net);

    // доступ к разметке
    const Marking& GetMarking() const;
    void SetMarking(const Marking& m);

    // внешнее воздействие (ImGui)
    void AddTokens(PlaceID place, int count);

    // логика
    bool IsEnabled(TransitionID t) const;
    bool Fire(TransitionID t);

    // шаг симуляции
    bool Step();          // один переход
    void RunUntilStable(); // пока есть разрешённые

private:
    const PetriNet& net;
    Marking marking;
};
