#include "Editor.h"

#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Scene/Component.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Log.h"
#include "Window.h"
#include <imgui.h>
#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>

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

    // Conserva aquí tu inicialización del aspect ratio
    // de la cámara que ya tienes implementada.

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
    // Conserva aquí el código actual de movimiento de cámara,
    // zoom y ratón.

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
        if (ImGui::BeginMenu("Window"))
        {
            ImGui::MenuItem("Scene", nullptr, &showScene);
            ImGui::MenuItem("Hierarchy", nullptr, &showHierarchy);
            ImGui::MenuItem("Inspector", nullptr, &showInspector);
            ImGui::MenuItem("Console", nullptr, &showConsole);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Editor::DrawSceneWindow()
{
    ImGui::Begin("Scene");

    ImGui::TextUnformatted(
        "Scene viewport - OpenGL rendering"
    );

    ImGui::Separator();

    ImGui::TextWrapped(
        ""
    );

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
        }
    }

    ImGui::End();
}

void Editor::DrawConsole()
{
    ImGui::Begin("Console");

    if (ImGui::Button("Clear"))
        consoleLogs.clear();

    ImGui::SameLine();

    ImGui::Text(
        "Messages: %zu",
        consoleLogs.size()
    );

    ImGui::Separator();

    ImGui::BeginChild(
        "ConsoleMessages",
        ImVec2(0, 0),
        ImGuiChildFlags_None,
        ImGuiWindowFlags_HorizontalScrollbar
    );

    for (const std::string& message : consoleLogs)
    {
        ImGui::TextUnformatted(message.c_str());
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

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