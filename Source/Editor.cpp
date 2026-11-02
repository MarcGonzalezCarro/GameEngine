#include "Editor.h"

#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Scene/Component.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/TextureComponent.h"
#include "Log.h"
#include "Window.h"
#include "Render.h"
#include <imgui.h>
#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ResourceManager.h"

#include <cmath>
#include <vector>
#include <algorithm>

namespace
{
    constexpr float PI = 3.14159265359f;

    // ============================================================
    // CREAR MESH A PARTIR DE VECTORES
    // ============================================================

    Mesh BuildMesh(
        const std::vector<float>& vertices,
        const std::vector<GLuint>& indices)
    {
        Mesh mesh;

        mesh.num_vertices =
            static_cast<GLuint>(vertices.size() / 5);

        mesh.num_indices =
            static_cast<GLuint>(indices.size());

        mesh.vertices = new float[vertices.size()];
        mesh.indices = new GLuint[indices.size()];

        std::copy(
            vertices.begin(),
            vertices.end(),
            mesh.vertices
        );

        std::copy(
            indices.begin(),
            indices.end(),
            mesh.indices
        );

        mesh.diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

        return mesh;
    }


    // ============================================================
    // AÑADIR VÉRTICE
    // ============================================================

    void AddVertex(
        std::vector<float>& vertices,
        float x,
        float y,
        float z,
        float u,
        float v)
    {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(u);
        vertices.push_back(v);
    }


    // ============================================================
    // CUBE
    // Tamaño: 1 x 1 x 1
    // ============================================================

    Mesh CreateCubeMesh()
    {
        std::vector<float> vertices;
        std::vector<GLuint> indices;

        const float h = 0.5f;

        struct Face
        {
            glm::vec3 p0;
            glm::vec3 p1;
            glm::vec3 p2;
            glm::vec3 p3;
        };

        const Face faces[] =
        {
            // Front (+Z)
            {
                {-h, -h, h},
                { h, -h, h},
                { h,  h, h},
                {-h,  h, h}
            },

            // Back (-Z)
            {
                { h, -h, -h},
                {-h, -h, -h},
                {-h,  h, -h},
                { h,  h, -h}
            },

            // Right (+X)
            {
                {h, -h,  h},
                {h, -h, -h},
                {h,  h, -h},
                {h,  h,  h}
            },

            // Left (-X)
            {
                {-h, -h, -h},
                {-h, -h,  h},
                {-h,  h,  h},
                {-h,  h, -h}
            },

            // Top (+Y)
            {
                {-h, h,  h},
                { h, h,  h},
                { h, h, -h},
                {-h, h, -h}
            },

            // Bottom (-Y)
            {
                {-h, -h, -h},
                { h, -h, -h},
                { h, -h,  h},
                {-h, -h,  h}
            }
        };

        for (GLuint face = 0; face < 6; ++face)
        {
            const GLuint base = face * 4;

            const Face& f = faces[face];

            AddVertex(vertices, f.p0.x, f.p0.y, f.p0.z, 0.0f, 0.0f);
            AddVertex(vertices, f.p1.x, f.p1.y, f.p1.z, 1.0f, 0.0f);
            AddVertex(vertices, f.p2.x, f.p2.y, f.p2.z, 1.0f, 1.0f);
            AddVertex(vertices, f.p3.x, f.p3.y, f.p3.z, 0.0f, 1.0f);

            indices.insert(indices.end(),
                {
                    base, base + 1, base + 2,
                    base, base + 2, base + 3
                });
        }

        return BuildMesh(vertices, indices);
    }


    // ============================================================
    // PLANE
    // Tamaño: 1 x 1
    // Situado en Y = 0
    // ============================================================

    Mesh CreatePlaneMesh()
    {
        std::vector<float> vertices =
        {
            // Position          // UV

            -0.5f, 0.0f, -0.5f, 0.0f, 0.0f,
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f,
             0.5f, 0.0f,  0.5f, 1.0f, 1.0f,
             0.5f, 0.0f, -0.5f, 1.0f, 0.0f
        };

        std::vector<GLuint> indices =
        {
            0, 1, 2,
            0, 2, 3
        };

        return BuildMesh(vertices, indices);
    }


