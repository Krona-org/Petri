#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Window.h"
#include "shaderClass.h"
#include "Camera.h"
#include "Sphere.h"
#include "Grid.h"
#include "Scene.h"
#include "Line.h"

#include "petri_loader.h"
#include "reachability_graph.h"
#include "rg_loader.h"
#include "reachability_graph_renderer.h"

namespace fs = std::filesystem;

const unsigned int width  = 1280;
const unsigned int height = 800;

int main()
{
    srand((unsigned int)time(nullptr));

    try
    {
        // =========================
        // WINDOW + IMGUI
        // =========================
        Window win(width, height, "Petri Reachability Graph");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(win.window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // =========================
        // RENDER
        // =========================
        Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

        Camera camera(width, height, glm::vec3(0.0f, 10.0f, 18.0f));
        camera.Orientation = glm::normalize(glm::vec3(0.0f, -0.4f, -1.0f));

        Scene scene;
        //scene.SetGrid(Grid::Create(500.0f, 100));

        ReachabilityGraphRenderer rgRenderer(scene);

        // =========================
        // STATE
        // =========================
        bool petriLoaded = false;
        bool rgBuilt     = false;
        bool rgLoaded    = false;

        size_t rgNodes = 0;
        size_t rgEdges = 0;

        std::string petriStatus;
        std::string rgLoadError;

        ReachabilityGraphData rgData;

        // =========================
        // EXAMPLES (.pn)
        // =========================
        std::vector<std::string> pnFiles;
        int selectedPN = 0;
        std::string currentPNPath;

        auto RefreshExamples = [&]()
        {
            pnFiles.clear();
            try
            {
                for (const auto& e : fs::directory_iterator("examples"))
                {
                    if (e.is_regular_file() && e.path().extension() == ".pn")
                        pnFiles.push_back(e.path().string());
                }
                std::sort(pnFiles.begin(), pnFiles.end());
            }
            catch (...) {}

            if (!pnFiles.empty())
                currentPNPath = pnFiles[selectedPN];
        };

        RefreshExamples();

        // =========================
        // MAIN LOOP
        // =========================
        while (!win.ShouldClose())
        {
            scene.Update();

            ImGuiIO& io = ImGui::GetIO();
            if (!io.WantCaptureMouse && !io.WantCaptureKeyboard)
                camera.Inputs(win.window);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // =========================
            // UI
            // =========================
            ImGui::Begin("Petri / Reachability");

            ImGui::Text("Petri Net Examples (./examples)");
            ImGui::Separator();

            if (ImGui::Button("Refresh examples"))
                RefreshExamples();

            if (pnFiles.empty())
            {
                ImGui::TextColored(ImVec4(1,0,0,1), "No .pn files found");
            }
            else
            {
                std::string preview =
                    fs::path(pnFiles[selectedPN]).filename().string();

                if (ImGui::BeginCombo("Example", preview.c_str()))
                {
                    for (int i = 0; i < (int)pnFiles.size(); ++i)
                    {
                        std::string name =
                            fs::path(pnFiles[i]).filename().string();

                        bool selected = (selectedPN == i);
                        if (ImGui::Selectable(name.c_str(), selected))
                        {
                            selectedPN = i;
                            currentPNPath = pnFiles[i];
                        }
                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::Button("Load selected Petri Net"))
                {
                    try
                    {
                        PetriLoader::Load(currentPNPath);
                        petriLoaded = true;
                        rgBuilt = rgLoaded = false;
                        rgRenderer.Clear();

                        petriStatus =
                            "Loaded: " + fs::path(currentPNPath).filename().string();
                    }
                    catch (...)
                    {
                        petriLoaded = false;
                        petriStatus = "ERROR loading Petri net";
                    }
                }
            }

            if (petriLoaded)
                ImGui::TextColored(ImVec4(0,1,0,1), "%s", petriStatus.c_str());
            else
                ImGui::TextColored(ImVec4(1,0,0,1), "%s", petriStatus.c_str());

            ImGui::Separator();

            if (petriLoaded && ImGui::Button("Build Reachability Graph"))
            {
                try
                {
                    auto loaded = PetriLoader::Load(currentPNPath);

                    ReachabilityGraph rg;
                    rg.Build(loaded.net, loaded.state.GetMarking());
                    rg.SaveToFile("reachability.rg");

                    rgNodes = rg.GetNodes().size();
                    rgEdges = rg.GetEdges().size();
                    rgBuilt = true;
                }
                catch (...)
                {
                    rgBuilt = false;
                }
            }

            if (rgBuilt)
            {
                ImGui::Text("RG Nodes: %llu", (unsigned long long)rgNodes);
                ImGui::Text("RG Edges: %llu", (unsigned long long)rgEdges);
            }

            ImGui::Separator();
            ImGui::Text("Visualization");

            if (ImGui::Button("Load reachability.rg"))
            {
                rgLoaded = RGLoader_load::LoadFromFile(
                    "reachability.rg",
                    rgData,
                    rgLoadError
                );
            }

            if (!rgLoaded && !rgLoadError.empty())
                ImGui::TextColored(ImVec4(1,0,0,1), "%s", rgLoadError.c_str());

            if (rgLoaded)
            {
                ImGui::Text("RG loaded");

                if (ImGui::Button("Visualize (circle)"))
                    rgRenderer.BuildCircleLayout(rgData, 6.0f);

                if (ImGui::Button("Visualize (three)"))
                    rgRenderer.BuildTreeLayout(rgData);

                if (ImGui::Button("Clear"))
                    rgRenderer.Clear();
            }

            ImGui::Separator();
            ImGui::Text("FPS: %.1f", io.Framerate);
            ImGui::End();

            // =========================
            // RENDER
            // =========================
            glClearColor(0.05f, 0.06f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.Draw(shaderProgram, camera);

            // =========================
            // GRAPH LABELS
            // =========================
            {
                ImDrawList* dl = ImGui::GetForegroundDrawList();
                ImVec2 ds = ImGui::GetIO().DisplaySize;

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

                auto WorldToScreen = [&](const glm::vec3& w, ImVec2& out)
                {
                    glm::vec4 clip = projection * view * glm::vec4(w, 1.0f);
                    if (clip.w <= 0.001f) return false;

                    glm::vec3 ndc = glm::vec3(clip) / clip.w;
                    if (ndc.x < -1 || ndc.x > 1 || ndc.y < -1 || ndc.y > 1)
                        return false;

                    out.x = (ndc.x * 0.5f + 0.5f) * ds.x;
                    out.y = (-ndc.y * 0.5f + 0.5f) * ds.y;
                    return true;
                };

                for (const auto& lab : rgRenderer.GetNodeLabels())
                {
                    ImVec2 p;
                    if (WorldToScreen(lab.worldPos, p))
                        dl->AddText(
                            ImVec2(p.x + 6, p.y - 6),
                            IM_COL32(255,255,255,255),
                            lab.text.c_str());
                }

                for (const auto& lab : rgRenderer.GetEdgeLabels())
                {
                    ImVec2 p;
                    if (WorldToScreen(lab.worldPos, p))
                        dl->AddText(
                            ImVec2(p.x + 6, p.y + 6),
                            IM_COL32(255,255,0,255),
                            lab.text.c_str());
                }
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            win.SwapBuffers();
            win.PollEvents();
        }

        // =========================
        // SHUTDOWN
        // =========================
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        shaderProgram.Delete();
    }
    catch (...)
    {
        return -1;
    }

    return 0;
}