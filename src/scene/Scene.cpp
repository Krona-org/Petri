#include "Scene.h"

#include <algorithm>
#include <cmath>

#include <glad/glad.h>   // glDepthMask
#include <glm/glm.hpp>

#include "Sphere.h"

// ============================================================
// ШАР–ШАР КОЛЛИЗИИ (с массой и упругостью)
// ============================================================
static void ResolveSphereSphere(std::vector<Sphere*>& spheres)
{
    for (size_t i = 0; i < spheres.size(); ++i)
    {
        for (size_t j = i + 1; j < spheres.size(); ++j)
        {
            Sphere* A = spheres[i];
            Sphere* B = spheres[j];

            glm::vec3 dp = A->GetPosition() - B->GetPosition();
            float dist2 = glm::dot(dp, dp);

            float r = A->GetRadius() + B->GetRadius();
            if (dist2 >= r * r)
                continue;

            float dist = std::sqrt(dist2);
            if (dist < 1e-6f)
            {
                dp = glm::vec3(1, 0, 0);
                dist = 1.0f;
            }

            glm::vec3 n = dp / dist;
            float penetration = r - dist;

            float invMassA = A->GetInvMass();
            float invMassB = B->GetInvMass();
            float invMassSum = invMassA + invMassB;

            if (invMassSum <= 0.0f)
                continue;

            // --- позиционная коррекция (чтобы не слипались)
            glm::vec3 correction = n * (penetration / invMassSum) * 0.8f;
            A->SetPosition(A->GetPosition() + correction * invMassA);
            B->SetPosition(B->GetPosition() - correction * invMassB);

            // --- импульс
            glm::vec3 rv = A->GetVelocity() - B->GetVelocity();
            float velAlongNormal = glm::dot(rv, n);

            if (velAlongNormal > 0.0f)
                continue;

            float e = 0.5f * (A->GetRestitution() + B->GetRestitution());

            float impulseMag = -(1.0f + e) * velAlongNormal;
            impulseMag /= invMassSum;

            glm::vec3 impulse = impulseMag * n;

            A->VelocityRef() += impulse * invMassA;
            B->VelocityRef() -= impulse * invMassB;

        }
    }
}

// ============================================================
// SCENE
// ============================================================

Scene::Scene() : grid(nullptr) {}

Scene::~Scene()
{
    for (auto obj : objects)
        delete obj;

    if (grid)
        delete grid;
}

void Scene::AddShape(Shape* shape)
{
    objects.push_back(shape);
}

void Scene::RemoveShape(Shape* shape)
{
    auto it = std::find(objects.begin(), objects.end(), shape);
    if (it != objects.end())
    {
        delete *it;
        objects.erase(it);
    }
}

void Scene::SetGrid(Grid* g)
{
    grid = g;
}

// ============================================================
// UPDATE
// ============================================================

void Scene::Update()
{
    timer.Update();
    float dt = timer.GetDeltaTime();
    float t  = timer.GetTotalTime();

    std::vector<Sphere*> spheres;
    spheres.reserve(objects.size());

    ContainerSphere* container = nullptr;

    // 1) обновляем объекты и собираем шары
    for (auto obj : objects)
    {
        obj->Update(t, dt);

        if (auto* c = dynamic_cast<ContainerSphere*>(obj))
            container = c;

        if (auto* s = dynamic_cast<Sphere*>(obj))
            spheres.push_back(s);
    }

    // 2) шар–шар
    ResolveSphereSphere(spheres);

    // 3) шар ВНУТРИ контейнера
    if (container)
    {
        glm::vec3 center = container->GetPosition();
        float R = container->GetRadius();

        for (auto* s : spheres)
        {
            if (s == container) continue;

            float r = s->GetRadius();
            glm::vec3 d = s->GetPosition() - center;
            float dist = glm::length(d);

            if (dist + r > R)
            {
                if (dist < 1e-6f)
                    d = glm::vec3(1, 0, 0);
                else
                    d /= dist; // нормаль наружу

                // вернуть внутрь
                s->SetPosition(center + d * (R - r));

                // отражаем скорость
                glm::vec3 v = s->GetVelocity();
                float vn = glm::dot(v, d);

                glm::vec3 vN = vn * d;
                glm::vec3 vT = v - vN;

                float e = s->GetRestitution();
                s->SetVelocity((-vN * e) + vT);
            }
        }
    }
}

