#pragma once

#include <vector>
#include <string>
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

    explicit ReachabilityGraphRenderer(Scene& scene);
    ~ReachabilityGraphRenderer();

    void Clear();

    void BuildCircleLayout(
        const ReachabilityGraphData& graph,
        float radius = 12.0f
    );

    void BuildTreeLayout(
    const ReachabilityGraphData& graph,
    float levelStep = 3.0f,
    float nodeSpacing = 3.0f
    );

    // ВОТ ЭТО НУЖНО main.cpp
    const std::vector<Label3D>& GetNodeLabels() const;
    const std::vector<Label3D>& GetEdgeLabels() const;

private:
    Scene& m_Scene;

    std::vector<Sphere*> m_NodeSpheres;
    std::vector<Line*>   m_EdgeLines;

    std::vector<Label3D> m_NodeLabels;
    std::vector<Label3D> m_EdgeLabels;
};