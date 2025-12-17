#include "petri_loader.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

LoadedPetri PetriLoader::Load(const std::string& filename)
{
    std::ifstream file(filename);

    PetriNet net;
    std::unordered_map<std::string, int> pId, tId;
    std::vector<int> initialMarking;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        char type;
        ss >> type;

        if (type == 'P')
        {
            std::string name; int tokens;
            ss >> name >> tokens;
            pId[name] = (int)net.places.size();
            net.places.push_back({ name });
            initialMarking.push_back(tokens);
        }
        else if (type == 'T')
        {
            std::string name;
            ss >> name;
            tId[name] = (int)net.transitions.size();
            net.transitions.push_back({ name });
        }
        else if (type == 'I')
        {
            std::string p, t; int w;
            ss >> p >> t >> w;
            net.transitions[tId[t]].input.push_back({ pId[p], w });
        }
        else if (type == 'O')
        {
            std::string t, p; int w;
            ss >> t >> p >> w;
            net.transitions[tId[t]].output.push_back({ pId[p], w });
        }
    }

    LoadedPetri result{
        net,
        PetriState(net)
    };
    result.state.SetMarking(initialMarking);
    return result;
}
