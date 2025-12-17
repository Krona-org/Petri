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

    struct Label3D
    {
        glm::vec3 worldPos;
        std::string text;
    };
    const std::vector<Label3D>& GetNodeLabels() const { return m_NodeLabels; }
    const std::vector<Label3D>& GetEdgeLabels() const { return m_EdgeLabels; }

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
    std::vector<Label3D> m_NodeLabels;
    std::vector<Label3D> m_EdgeLabels;

};
