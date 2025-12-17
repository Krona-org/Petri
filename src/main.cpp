#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <string>

// ================= ImGui =================
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// ================= Engine =================
#include "Window.h"
#include "shaderClass.h"
#include "Camera.h"
#include "Sphere.h"
#include "Grid.h"
#include "Scene.h"
#include "ContainerSphere.h"
#include "Line.h"

// ================= Petri =================
#include "petri_loader.h"
#include "reachability_graph.h"

// ================= Reachability Visualization =================
#include "rg_loader.h"
#include "reachability_graph_renderer.h"

const unsigned int width  = 1280;
const unsigned int height = 800;

int main()
{
    srand((unsigned int)time(nullptr));

    try
    {
        // =====================================================
        // Window
        // =====================================================
        Window win(width, height, "Petri Reachability Test");

        // =====================================================
        // ImGui init
        // =====================================================
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(win.window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // =====================================================
        // OpenGL objects
        // =====================================================
        Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

        Camera camera(
            width,
            height,
            glm::vec3(0.0f, 10.0f, 18.0f)
        );
        camera.Orientation = glm::normalize(glm::vec3(0.0f, -0.4f, -1.0f));

        Scene scene;
        scene.SetGrid(Grid::Create(500.0f, 100));

        auto* container = new ContainerSphere(
            6.0f,
            glm::vec3(0.0f, 6.0f, 0.0f),
            glm::vec3(0.2f, 0.8f, 1.0f)
        );
        container->SetUseWorldBounds(false);
        container->SetMass(0.0f);
        container->SetAlpha(0.25f);
        scene.AddShape(container);

        // =====================================================
        // Petri state
        // =====================================================
        bool petriLoaded = false;
        bool rgBuilt = false;
        std::string petriStatus = "Petri net not loaded";

        size_t rgNodes = 0;
        size_t rgEdges = 0;

        // =====================================================
        // Reachability visualization state
        // =====================================================
        ReachabilityGraphData rgData;
        std::string rgLoadError;
        bool rgLoaded = false;

        ReachabilityGraphRenderer rgRenderer(scene);

        // =====================================================
        // Main loop
        // =====================================================
        while (!win.ShouldClose())
        {
            ImGuiIO& io = ImGui::GetIO();
            if (!io.WantCaptureMouse && !io.WantCaptureKeyboard)
            {
                camera.Inputs(win.window);
            }

            // ================= ImGui frame =================
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // =================================================
            // ImGui window
            // =================================================
            ImGui::Begin("Petri / Reachability Test");

            ImGui::Text("Petri Net and Reachability Graph");
            ImGui::Separator();

            // ---------- Load Petri ----------
            if (ImGui::Button("Load Petri Net (example.pn)"))
            {
                try
                {
                    PetriLoader::Load("example.pn");
                    petriLoaded = true;
                    rgBuilt = false;
                    petriStatus = "Petri net loaded successfully";
                }
                catch (...)
                {
                    petriLoaded = false;
                    rgBuilt = false;
                    petriStatus = "ERROR: cannot load example.pn";
                }
            }

            if (petriLoaded)
                ImGui::TextColored(ImVec4(0,1,0,1), "%s", petriStatus.c_str());
            else
                ImGui::TextColored(ImVec4(1,0,0,1), "%s", petriStatus.c_str());

            ImGui::Separator();

            // ---------- Build Reachability ----------
            if (petriLoaded && ImGui::Button("Build Reachability Graph"))
            {
                try
                {
                    auto loaded = PetriLoader::Load("example.pn");

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
                ImGui::TextColored(ImVec4(0,1,0,1),
                    "Reachability graph built successfully");
                ImGui::Text("Nodes: %llu", (unsigned long long)rgNodes);
                ImGui::Text("Edges: %llu", (unsigned long long)rgEdges);
                ImGui::Text("Saved: reachability.rg");
            }
            else
            {
                ImGui::TextColored(ImVec4(1,1,0,1),
                    "Reachability graph not built yet");
            }

            ImGui::Separator();
            ImGui::Text("Reachability Graph Visualization");

            // ---------- Load RG ----------
            if (ImGui::Button("Load reachability.rg"))
            {
                rgLoaded = RGLoader_load::LoadFromFile(
                    "reachability.rg",
                    rgData,
                    rgLoadError
                );
            }

            if (!rgLoaded && !rgLoadError.empty())
            {
                ImGui::TextColored(ImVec4(1,0,0,1), "%s", rgLoadError.c_str());
            }

            if (rgLoaded)
            {
                ImGui::TextColored(ImVec4(0,1,0,1), "RG loaded");
                ImGui::Text("RG Nodes: %llu",
                    (unsigned long long)rgData.nodes.size());
                ImGui::Text("RG Edges: %llu",
                    (unsigned long long)rgData.edges.size());

                if (ImGui::Button("Visualize graph (circle)"))
                {
                    rgRenderer.BuildCircleLayout(rgData, 6.0f);
                }

                if (ImGui::Button("Clear visualization"))
                {
                    rgRenderer.Clear();
                }
            }

            ImGui::Separator();
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::End();

            // ================= Render =================
            glClearColor(0.05f, 0.06f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.Draw(shaderProgram, camera);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            win.SwapBuffers();
            win.PollEvents();
        }

        // =====================================================
        // Shutdown
        // =====================================================
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