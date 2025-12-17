#pragma once
#include "petri_net.h"
#include "petri_state.h"
#include <vector>
#include <string>

struct RGNode
{
    int id;
    Marking marking;
};

struct RGEdge
{
    int from;
    int to;
    std::string transition;
};

class ReachabilityGraph
{
public:
    void Build(const PetriNet& net, const Marking& initial);
    void SaveToFile(const std::string& filename) const;

    const std::vector<RGNode>& GetNodes() const;
    const std::vector<RGEdge>& GetEdges() const;

private:
    int GetOrCreateNode(const Marking& m);

    std::vector<RGNode> nodes;
    std::vector<RGEdge> edges;
};
