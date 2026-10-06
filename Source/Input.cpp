#include "Engine.h"
#include "Input.h"
#include "Window.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Render.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>

#define MAX_KEYS 300

Input::Input() : Module()
{
	name = "input";

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	memset(windowEvents, 0, sizeof(windowEvents));
	mouseMotionX = mouseMotionY = mouseX = mouseY = 0;
	mouseWheelY = 0;
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
}

// Called before render is available
bool Input::Awake()
{
	LOG("Init SDL input event system");
	bool ret = true;

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) != true)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Input::Start()
{
	SDL_StopTextInput(Engine::GetInstance().window->window);
	return true;
}

bool Input::PreUpdate()
{
    static SDL_Event event;

    // 1. REINICIAR MOVIMIENTO Y RUEDA EN CADA FRAME
    mouseMotionX = 0;
    mouseMotionY = 0;
    mouseWheelY = 0;

    // Actualizar estados de botones del ratón (DOWN -> REPEAT, UP -> IDLE)
    for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
    {
        if (mouseButtons[i] == KEY_DOWN)
            mouseButtons[i] = KEY_REPEAT;
        else if (mouseButtons[i] == KEY_UP)
            mouseButtons[i] = KEY_IDLE;
    }

    // Poll events
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            windowEvents[WE_QUIT] = true;
            break;
        case SDL_EVENT_DROP_FILE:
        {
            if (event.drop.data == nullptr)
                break;


            std::string droppedPath =
                event.drop.data;

            LOG(
                "Dropped file: %s",
                droppedPath.c_str()
            );

            std::string extension;

            size_t dot =
                droppedPath.find_last_of('.');

            if (dot != std::string::npos)
            {
                extension =
                    droppedPath.substr(dot);

                std::transform(
                    extension.begin(),
                    extension.end(),
                    extension.begin(),
                    [](unsigned char c)
                    {
                        return static_cast<char>(
                            std::tolower(c)
                            );
                    }
                );
            }


            std::transform(
                extension.begin(),
                extension.end(),
                extension.begin(),
                [](unsigned char c)
                {
                    return static_cast<char>(
                        std::tolower(c)
                        );
                }
            );


            auto& engine =
                Engine::GetInstance();


            // ========================================================
            // MODEL
            // ========================================================

            if (extension == ".fbx" ||
                extension == ".obj" ||
                extension == ".gltf" ||
                extension == ".glb")
            {
                if (!engine.resourceManager->LoadModel(
                    droppedPath.c_str()))
                {
                    LOG(
                        "Failed to load model: %s",
                        droppedPath.c_str()
                    );

                    break;
                }


                // Eliminar modelo anterior
                for (MeshGPU& gpuMesh :
                    engine.render->modelMeshes)
                {
                    engine.render->DeleteMesh(gpuMesh);
                }

                engine.render->modelMeshes.clear();


                // Subir nuevo modelo
                const std::vector<Mesh>& meshes =
                    engine.resourceManager->GetMeshes();


                for (const Mesh& mesh : meshes)
                {
                    MeshGPU gpuMesh =
                        engine.render->UploadMesh(mesh);

                    engine.render->modelMeshes.push_back(
                        gpuMesh
                    );
                }


                LOG(
                    "Model loaded successfully: %s",
                    droppedPath.c_str()
                );
            }


            // ========================================================
            // TEXTURE
            // ========================================================

            else if (extension == ".png" ||
                extension == ".jpg" ||
                extension == ".jpeg" ||
                extension == ".tga" ||
                extension == ".bmp")
            {
                Texture texture;


                if (!engine.resourceManager->LoadTexture(
                    droppedPath.c_str(),
                    texture))
                {
                    LOG(
                        "Failed to load texture: %s",
                        droppedPath.c_str()
                    );

                    break;
                }


                GLuint textureID =
                    engine.render->UploadTexture(texture);


                LOG(
                    "Texture loaded successfully: %s (ID: %u)",
                    droppedPath.c_str(),
                    textureID
                );
            }


            // ========================================================
            // UNKNOWN
            // ========================================================

            else
            {
                LOG(
                    "Unsupported file type: %s",
                    droppedPath.c_str()
                );
            }


            break;
        }

        case SDL_EVENT_WINDOW_HIDDEN:
        case SDL_EVENT_WINDOW_MINIMIZED:
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            windowEvents[WE_HIDE] = true;
            break;

        case SDL_EVENT_WINDOW_SHOWN:
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        case SDL_EVENT_WINDOW_MAXIMIZED:
        case SDL_EVENT_WINDOW_RESTORED:
            windowEvents[WE_SHOW] = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button >= 1 && event.button.button <= NUM_MOUSE_BUTTONS)
                mouseButtons[event.button.button - 1] = KEY_DOWN;
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button >= 1 && event.button.button <= NUM_MOUSE_BUTTONS)
                mouseButtons[event.button.button - 1] = KEY_UP;
            break;

        case SDL_EVENT_MOUSE_MOTION:
        {
            int scale = Engine::GetInstance().window->GetScale();
            if (scale <= 0) scale = 1;

            mouseMotionX = (int)(event.motion.xrel / scale);
            mouseMotionY = (int)(event.motion.yrel / scale);
            mouseX = (int)(event.motion.x / scale);
            mouseY = (int)(event.motion.y / scale);
        }
        break;

        case SDL_EVENT_MOUSE_WHEEL:
            mouseWheelY = (int)event.wheel.y;
            break;
        }

    }

    return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

// 3. NUEVO MÉTODO PARA LEER LA RUEDA
int Input::GetMouseWheel()
{
	return mouseWheelY;
}