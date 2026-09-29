#include "Editor.h"
#include "Engine.h"
#include "Window.h"
#include <SDL3/SDL.h>
#include "Input.h"

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
    return true;
}

bool Editor::Start()
{
    LOG("Editor::Start");

    int width = 0, height = 0;
    if (Engine::GetInstance().window != nullptr)
    {
        SDL_GetWindowSize(Engine::GetInstance().window->window, &width, &height);
        if (height > 0)
        {
            camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
            LOG("Camera AspectRatio updated to %.2f (%dx%d)", camera.aspectRatio, width, height);
        }
    }

    return true;
}

bool Editor::PreUpdate()
{
    return true;
}

bool Editor::Update(float dt)
{
    auto input = Engine::GetInstance().input;

    // ========================================================
    // 1. TECLADO (WASD / Q / E / Shift)
    // ========================================================
    const bool* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_W]) camera.ProcessKeyboard("FORWARD", dt);
    if (keys[SDL_SCANCODE_S]) camera.ProcessKeyboard("BACKWARD", dt);
    if (keys[SDL_SCANCODE_A]) camera.ProcessKeyboard("LEFT", dt);
    if (keys[SDL_SCANCODE_D]) camera.ProcessKeyboard("RIGHT", dt);
    if (keys[SDL_SCANCODE_E] || keys[SDL_SCANCODE_SPACE]) camera.ProcessKeyboard("UP", dt);
    if (keys[SDL_SCANCODE_Q] || keys[SDL_SCANCODE_LSHIFT]) camera.ProcessKeyboard("DOWN", dt);

    // ========================================================
    // 2. ZOOM (RUEDA DEL RATÓN)
    // ========================================================
    int wheel = input->GetMouseWheel();
    if (wheel != 0)
    {
        camera.ProcessMouseScroll((float)wheel);
    }

    // ========================================================
    // 3. MOVIMIENTO Y BOTONES DEL RATÓN
    // ========================================================
    int mouseRelX = 0, mouseRelY = 0;
    input->GetMouseMotion(mouseRelX, mouseRelY);

    float xoffset = (float)mouseRelX;
    float yoffset = -(float)mouseRelY; // Invertir Y para coordenadas OpenGL

    // Lectura directa de estados mediante SDL_GetMouseState (100% fiable)
    SDL_MouseButtonFlags mouseButtons = SDL_GetMouseState(nullptr, nullptr);

    bool rightClick = (mouseButtons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) != 0;
    bool leftClick = (mouseButtons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
    bool middleClick = (mouseButtons & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) != 0;
    bool isAltPressed = keys[SDL_SCANCODE_LALT] || keys[SDL_SCANCODE_RALT];

    // A) ROTACIÓN DE CÁMARA: Click Derecho + Arrastrar
    if (rightClick && (xoffset != 0.0f || yoffset != 0.0f))
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    // B) PAN SCREEN (Mover plano): Alt + Click Izquierdo OR Click Central + Arrastrar
    else if (((isAltPressed && leftClick) || middleClick) && (xoffset != 0.0f || yoffset != 0.0f))
    {
        camera.ProcessMousePan(xoffset, yoffset, dt);
    }

    return true;
}

bool Editor::PostUpdate()
{
    return true;
}

bool Editor::CleanUp()
{
    LOG("Editor::CleanUp");
    return true;
}

// ============================================================
// FUNCIONES DE LA CÁMARA
// ============================================================

void Camera::ProcessMousePan(float xoffset, float yoffset, float dt)
{
    float panSpeed = 5.0f * dt;

    // Mueve la posición de la cámara a lo largo de los vectores Right y Up
    position -= right * (xoffset * panSpeed);
    position -= up * (yoffset * panSpeed);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    float zoomSpeed = 2.0f;
    position += front * (yoffset * zoomSpeed);
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}