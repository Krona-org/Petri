#include "reachability_graph_renderer.h"
#include <cmath>

// --- helper ---
static std::string MarkingToString(const std::vector<int>& m)
{
    std::string s = "M=(";
    for (size_t i = 0; i < m.size(); ++i)
    {
        s += std::to_string(m[i]);
        if (i + 1 < m.size()) s += ", ";
    }
    s += ")";
    return s;
}

// --- getters ---
const std::vector<ReachabilityGraphRenderer::Label3D>&
ReachabilityGraphRenderer::GetNodeLabels() const
{
    return m_NodeLabels;
}

const std::vector<ReachabilityGraphRenderer::Label3D>&
ReachabilityGraphRenderer::GetEdgeLabels() const
{
    return m_EdgeLabels;
}

// --- ctor / dtor ---
ReachabilityGraphRenderer::ReachabilityGraphRenderer(Scene& scene)
    : m_Scene(scene)
{}

ReachabilityGraphRenderer::~ReachabilityGraphRenderer()
{
    Clear();
}

// --- clear ---
void ReachabilityGraphRenderer::Clear()
{
    for (Sphere* s : m_NodeSpheres)
        m_Scene.RemoveShape(s);

    for (Line* l : m_EdgeLines)
        m_Scene.RemoveShape(l);

    m_NodeSpheres.clear();
    m_EdgeLines.clear();
    m_NodeLabels.clear();
    m_EdgeLabels.clear();
}

// --- build ---
void ReachabilityGraphRenderer::BuildCircleLayout(
    const ReachabilityGraphData& graph,
    float radius
)
{
    const int n = (int)graph.nodes.size();
    if (n == 0)
        return;

    const float yLevel     = 8.0f;
    const float nodeRadius = 0.9f;
    const glm::vec3 nodeCol{1.0f, 0.2f, 0.2f};
    const glm::vec3 edgeCol{1.0f, 1.0f, 0.0f};

    std::vector<glm::vec3> positions(n);

    for (int i = 0; i < n; ++i)
    {
        float a = 2.0f * 3.1415926f * i / n;
        positions[i] = {
            radius * std::cos(a),
            yLevel,
            radius * std::sin(a)
        };
    }

    // nodes
    for (int i = 0; i < n; ++i)
    {
        Sphere* s = new Sphere(nodeRadius, positions[i], nodeCol);
        s->SetMass(0.0f);
        s->SetUseWorldBounds(false);
        s->SetVelocity(glm::vec3(0));

        m_Scene.AddShape(s);
        m_NodeSpheres.push_back(s);

        m_NodeLabels.push_back({
            positions[i],
            MarkingToString(graph.nodes[i].marking)
        });
    }

    // edges
    for (const RGEdge_load& e : graph.edges)
    {
        if (e.from < 0 || e.from >= n || e.to < 0 || e.to >= n)
            continue;

        Line* l = new Line(positions[e.from], positions[e.to], edgeCol);
        m_Scene.AddShape(l);
        m_EdgeLines.push_back(l);

        glm::vec3 mid = 0.5f * (positions[e.from] + positions[e.to]);
        m_EdgeLabels.push_back({ mid, e.transition });
    }
}
