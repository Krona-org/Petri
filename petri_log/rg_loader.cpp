#include "rg_loader.h"

#include <fstream>
#include <sstream>

bool RGLoader::LoadFromFile(
    const std::string& path,
    ReachabilityGraphData& outGraph,
    std::string& outError
)
{
    outGraph.nodes.clear();
    outGraph.edges.clear();

    std::ifstream file(path);
    if (!file.is_open())
    {
        outError = "Cannot open file: " + path;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        // пустая строка или комментарий
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'N')
        {
            RGNode node;
            iss >> node.id;

            int token;
            while (iss >> token)
                node.marking.push_back(token);

            outGraph.nodes.push_back(node);
        }
        else if (type == 'E')
        {
            RGEdge edge;
            iss >> edge.from >> edge.to >> edge.transition;
            outGraph.edges.push_back(edge);
        }
        else
        {
            outError = "Unknown line type at line " + std::to_string(lineNumber);
            return false;
        }
    }

    return true;
}