    // ============================================================
    // SPHERE
    // Diámetro: 1
    // ============================================================

    Mesh CreateSphereMesh()
    {
        std::vector<float> vertices;
        std::vector<GLuint> indices;

        constexpr GLuint sectors = 32;
        constexpr GLuint stacks = 16;

        const float radius = 0.5f;

        for (GLuint i = 0; i <= stacks; ++i)
        {
            const float phi =
                PI * static_cast<float>(i) /
                static_cast<float>(stacks);

            for (GLuint j = 0; j <= sectors; ++j)
            {
                const float theta =
                    2.0f * PI * static_cast<float>(j) /
                    static_cast<float>(sectors);

                const float x =
                    radius * sin(phi) * cos(theta);

                const float y =
                    radius * cos(phi);

                const float z =
                    radius * sin(phi) * sin(theta);

                const float u =
                    static_cast<float>(j) / sectors;

                const float v =
                    1.0f - static_cast<float>(i) / stacks;

                AddVertex(vertices, x, y, z, u, v);
            }
        }

        for (GLuint i = 0; i < stacks; ++i)
        {
            for (GLuint j = 0; j < sectors; ++j)
            {
                const GLuint a =
                    i * (sectors + 1) + j;

                const GLuint b =
                    (i + 1) * (sectors + 1) + j;

                const GLuint c = a + 1;
                const GLuint d = b + 1;

                indices.insert(indices.end(),
                    {
                        a, c, b,
                        c, d, b
                    });
            }
        }

        return BuildMesh(vertices, indices);
    }


    // ============================================================
    // CYLINDER
    // Diámetro: 1
    // Altura: 1
    // Eje: Y
    // ============================================================

    Mesh CreateCylinderMesh()
    {
        std::vector<float> vertices;
        std::vector<GLuint> indices;

        constexpr GLuint sectors = 32;

        const float radius = 0.5f;
        const float halfHeight = 0.5f;

        // --------------------------------------------------------
        // LATERAL
        // --------------------------------------------------------

        for (GLuint i = 0; i <= sectors; ++i)
        {
            const float angle =
                2.0f * PI * static_cast<float>(i) /
                static_cast<float>(sectors);

            const float x = radius * cos(angle);
            const float z = radius * sin(angle);

            const float u =
                static_cast<float>(i) / sectors;

            // Bottom
            AddVertex(
                vertices,
                x, -halfHeight, z,
                u, 0.0f
            );

            // Top
            AddVertex(
                vertices,
                x, halfHeight, z,
                u, 1.0f
            );
        }

        for (GLuint i = 0; i < sectors; ++i)
        {
            const GLuint base = i * 2;

            const GLuint bottom = base;
            const GLuint top = base + 1;

            const GLuint nextBottom = base + 2;
            const GLuint nextTop = base + 3;

            indices.insert(indices.end(),
                {
                    bottom, top, nextTop,
                    bottom, nextTop, nextBottom
                });
        }

        // --------------------------------------------------------
        // TAPAS
        // --------------------------------------------------------

        const GLuint bottomCenter =
            static_cast<GLuint>(vertices.size() / 5);

        AddVertex(
            vertices,
            0.0f, -halfHeight, 0.0f,
            0.5f, 0.5f
        );

        const GLuint topCenter =
            static_cast<GLuint>(vertices.size() / 5);

        AddVertex(
            vertices,
            0.0f, halfHeight, 0.0f,
            0.5f, 0.5f
        );

        const GLuint bottomRing =
            static_cast<GLuint>(vertices.size() / 5);

        // Bottom ring
        for (GLuint i = 0; i <= sectors; ++i)
        {
            const float angle =
                2.0f * PI * static_cast<float>(i) /
                static_cast<float>(sectors);

            const float x = radius * cos(angle);
            const float z = radius * sin(angle);

            AddVertex(
                vertices,
                x, -halfHeight, z,
                0.5f + 0.5f * cos(angle),
                0.5f + 0.5f * sin(angle)
            );
        }

        const GLuint topRing =
            static_cast<GLuint>(vertices.size() / 5);

        // Top ring
        for (GLuint i = 0; i <= sectors; ++i)
        {
            const float angle =
                2.0f * PI * static_cast<float>(i) /
                static_cast<float>(sectors);

            const float x = radius * cos(angle);
            const float z = radius * sin(angle);

            AddVertex(
                vertices,
                x, halfHeight, z,
                0.5f + 0.5f * cos(angle),
                0.5f + 0.5f * sin(angle)
            );
        }

        for (GLuint i = 0; i < sectors; ++i)
        {
            // Bottom cap: outward normal -Y
            indices.insert(indices.end(),
                {
                    bottomCenter,
                    bottomRing + i,
                    bottomRing + i + 1
                });

            // Top cap: outward normal +Y
            indices.insert(indices.end(),
                {
                    topCenter,
                    topRing + i + 1,
                    topRing + i
                });
        }

        return BuildMesh(vertices, indices);
    }
}

