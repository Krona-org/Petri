#include "reachability_graph_renderer.h"
#include <cmath>
#include <map>

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

void ReachabilityGraphRenderer::BuildTreeLayout(
    const ReachabilityGraphData& graph,
    float levelStep,
    float nodeSpacing
)
{
    Clear();

    const int n = (int)graph.nodes.size();
    if (n == 0)
        return;

    // =====================================================
    // 1. BFS-уровни
    // =====================================================
    std::vector<int> level(n, -1);
    level[0] = 0;

    bool changed = true;
    while (changed)
    {
        changed = false;
        for (const auto& e : graph.edges)
        {
            if (level[e.from] != -1)
            {
                int next = level[e.from] + 1;
                if (level[e.to] == -1 || next < level[e.to])
                {
                    level[e.to] = next;
                    changed = true;
                }
            }
        }
    }

    // =====================================================
    // 2. Группировка по уровням
    // =====================================================
    std::map<int, std::vector<int>> levels;
    for (int i = 0; i < n; ++i)
        if (level[i] >= 0)
            levels[level[i]].push_back(i);

    // =====================================================
    // 3. ПОЗИЦИИ: ПЛОСКОСТЬ XZ, Y = 0 !!!
    // =====================================================
    std::vector<glm::vec3> positions(n);

    const float yPlane = 0.0f;   // <<< ВОТ ЭТО БЫЛО ГЛАВНОЕ

    for (const auto& [lvl, nodes] : levels)
    {
        float z = -lvl * levelStep;   // глубина дерева
        int count = (int)nodes.size();

        for (int i = 0; i < count; ++i)
        {
            float x = (i - (count - 1) / 2.0f) * nodeSpacing;
            positions[nodes[i]] = glm::vec3(x, yPlane, z);
        }
    }

    // =====================================================
    // 4. Узлы
    // =====================================================
    const float nodeRadius = 0.35f;
    const glm::vec3 nodeCol(0.9f, 0.3f, 0.3f);

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

    // =====================================================
    // 5. Рёбра
    // =====================================================
    const glm::vec3 edgeCol(1.0f, 1.0f, 0.0f);

    for (const auto& e : graph.edges)
    {
        Line* l = new Line(
            positions[e.from],
            positions[e.to],
            edgeCol
        );

        m_Scene.AddShape(l);
        m_EdgeLines.push_back(l);

        glm::vec3 mid = 0.5f * (positions[e.from] + positions[e.to]);
        m_EdgeLabels.push_back({ mid, e.transition });
    }
}