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
            ImGui::MenuItem("Cube", nullptr, false, false);
            ImGui::MenuItem("Sphere", nullptr, false, false);
            ImGui::MenuItem("Plane", nullptr, false, false);
            ImGui::MenuItem("Cylinder", nullptr, false, false);

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

    // Pendiente de implementar:
    // - FPS
    // - Renderer configuration
    // - Window configuration
    // - Input configuration
    // - Texture configuration
    // - Hardware information
    // - Memory usage
    // - SDL3 / OpenGL / DevIL versions

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

        // 1. Textura asignada manualmente.
        Component* textureBase =
            selectedObject->GetComponent(
                ComponentType::Texture);

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

        // 2. Si no hay textura personalizada, usar la del FBX.
        if (textureID == 0)
        {
            Component* rendererBase =
                selectedObject->GetComponent(
                    ComponentType::MeshRenderer);

            if (rendererBase != nullptr)
            {
                auto* meshRenderer =
                    static_cast<MeshRenderer*>(rendererBase);

                const std::size_t index =
                    meshRenderer->GetMeshIndex();

                auto& render =
                    *Engine::GetInstance().render;

                if (meshRenderer->HasMesh() &&
                    index < render.modelMeshes.size())
                {
                    textureID =
                        render.modelMeshes[index].textureID;
                }
            }
        }

        ImGui::Separator();

        ImGui::TextUnformatted(
            customTexture
            ? "Source: Custom texture"
            : "Source: Imported mesh");

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
                "No texture assigned");
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