Editor::Editor() : Module()
{
    name = "editor";
}

Editor::~Editor()
{
}

bool Editor::Awake()
{
    LOG("Editor::Awake");

    auto& engine = Engine::GetInstance();

    if (!engine.window)
    {
        LOG("Editor: Window module is unavailable");
        return false;
    }

    if (!ImGui::GetCurrentContext())
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForOpenGL(
        engine.window->GetWindow(),
        engine.window->GetContext()))
    {
        LOG("Editor: ImGui SDL3 initialization failed");
        ImGui::DestroyContext();
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 460"))
    {
        LOG("Editor: ImGui OpenGL initialization failed");
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        return false;
    }

    imguiInitialized = true;

    return true;
}
bool Editor::Start()
{
    LOG("Editor::Start");

    

    return true;
}

bool Editor::PreUpdate()
{
    if (!imguiInitialized)
        return true;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    return true;
}

bool Editor::Update(float dt)
{
    if (!imguiInitialized)
        return true;

    ImGuiIO& io = ImGui::GetIO();

    const bool rightMouse =
        ImGui::IsMouseDown(ImGuiMouseButton_Right);

    const bool leftMouse =
        ImGui::IsMouseDown(ImGuiMouseButton_Left);

    const bool altPressed =
        ImGui::IsKeyDown(ImGuiKey_LeftAlt) ||
        ImGui::IsKeyDown(ImGuiKey_RightAlt);

    const bool shiftPressed =
        ImGui::IsKeyDown(ImGuiKey_LeftShift) ||
        ImGui::IsKeyDown(ImGuiKey_RightShift);

    // ========================================================
    // FOCUS - TECLA F
    // ========================================================

    if (sceneFocused &&
        !io.WantTextInput &&
        ImGui::IsKeyPressed(ImGuiKey_F))
    {
        if (selectedObject != nullptr)
        {
            Component* component =
                selectedObject->GetComponent(
                    ComponentType::Transform
                );

            if (component != nullptr)
            {
                auto* transform =
                    static_cast<Transform*>(component);

                camera.Focus(
                    transform->GetPosition(),
                    5.0f
                );
            }
        }
    }

    // ========================================================
    // FREE LOOK - RIGHT MOUSE + WASD
    // ========================================================

    if (sceneFocused &&
        sceneHovered &&
        rightMouse)
    {
        // Salir del modo órbita al entrar en FPS.
        camera.ClearOrbitTarget();

        if (!io.WantTextInput)
        {
            if (ImGui::IsKeyDown(ImGuiKey_W))
                camera.ProcessKeyboard(
                    "FORWARD", dt, shiftPressed
                );

            if (ImGui::IsKeyDown(ImGuiKey_S))
                camera.ProcessKeyboard(
                    "BACKWARD", dt, shiftPressed
                );

            if (ImGui::IsKeyDown(ImGuiKey_A))
                camera.ProcessKeyboard(
                    "LEFT", dt, shiftPressed
                );

            if (ImGui::IsKeyDown(ImGuiKey_D))
                camera.ProcessKeyboard(
                    "RIGHT", dt, shiftPressed
                );

            if (ImGui::IsKeyDown(ImGuiKey_E))
                camera.ProcessKeyboard(
                    "UP", dt, shiftPressed
                );

            if (ImGui::IsKeyDown(ImGuiKey_Q))
                camera.ProcessKeyboard(
                    "DOWN", dt, shiftPressed
                );
        }

        // Rotación con el ratón.
        if (io.MouseDelta.x != 0.0f ||
            io.MouseDelta.y != 0.0f)
        {
            camera.ProcessMouseMovement(
                io.MouseDelta.x,
                -io.MouseDelta.y
            );
        }
    }

    // ========================================================
    // ORBIT - ALT + LEFT MOUSE
    // ========================================================

    else if (sceneFocused &&
        sceneHovered &&
        altPressed &&
        leftMouse)
    {
        // Si todavía no tenemos objetivo, usar el seleccionado.
        if (!camera.IsOrbitEnabled() && selectedObject != nullptr)
        {
            Component* component =
                selectedObject->GetComponent(
                    ComponentType::Transform
                );

            if (component != nullptr)
            {
                auto* transform =
                    static_cast<Transform*>(component);

                camera.Focus(
                    transform->GetPosition(),
                    5.0f
                );
            }
        }

        if (io.MouseDelta.x != 0.0f ||
            io.MouseDelta.y != 0.0f)
        {
            camera.ProcessMouseOrbit(
                io.MouseDelta.x,
                -io.MouseDelta.y
            );
        }
    }

    // ========================================================
    // ZOOM - MOUSE WHEEL
    // ========================================================

    if (sceneHovered && io.MouseWheel != 0.0f)
    {
        camera.ProcessMouseScroll(io.MouseWheel);
    }

    return true;
}

