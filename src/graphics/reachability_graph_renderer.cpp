#include "reachability_graph_renderer.h"

#include <cmath>

ReachabilityGraphRenderer::ReachabilityGraphRenderer(Scene& scene)
    : m_Scene(scene)
{
}

ReachabilityGraphRenderer::~ReachabilityGraphRenderer()
{
    Clear();
}

void ReachabilityGraphRenderer::Clear()
{
    // ВАЖНО:
    // RemoveShape УДАЛЯЕТ объект, поэтому после этого
    // указатели больше НЕ используются
    for (Sphere* s : m_NodeSpheres)
        m_Scene.RemoveShape(s);

    for (Line* l : m_EdgeLines)
        m_Scene.RemoveShape(l);

    m_NodeSpheres.clear();
    m_EdgeLines.clear();
}

void ReachabilityGraphRenderer::BuildCircleLayout(
    const ReachabilityGraphData& graph,
    float radius
)
{
    const int n = (int)graph.nodes.size();
    if (n == 0)
        return;

    // -----------------------------
    // ЯВНЫЕ НАСТРОЙКИ ВИЗУАЛИЗАЦИИ
    // -----------------------------
    const float yLevel       = 8.0f;   // ВЫСОКО над сеткой
    const float nodeRadius   = 0.9f;   // КРУПНЫЕ сферы
    const glm::vec3 nodeCol  = {1.0f, 0.2f, 0.2f}; // КРАСНЫЕ
    const glm::vec3 edgeCol  = {1.0f, 1.0f, 0.0f}; // ЖЁЛТЫЕ

    std::vector<glm::vec3> positions(n);

    // -----------------------------
    // КРУГОВАЯ РАСКЛАДКА
    // -----------------------------
    for (int i = 0; i < n; ++i)
    {
        float angle = 2.0f * 3.1415926f * i / n;
        positions[i] = glm::vec3(
            radius * std::cos(angle),
            yLevel,
            radius * std::sin(angle)
        );
    }

    // -----------------------------
    // ВЕРШИНЫ (Sphere)
    // -----------------------------
    for (int i = 0; i < n; ++i)
    {
        Sphere* s = new Sphere(
            nodeRadius,
            positions[i],
            nodeCol
        );

        // ГАРАНТИРОВАННО статично
        s->SetMass(0.0f);
        s->SetUseWorldBounds(false);
        s->SetVelocity(glm::vec3(0));

        m_Scene.AddShape(s);
        m_NodeSpheres.push_back(s);
    }

    // -----------------------------
    // РЁБРА (Line)
    // -----------------------------
    for (const RGEdge_load& e : graph.edges)
    {
        if (e.from < 0 || e.from >= n || e.to < 0 || e.to >= n)
            continue;

        Line* l = new Line(
            positions[e.from],
            positions[e.to],
            edgeCol
        );

        m_Scene.AddShape(l);
        m_EdgeLines.push_back(l);
    }
}