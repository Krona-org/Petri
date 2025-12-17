#pragma once
#include <vector>
#include <string>

struct RGNode
{
    int id;
    std::vector<int> marking;
};

struct RGEdge
{
    int from;
    int to;
    std::string transition;
};

struct ReachabilityGraphData
{
    std::vector<RGNode> nodes;
    std::vector<RGEdge> edges;
};

class RGLoader
{
public:
    static bool LoadFromFile(
        const std::string& path,
        ReachabilityGraphData& outGraph,
        std::string& outError
    );
};
