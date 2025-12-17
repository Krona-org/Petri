#pragma once
#include <vector>
#include <string>

struct RGNode_load
{
    int id;
    std::vector<int> marking;
};

struct RGEdge_load
{
    int from;
    int to;
    std::string transition;
};

struct ReachabilityGraphData
{
    std::vector<RGNode_load> nodes;
    std::vector<RGEdge_load> edges;
};

class RGLoader_load
{
public:
    static bool LoadFromFile(
        const std::string& path,
        ReachabilityGraphData& outGraph,
        std::string& outError
    );
};
