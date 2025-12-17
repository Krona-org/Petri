#pragma once

#include "rg_loader.h"
#include "Scene.h"
#include "Sphere.h"
#include "Line.h"

class ReachabilityGraphRenderer
{
public:
    ReachabilityGraphRenderer(Scene& scene);
    ~ReachabilityGraphRenderer();

    void Clear();
    void BuildCircleLayout(
        const ReachabilityGraphData& graph,
        float radius = 6.0f
    );

private:
    Scene& m_Scene;

    std::vector<Sphere*> m_NodeSpheres;
    std::vector<Line*>   m_EdgeLines;
};
