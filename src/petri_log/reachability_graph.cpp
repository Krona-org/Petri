#include "reachability_graph.h"
#include <queue>
#include <map>
#include <fstream>

// =====================
// Build graph (BFS)
// =====================

void ReachabilityGraph::Build(const PetriNet& net, const Marking& initial)
{
    nodes.clear();
    edges.clear();

    std::map<Marking, int> visited;
    std::queue<Marking> q;

    int startId = GetOrCreateNode(initial);
    visited[initial] = startId;
    q.push(initial);

    while (!q.empty())
    {
        Marking current = q.front();
        q.pop();

        int fromId = visited[current];

        for (int t = 0; t < net.TransitionCount(); ++t)
        {
            // проверка разрешимости
            bool enabled = true;
            const auto& tr = net.GetTransition(t);

            for (const auto& arc : tr.input)
            {
                if (current[arc.place] < arc.weight)
                {
                    enabled = false;
                    break;
                }
            }

            if (!enabled)
                continue;

            // вычисляем новую разметку
            Marking next = current;

            for (const auto& arc : tr.input)
                next[arc.place] -= arc.weight;

            for (const auto& arc : tr.output)
                next[arc.place] += arc.weight;

            int toId;
            auto it = visited.find(next);
            if (it == visited.end())
            {
                toId = GetOrCreateNode(next);
                visited[next] = toId;
                q.push(next);
            }
            else
            {
                toId = it->second;
            }

            edges.push_back({
                fromId,
                toId,
                tr.name
            });
        }
    }
}


void ReachabilityGraph::SaveToFile(const std::string& filename) const
{
    std::ofstream out(filename);

    out << "# Nodes\n";
    for (const auto& n : nodes)
    {
        out << "N " << n.id;
        for (int v : n.marking)
            out << " " << v;
        out << "\n";
    }

    out << "\n# Edges\n";
    for (const auto& e : edges)
    {
        out << "E "
            << e.from << " "
            << e.to << " "
            << e.transition << "\n";
    }
}

// =====================
// Accessors
// =====================

const std::vector<RGNode>& ReachabilityGraph::GetNodes() const
{
    return nodes;
}

const std::vector<RGEdge>& ReachabilityGraph::GetEdges() const
{
    return edges;
}

// =====================
// Internal
// =====================

int ReachabilityGraph::GetOrCreateNode(const Marking& m)
{
    int id = (int)nodes.size();
    nodes.push_back({ id, m });
    return id;
}
