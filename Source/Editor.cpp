#include "Editor.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "Log.h"

#include "Engine.h"
#include "Window.h"

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

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // SDL3 + ImGui
    ImGui_ImplSDL3_InitForOpenGL(
        Engine::GetInstance().window->GetWindow(),
        Engine::GetInstance().window->GetContext()
    );

    // OpenGL + ImGui
    ImGui_ImplOpenGL3_Init("#version 460");

    return true;
}

bool Editor::Start()
{
    return true;
}

bool Editor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    return true;
}

bool Editor::Update(float dt)
{
    ImGui::Begin("Test Window");

    ImGui::Text("Hello ImGui!");

    ImGui::Button("Test Button");

    ImGui::End();

    return true;
}

bool Editor::PostUpdate()
{
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData()
    );

    return true;
}

bool Editor::CleanUp()
{
    LOG("Editor::CleanUp");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    ImGui::DestroyContext();

    return true;
}