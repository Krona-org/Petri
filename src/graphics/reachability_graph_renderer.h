#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Sphere.h"
#include "Line.h"
#include "rg_loader.h"

class ReachabilityGraphRenderer
{
public:
    explicit ReachabilityGraphRenderer(Scene& scene);
    ~ReachabilityGraphRenderer();

    // Очистка визуализации (вызывать ТОЛЬКО кнопкой)
    void Clear();

    // Построение графа (НЕ очищает автоматически)
    void BuildCircleLayout(
        const ReachabilityGraphData& graph,
        float radius = 12.0f
    );

private:
    Scene& m_Scene;

    std::vector<Sphere*> m_NodeSpheres;
    std::vector<Line*>   m_EdgeLines;
};
