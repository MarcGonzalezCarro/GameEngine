#include "Engine.h"

#include <iostream>

#include "Log.h"

#include "Window.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Render.h"
#include "Editor.h"

//#include "Textures.h"
//#include "Audio.h"
//#include "Scene.h"


// ============================================================
// CONSTRUCTOR
// ============================================================

Engine::Engine()
{
    LOG("Constructor Engine::Engine");


    // ========================================================
    // MODULES
    // ========================================================

    window = std::make_shared<Window>();
    input = std::make_shared<Input>();
    resourceManager = std::make_shared<ResourceManager>();
    render = std::make_shared<Render>();
    editor = std::make_shared<Editor>();

    /*
    textures = std::make_shared<Textures>();
    audio = std::make_shared<Audio>();
    scene = std::make_shared<Scene>();
    */


    // ========================================================
    // MODULE ORDER
    // ========================================================
    //
    // Awake / Start / Update se ejecutan en este orden.
    //
    // CleanUp se ejecuta en orden inverso.
    //
    // Window
    // Input
    // Assets
    // Render
    // Editor
    //
    // ========================================================

    AddModule(
        std::static_pointer_cast<Module>(window)
    );

    AddModule(
        std::static_pointer_cast<Module>(input)
    );

    AddModule(
        std::static_pointer_cast<Module>(resourceManager)
    );

    /*
    AddModule(
        std::static_pointer_cast<Module>(textures)
    );

    AddModule(
        std::static_pointer_cast<Module>(audio)
    );

    AddModule(
        std::static_pointer_cast<Module>(scene)
    );
    */


    // Render después de Assets
    AddModule(
        std::static_pointer_cast<Module>(render)
    );


    // Editor después de Render
    AddModule(
        std::static_pointer_cast<Module>(editor)
    );
}


// ============================================================
// SINGLETON
// ============================================================

Engine& Engine::GetInstance()
{
    static Engine instance;

    return instance;
}


// ============================================================
// ADD MODULE
// ============================================================

void Engine::AddModule(
    std::shared_ptr<Module> module
)
{
    module->Init();

    moduleList.push_back(module);
}


// ============================================================
// AWAKE
// ============================================================

bool Engine::Awake()
{
    LOG("Engine::Awake");

    bool result = true;


    for (const auto& module : moduleList)
    {
        result = module->Awake();

        if (!result)
        {
            break;
        }
    }


    return result;
}


// ============================================================
// START
// ============================================================

bool Engine::Start()
{
    LOG("Engine::Start");

    bool result = true;


    for (const auto& module : moduleList)
    {
        result = module->Start();

        if (!result)
        {
            break;
        }
    }


    return result;
}


// ============================================================
// UPDATE
// ============================================================

bool Engine::Update()
{
    bool ret = true;


    PrepareUpdate();


    // --------------------------------------------------------
    // Check quit event
    // --------------------------------------------------------

    if (input->GetWindowEvent(WE_QUIT) == true)
    {
        ret = false;
    }


    // --------------------------------------------------------
    // PreUpdate
    // --------------------------------------------------------

    if (ret == true)
    {
        ret = PreUpdate();
    }


    // --------------------------------------------------------
    // Update
    // --------------------------------------------------------

    if (ret == true)
    {
        ret = DoUpdate();
    }


    // --------------------------------------------------------
    // PostUpdate
    // --------------------------------------------------------

    if (ret == true)
    {
        ret = PostUpdate();
    }


    FinishUpdate();


    return ret;
}


// ============================================================
// CLEAN UP
// ============================================================

bool Engine::CleanUp()
{
    LOG("Engine::CleanUp");

    bool result = true;


    // ========================================================
    // IMPORTANT:
    // CleanUp in REVERSE order
    // ========================================================

    for (auto it = moduleList.rbegin();
        it != moduleList.rend();
        ++it)
    {
        result = (*it)->CleanUp();

        if (!result)
        {
            break;
        }
    }


    return result;
}


// ============================================================
// PREPARE UPDATE
// ============================================================

void Engine::PrepareUpdate()
{
}


// ============================================================
// FINISH UPDATE
// ============================================================

void Engine::FinishUpdate()
{
    window->SwapBuffers();
}


// ============================================================
// PRE UPDATE
// ============================================================

bool Engine::PreUpdate()
{
    bool result = true;


    for (const auto& module : moduleList)
    {
        result = module->PreUpdate();

        if (!result)
        {
            break;
        }
    }


    return result;
}


// ============================================================
// UPDATE
// ============================================================

bool Engine::DoUpdate()
{
    bool result = true;


    for (const auto& module : moduleList)
    {
        result = module->Update(dt);

        if (!result)
        {
            break;
        }
    }


    return result;
}


// ============================================================
// POST UPDATE
// ============================================================

bool Engine::PostUpdate()
{
    bool result = true;


    for (const auto& module : moduleList)
    {
        result = module->PostUpdate();

        if (!result)
        {
            break;
        }
    }


    return result;
}
