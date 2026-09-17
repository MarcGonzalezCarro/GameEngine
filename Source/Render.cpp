#include "Render.h"
#include "Log.h"
#include "Engine.h"
#include "Window.h"

// Constructor
Render::Render() : Module()
{
    name = "render";
}

// Destructor
Render::~Render()
{
}

// Called before render is available
bool Render::Awake()
{
    LOG("Render::Awake");

    LOG("Initializing OpenGL");

    bool ret = true;

    // Initialize GLAD
    int version = gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)
    );

    if (version == 0)
    {
        LOG("Error loading the GLAD library");
        ret = false;
    }
    else
    {
        LOG("GLAD initialized successfully");

        // Detect hardware and OpenGL capabilities
        LOG("Vendor: %s", glGetString(GL_VENDOR));
        LOG("Renderer: %s", glGetString(GL_RENDERER));
        LOG("OpenGL version supported: %s", glGetString(GL_VERSION));
        LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    // OpenGL configuration
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return ret;
}

// Called before the first frame
bool Render::Start()
{
    LOG("Render::Start");

    bool ret = true;

    return ret;
}

// Called before each loop iteration
bool Render::PreUpdate()
{
    bool ret = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return ret;
}

// Called each loop iteration
bool Render::Update(float dt)
{
    bool ret = true;

    return ret;
}

// Called after each loop iteration
bool Render::PostUpdate()
{
    bool ret = true;

    // 1. Dibujar geometría del nivel
    // App->level->Draw();

    // 2. Dibujar debug
    // if (debug_draw)
    // {
    //     BeginDebugDraw();
    //     App->DebugDraw();
    //     EndDebugDraw();
    // }

    // 3. Dibujar editor
    // App->editor->Draw();

    // 4. Presentar el frame

    Engine::GetInstance().window->SwapBuffers();

    return ret;
}

// Called before quitting
bool Render::CleanUp()
{
    LOG("Render::CleanUp");

    bool ret = true;

    return ret;
}