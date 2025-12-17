#include "reachability_graph_renderer.h"

#include <glm/glm.hpp>
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
    for (auto* s : m_NodeSpheres)
        m_Scene.RemoveShape(s);

    for (auto* l : m_EdgeLines)
        m_Scene.RemoveShape(l);

    m_NodeSpheres.clear();
    m_EdgeLines.clear();
}

void ReachabilityGraphRenderer::BuildCircleLayout(
    const ReachabilityGraphData& graph,
    float radius
)
{
    Clear();

    const int n = (int)graph.nodes.size();
    if (n == 0)
        return;

    std::vector<glm::vec3> positions(n);

    for (int i = 0; i < n; ++i)
    {
        float angle = 2.0f * 3.1415926f * i / n;
        positions[i] = glm::vec3(
            radius * cos(angle),
            2.0f,
            radius * sin(angle)
        );
    }

    // === Nodes ===
    for (int i = 0; i < n; ++i)
    {
        auto* sphere = new Sphere(
            0.3f,
            positions[i],
            glm::vec3(0.2f, 0.8f, 1.0f)
        );

        sphere->SetMass(0.0f);
        sphere->SetUseWorldBounds(false);
        sphere->SetVelocity(glm::vec3(0));

        m_Scene.AddShape(sphere);
        m_NodeSpheres.push_back(sphere);
    }

    // === Edges ===
    for (const auto& e : graph.edges)
    {
        if (e.from < 0 || e.from >= n || e.to < 0 || e.to >= n)
            continue;

        auto* line = new Line(
            positions[e.from],
            positions[e.to],
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        m_Scene.AddShape(line);
        m_EdgeLines.push_back(line);
    }
}