bool Editor::PostUpdate()
{
    if (!imguiInitialized)
        return true;

    DrawDockspace();
    DrawMainMenu();

    if (showScene)
        DrawSceneWindow();

    if (showHierarchy)
        DrawHierarchy();

    if (showInspector)
        DrawInspector();

    if (showConsole)
        DrawConsole();

    if (showConfiguration)
        DrawConfiguration();

    if (showAbout)
        DrawAbout();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData()
    );

    return true;
}

void Editor::DrawDockspace()
{
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    const ImGuiViewport* viewport =
        ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::Begin("EditorDockspace", nullptr, flags);

    ImGui::DockSpace(
        ImGui::GetID("MainDockspace"),
        ImVec2(0.0f, 0.0f)
    );

    ImGui::End();
}

void Editor::DrawMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        // ====================================================
        // FILE
        // ====================================================

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                SDL_Event event{};
                event.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&event);
            }

            ImGui::EndMenu();
        }

        // ====================================================
        // VIEW
        // ====================================================

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Scene", nullptr, &showScene);
            ImGui::MenuItem("Hierarchy", nullptr, &showHierarchy);
            ImGui::MenuItem("Inspector", nullptr, &showInspector);
            ImGui::MenuItem("Console", nullptr, &showConsole);

            ImGui::Separator();

            ImGui::MenuItem(
                "Configuration",
                nullptr,
                &showConfiguration
            );

            ImGui::MenuItem(
                "About",
                nullptr,
                &showAbout
            );

            ImGui::EndMenu();
        }

        // ====================================================
        // GEOMETRY
        // ====================================================

        if (ImGui::BeginMenu("Geometry"))
        {
            if (ImGui::MenuItem("Cube"))
            {
                CreatePrimitive("Cube");
            }

            if (ImGui::MenuItem("Sphere"))
            {
                CreatePrimitive("Sphere");
            }

            if (ImGui::MenuItem("Plane"))
            {
                CreatePrimitive("Plane");
            }

            if (ImGui::MenuItem("Cylinder"))
            {
                CreatePrimitive("Cylinder");
            }

            ImGui::EndMenu();
        }

        // ====================================================
        // HELP
        // ====================================================

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Documentation"))
            {
                SDL_OpenURL(
                    "https://github.com/TU_REPOSITORIO/docs"
                );
            }

            if (ImGui::MenuItem("Report a Bug"))
            {
                SDL_OpenURL(
                    "https://github.com/TU_REPOSITORIO/issues"
                );
            }

            if (ImGui::MenuItem("Download Latest"))
            {
                SDL_OpenURL(
                    "https://github.com/TU_REPOSITORIO/releases"
                );
            }

            ImGui::Separator();

            if (ImGui::MenuItem("About"))
            {
                showAbout = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Editor::DrawSceneWindow()
{
    ImGui::Begin("Scene");

    sceneHovered = ImGui::IsWindowHovered();
    sceneFocused = ImGui::IsWindowFocused();

    // Espacio disponible dentro de la ventana
    ImVec2 available = ImGui::GetContentRegionAvail();

    // Evitar dimensiones inválidas
    int width = std::max(
        1,
        static_cast<int>(available.x)
    );

    int height = std::max(
        1,
        static_cast<int>(available.y)
    );

    auto& engine = Engine::GetInstance();
     

    if (engine.render != nullptr)
    {
        // Ajustar la resolución de la textura al viewport
        engine.render->SetSceneViewportSize(width, height);

        // Mostrar la textura OpenGL en ImGui
        ImGui::Image(
            (ImTextureID)(intptr_t)engine.render->GetSceneTexture(),
            ImVec2(
                static_cast<float>(width),
                static_cast<float>(height)
            ),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );
    }

    ImGui::End();
}

void Editor::DrawHierarchy()
{
    ImGui::Begin("Hierarchy");

    auto scene = Engine::GetInstance().scene;

    if (scene)
    {
        const auto& roots = scene->GetRootObjects();

        for (const auto& object : roots)
        {
            if (object)
                DrawHierarchyNode(*object);
        }
    }

    if (ImGui::Button("Create GameObject"))
    {
        if (scene)
        {
            GameObject& object =
                scene->CreateGameObject("GameObject");

            selectedObject = &object;
        }
    }

    ImGui::End();
}

void Editor::DrawHierarchyNode(GameObject& object)
{
    const bool hasChildren =
        !object.GetChildren().empty();

    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    if (!hasChildren)
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (selectedObject == &object)
        flags |= ImGuiTreeNodeFlags_Selected;

    const bool opened = ImGui::TreeNodeEx(
        static_cast<void*>(&object),
        flags,
        "%s",
        object.GetName().c_str()
    );

    if (ImGui::IsItemClicked())
        selectedObject = &object;

    if (opened)
    {
        for (const auto& child : object.GetChildren())
        {
            if (child)
                DrawHierarchyNode(*child);
        }

        ImGui::TreePop();
    }
}

void Editor::DrawConfiguration()
{
    if (!showConfiguration)
        return;

    ImGui::Begin(
        "Configuration",
        &showConfiguration
    );

    auto& engine = Engine::GetInstance();

    // ========================================================
    // PERFORMANCE
    // ========================================================

    if (ImGui::CollapsingHeader(
        "Performance",
        ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGuiIO& io = ImGui::GetIO();

        float fps = io.Framerate;
        float frameTime = fps > 0.0f
            ? 1000.0f / fps
            : 0.0f;

        // Guardar FPS en el historial circular
        fpsHistory[fpsHistoryOffset] = fps;

        fpsHistoryOffset =
            (fpsHistoryOffset + 1) % FPS_HISTORY_SIZE;

        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("Frame time: %.3f ms", frameTime);

        ImGui::Spacing();

        ImGui::PlotLines(
            "FPS History",
            fpsHistory,
            FPS_HISTORY_SIZE,
            fpsHistoryOffset,
            nullptr,
            0.0f,
            120.0f,
            ImVec2(0, 100)
        );

        ImGui::Spacing();
        ImGui::Separator();
    }

    // ========================================================
    // RENDERER
    // ========================================================

    if (ImGui::CollapsingHeader(
        "Renderer",
        ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (engine.render != nullptr)
        {
            Render& render = *engine.render;

            ImGui::SliderFloat(
                "Normal Length",
                &render.normalLength,
                0.01f,
                5.0f
            );

            ImGui::ColorEdit3(
                "Normal Color",
                glm::value_ptr(render.normalColor)
            );
        }

        ImGui::Separator();
    }

    // ========================================================
    // WINDOW
    // ========================================================

    if (ImGui::CollapsingHeader("Window"))
    {
        if (engine.window != nullptr)
        {
            int width = 0;
            int height = 0;

            SDL_GetWindowSize(
                engine.window->GetWindow(),
                &width,
                &height
            );

            ImGui::Text(
                "Resolution: %d x %d",
                width,
                height
            );

            ImGui::Text(
                "Scale: %.2f",
                engine.window->GetScale()
            );

            SDL_WindowFlags flags =
                SDL_GetWindowFlags(
                    engine.window->GetWindow()
                );

            ImGui::Text(
                "Fullscreen: %s",
                (flags & SDL_WINDOW_FULLSCREEN)
                ? "Yes"
                : "No"
            );
        }

        ImGui::Separator();
    }

    // ========================================================
    // HARDWARE / OPENGL
    // ========================================================

    if (ImGui::CollapsingHeader("Hardware"))
    {
        ImGui::Text(
            "Vendor: %s",
            glGetString(GL_VENDOR)
            ? reinterpret_cast<const char*>(
                glGetString(GL_VENDOR))
            : "Unknown"
        );

        ImGui::Text(
            "GPU: %s",
            glGetString(GL_RENDERER)
            ? reinterpret_cast<const char*>(
                glGetString(GL_RENDERER))
            : "Unknown"
        );

        ImGui::Text(
            "OpenGL: %s",
            glGetString(GL_VERSION)
            ? reinterpret_cast<const char*>(
                glGetString(GL_VERSION))
            : "Unknown"
        );

        ImGui::Text(
            "GLSL: %s",
            glGetString(GL_SHADING_LANGUAGE_VERSION)
            ? reinterpret_cast<const char*>(
                glGetString(GL_SHADING_LANGUAGE_VERSION))
            : "Unknown"
        );
    }

    ImGui::End();
}

void Editor::DrawAbout()
{
    if (!showAbout)
        return;

    ImGui::Begin(
        "About",
        &showAbout
    );

    ImGui::Spacing();

    ImGui::SetWindowFontScale(1.5f);

    ImGui::Text("MY ENGINE");

    ImGui::SetWindowFontScale(1.0f);

    ImGui::Separator();

    ImGui::Spacing();

    ImGui::Text("Version: 1.0");

    ImGui::Text("Geometry Visualizer");

    ImGui::Spacing();

    ImGui::Separator();

    // ========================================================
    // TEAM
    // ========================================================

    if (ImGui::CollapsingHeader(
        "Development Team",
        ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BulletText("Member 1 - GitHub");
        ImGui::BulletText("Member 2 - GitHub");
        ImGui::BulletText("Member 3 - GitHub");
    }

    // ========================================================
    // LIBRARIES
    // ========================================================

    if (ImGui::CollapsingHeader(
        "Third Party Libraries",
        ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BulletText("SDL3");
        ImGui::BulletText("OpenGL");
        ImGui::BulletText("GLM");
        ImGui::BulletText("Assimp");
        ImGui::BulletText("DevIL");
        ImGui::BulletText("Dear ImGui");
    }

    // ========================================================
    // LICENSE
    // ========================================================

    if (ImGui::CollapsingHeader("License"))
    {
        ImGui::TextWrapped(
            "This project is licensed under the MIT License."
        );

        ImGui::Spacing();

        ImGui::TextWrapped(
            "Copyright (c) 2026"
        );

        if (ImGui::Button("View MIT License"))
        {
            SDL_OpenURL(
                "https://opensource.org/license/mit"
            );
        }
    }

    ImGui::Spacing();

    ImGui::Separator();

    ImGui::TextDisabled(
        "Geometry Visualizer - 2026"
    );

    ImGui::End();
}

void Editor::DrawInspector()
{
    ImGui::Begin("Inspector");

    if (selectedObject == nullptr)
    {
        ImGui::TextUnformatted("No GameObject selected");
        ImGui::End();
        return;
    }

    ImGui::Text(
        "GameObject: %s",
        selectedObject->GetName().c_str()
    );

    ImGui::Separator();

    // ========================================================
    // TRANSFORM
    // ========================================================

    Component* component =
        selectedObject->GetComponent(
            ComponentType::Transform
        );

    if (component != nullptr)
    {
        auto* transform =
            static_cast<Transform*>(component);

        if (ImGui::CollapsingHeader(
            "Transform",
            ImGuiTreeNodeFlags_DefaultOpen))
        {
            glm::vec3 position = transform->GetPosition();
            glm::vec3 rotation = transform->GetRotation();
            glm::vec3 scale = transform->GetScale();

            if (ImGui::DragFloat3(
                "Position",
                &position.x,
                0.1f))
            {
                transform->SetPosition(position);
            }

            if (ImGui::DragFloat3(
                "Rotation",
                &rotation.x,
                0.5f))
            {
                transform->SetRotation(rotation);
            }

            if (ImGui::DragFloat3(
                "Scale",
                &scale.x,
                0.01f))
            {
                transform->SetScale(scale);
            }

            ImGui::Separator();
        }
    }

    // ========================================================
    // MESH RENDERER
    // ========================================================

    Component* renderer =
        selectedObject->GetComponent(
            ComponentType::MeshRenderer
        );

    if (renderer != nullptr)
    {
        if (ImGui::CollapsingHeader(
            "Mesh Renderer",
            ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto* meshRenderer =
                static_cast<MeshRenderer*>(renderer);

            ImGui::Text(
                "Mesh index: %zu",
                meshRenderer->GetMeshIndex()
            );

            ImGui::Text(
                "Has mesh: %s",
                meshRenderer->HasMesh() ? "Yes" : "No"
            );

            ImGui::Separator();
            ImGui::Text("Normals");

            bool faceNormals = meshRenderer->GetShowFaceNormals();

            if (ImGui::Checkbox("Face Normals", &faceNormals))
            {
                meshRenderer->SetShowFaceNormals(faceNormals);
            }

            bool vertexNormals = meshRenderer->GetShowVertexNormals();

            if (ImGui::Checkbox("Vertex Normals", &vertexNormals))
            {
                meshRenderer->SetShowVertexNormals(vertexNormals);
            }
        }
    }

    // ========================================================
    // TEXTURE
    // ========================================================

    if (ImGui::CollapsingHeader(
        "Texture",
        ImGuiTreeNodeFlags_DefaultOpen))
    {
        GLuint textureID = 0;

        bool customTexture = false;

        auto& engine = Engine::GetInstance();

        // ====================================================
        // 1. TEXTURA OVERRIDE DEL MESH RENDERER
        // ====================================================

        Component* rendererBase =
            selectedObject->GetComponent(
                ComponentType::MeshRenderer
            );

        if (rendererBase != nullptr)
        {
            auto* meshRenderer =
                static_cast<MeshRenderer*>(rendererBase);

            if (meshRenderer->HasTextureOverride())
            {
                textureID = engine.render->GetTexturePreview(
                    meshRenderer->GetTexturePath()
                );

                customTexture = (textureID != 0);
            }

            // ====================================================
            // 2. TEXTURA ORIGINAL DEL FBX
            // ====================================================

            if (textureID == 0 && meshRenderer->HasMesh())
            {
                const std::size_t index =
                    meshRenderer->GetMeshIndex();

                if (index < engine.render->modelMeshes.size())
                {
                    textureID =
                        engine.render->modelMeshes[index].textureID;
                }
            }
        }

        // ====================================================
        // 3. TEXTURE COMPONENT
        // ====================================================

        if (textureID == 0)
        {
            Component* textureBase =
                selectedObject->GetComponent(
                    ComponentType::Texture
                );

            if (textureBase != nullptr)
            {
                auto* textureComponent =
                    static_cast<TextureComponent*>(textureBase);

                if (textureComponent->HasTexture())
                {
                    textureID = textureComponent->GetTextureID();
                    customTexture = (textureID != 0);
                }
            }
        }

        // ====================================================
        // 4. MOSTRAR INFORMACIÓN
        // ====================================================

        ImGui::Separator();

        ImGui::TextUnformatted(
            customTexture
            ? "Source: Custom texture"
            : "Source: Imported mesh"
        );

        if (textureID != 0)
        {
            ImGui::Image(
                (ImTextureID)(intptr_t)textureID,
                ImVec2(200.0f, 200.0f),
                ImVec2(0.0f, 1.0f),
                ImVec2(1.0f, 0.0f)
            );
        }
        else
        {
            ImGui::TextUnformatted(
                "No texture assigned"
            );
        }
    }

    ImGui::End();
}

void Editor::DrawConsole()
{
    ImGui::Begin("Console");

    if (ImGui::Button("Clear"))
    {
        Logger::Clear();
    }

    ImGui::SameLine();

    const auto& messages = Logger::GetMessages();

    ImGui::Text("Messages: %zu", messages.size());

    ImGui::Separator();

    ImGui::BeginChild(
        "ConsoleMessages",
        ImVec2(0, 0),
        ImGuiChildFlags_None,
        ImGuiWindowFlags_HorizontalScrollbar
    );

    for (const std::string& message : messages)
    {
        ImGui::TextUnformatted(message.c_str());
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::End();
}

void Editor::AddConsoleLog(const std::string& message)
{
    consoleLogs.push_back(message);
}

GameObject* Editor::GetSelectedObject() const
{
    return selectedObject;
}

void Editor::CreatePrimitive(const std::string& primitiveName)
{
    auto& engine = Engine::GetInstance();

    if (!engine.scene || !engine.render)
    {
        LOG("Cannot create primitive: Scene or Render is null");
        return;
    }

    // ========================================================
    // GENERAR GEOMETRÍA CPU
    // ========================================================

    Mesh mesh;

    if (primitiveName == "Cube")
    {
        mesh = CreateCubeMesh();
    }
    else if (primitiveName == "Sphere")
    {
        mesh = CreateSphereMesh();
    }
    else if (primitiveName == "Plane")
    {
        mesh = CreatePlaneMesh();
    }
    else if (primitiveName == "Cylinder")
    {
        mesh = CreateCylinderMesh();
    }
    else
    {
        LOG("Unknown primitive: %s", primitiveName.c_str());
        return;
    }

    // ========================================================
    // SUBIR MALLA A GPU
    // ========================================================

    const std::size_t meshIndex =
        engine.render->modelMeshes.size();

    MeshGPU gpuMesh =
        engine.render->UploadMesh(mesh);

    engine.render->modelMeshes.push_back(gpuMesh);

    // ========================================================
    // CREAR GAME OBJECT
    // ========================================================

    GameObject& object =
        engine.scene->CreateGameObject(primitiveName);

    // ========================================================
    // AÑADIR MESH RENDERER
    // ========================================================

    Component& component =
        object.CreateComponent(ComponentType::MeshRenderer);

    auto* meshRenderer =
        static_cast<MeshRenderer*>(&component);

    meshRenderer->SetMeshIndex(meshIndex);

    // ========================================================
    // SELECCIONAR OBJETO
    // ========================================================

    selectedObject = &object;

    // ========================================================
    // LOG
    // ========================================================

    LOG(
        "Primitive created: %s | Mesh index: %zu",
        primitiveName.c_str(),
        meshIndex
    );

    AddConsoleLog(
        "Created primitive: " + primitiveName
    );
}

void Camera::ProcessMouseMovement(
    float xoffset,
    float yoffset,
    bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        pitch = glm::clamp(pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (orbitEnabled)
    {
        orbitDistance -= yoffset *
            glm::max(orbitDistance * 0.1f, 0.5f);

        orbitDistance = glm::clamp(
            orbitDistance,
            0.5f,
            500.0f
        );

        UpdateOrbitPosition();
    }
    else
    {
        position += front * yoffset * movementSpeed * 0.2f;
    }
}

void Camera::ProcessMousePan(
    float xoffset,
    float yoffset,
    float dt)
{
    const float speed = movementSpeed * dt;

    position -= right * xoffset * speed;
    position += up * yoffset * speed;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 newFront;

    newFront.x = cos(glm::radians(yaw)) *
        cos(glm::radians(pitch));

    newFront.y = sin(glm::radians(pitch));

    newFront.z = sin(glm::radians(yaw)) *
        cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessMouseOrbit(
    float xoffset,
    float yoffset
)
{
    if (!orbitEnabled)
        return;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    UpdateCameraVectors();

    UpdateOrbitPosition();
}

void Camera::UpdateOrbitPosition()
{
    position = orbitTarget - front * orbitDistance;
}

void Camera::Focus(
    const glm::vec3& target,
    float distance
)
{
    orbitTarget = target;
    orbitDistance = glm::max(distance, 0.5f);

    orbitEnabled = true;

    UpdateOrbitPosition();

    LOG(
        "Camera focused on: (%.2f, %.2f, %.2f)",
        target.x,
        target.y,
        target.z
    );
}

void Camera::ClearOrbitTarget()
{
    orbitEnabled = false;
}

bool Editor::CleanUp()
{
    LOG("Editor::CleanUp");

    if (imguiInitialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        imguiInitialized = false;
    }

    return true;
}