void Scene::Update(float deltaTime)
{
    float t = timer.GetTotalTime() + deltaTime;

    std::vector<Sphere*> spheres;
    spheres.reserve(objects.size());

    ContainerSphere* container = nullptr;

    // 1) обновляем все объекты и собираем шары
    for (auto obj : objects)
    {
        obj->Update(t, deltaTime);

        if (auto* c = dynamic_cast<ContainerSphere*>(obj))
            container = c;

        if (auto* s = dynamic_cast<Sphere*>(obj))
            spheres.push_back(s);
    }

    // 2) шар–шар коллизии
    ResolveSphereSphere(spheres);

    // 3) шар ВНУТРИ контейнера
    if (container)
    {
        glm::vec3 center = container->GetPosition();
        float R = container->GetRadius();

        for (auto* s : spheres)
        {
            if (s == container) continue;

            float r = s->GetRadius();
            glm::vec3 d = s->GetPosition() - center;
            float dist = glm::length(d);

            if (dist + r > R)
            {
                if (dist < 1e-6f)
                    d = glm::vec3(1, 0, 0);
                else
                    d /= dist; // нормаль НАРУЖУ

                // 1) вернуть ВНУТРЬ
                s->SetPosition(center + d * (R - r));

                // 2) отразить скорость (ВСЕГДА)
                glm::vec3 v = s->GetVelocity();

                float vn = glm::dot(v, d);
                glm::vec3 vNormal = vn * d;
                glm::vec3 vTangent = v - vNormal;

                // отражаем нормаль, касательную оставляем
                float e = s->GetRestitution();
                glm::vec3 reflected =
                    (-vNormal * e) + vTangent;

                s->SetVelocity(reflected);
            }
        }
    }
}


// ============================================================
// DRAW
// ============================================================

void Scene::Draw(Shader& shader, Camera& camera)
{
    glm::mat4 view = glm::lookAt(
        camera.Position,
        camera.Position + camera.Orientation,
        camera.Up
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)camera.width / camera.height,
        0.1f,
        1000.0f
    );

    float time = timer.GetTotalTime();

    // --- GRID (непрозрачный)
    if (grid)
        grid->Draw(shader, view, projection, time);

    // --- НЕПРОЗРАЧНЫЕ
    for (auto obj : objects)
    {
        if (!obj->IsTransparent())
            obj->Draw(shader, view, projection, time);
    }

    // --- ПРОЗРАЧНЫЕ (последними)
    glDepthMask(GL_FALSE);

    for (auto obj : objects)
    {
        if (obj->IsTransparent())
            obj->Draw(shader, view, projection, time);
    }

    glDepthMask(GL_TRUE);
}

// ============================================================
// TIMER
// ============================================================

float Scene::GetTotalTime() const
{
    return timer.GetTotalTime();
}

float Scene::GetDeltaTime() const
{
    return timer.GetDeltaTime();
}

void Scene::ResolveSphereInsideContainer(
    ContainerSphere* container,
    const std::vector<Sphere*>& spheres)
{
    glm::vec3 center = container->GetPosition();
    float R = container->GetRadius();

    for (auto* s : spheres)
    {
        if (s == container) continue;

        float r = s->GetRadius();
        glm::vec3 d = s->GetPosition() - center;
        float dist = glm::length(d);

        // если шар вышел за внутреннюю поверхность
        if (dist + r > R)
        {
            if (dist < 1e-6f)
                d = glm::vec3(1, 0, 0);
            else
                d /= dist;

            // вернуть внутрь
            s->SetPosition(center + d * (R - r));

            // отражаем скорость
            glm::vec3 v = s->GetVelocity();
            float vn = glm::dot(v, d);

            if (vn > 0.0f)
            {
                glm::vec3 reflected =
                    v - (1.0f + s->GetRestitution()) * vn * d;
                s->SetVelocity(reflected);
            }
        }
    }
}
