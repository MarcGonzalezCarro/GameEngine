#include "Editor.h"
#include <imgui.h>


Editor::Editor() : Module()
{
    name = "editor";
}

Editor::~Editor()
{
}

bool Editor::Awake()
{
    return true;
}

bool Editor::Start()
{
    return true;
}

bool Editor::PreUpdate()
{
    return true;
}

bool Editor::Update(float dt)
{
    return true;
}

bool Editor::PostUpdate()
{
    return true;
}

bool Editor::CleanUp()
{
    return true;
}