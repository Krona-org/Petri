#pragma once
#include "Sphere.h"

class ContainerSphere : public Sphere
{
public:
    ContainerSphere(float radius,
                    const glm::vec3& pos,
                    const glm::vec3& col)
        : Sphere(radius, pos, col)
    {
        SetMass(0.0f);     // бесконечная масса → неподвижный
        SetAlpha(0.25f);  // прозрачный
    }

    bool IsTransparent() const override { return true; }
};
