#pragma once

#include "Module.h"
#include "Log.h" // Incluimos tu sistema de logs

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

// ============================================================
// CLASE CAMERA
// ============================================================
class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float fov;
    float aspectRatio;
    float zoom = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f),
        float startYaw = -90.0f,
        float startPitch = 0.0f)
        : position(startPosition), worldUp(startUp), yaw(startYaw), pitch(startPitch),
        movementSpeed(5.0f), mouseSensitivity(0.1f), fov(45.0f), aspectRatio(800.0f / 600.0f)
    {
        updateCameraVectors();
        LOG("Camera initialized at Pos: (%.2f, %.2f, %.2f) | Yaw: %.2f | Pitch: %.2f",
            position.x, position.y, position.z, yaw, pitch);
    }

    // Método para generar la matriz de proyección actualizada con el Zoom/FOV actual
    glm::mat4 GetProjectionMatrix() const
    {
        return glm::perspective(glm::radians(zoom), aspectRatio, nearPlane, farPlane);
    }

    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    void ProcessKeyboard(const std::string& direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        glm::vec3 oldPos = position;

        if (direction == "FORWARD")  position += front * velocity;
        if (direction == "BACKWARD") position -= front * velocity;
        if (direction == "LEFT")     position -= right * velocity;
        if (direction == "RIGHT")    position += right * velocity;
        if (direction == "UP")       position += worldUp * velocity;
        if (direction == "DOWN")     position -= worldUp * velocity;

        LOG("Camera Moving [%s] -> Pos: (%.2f, %.2f, %.2f)",
            direction.c_str(), position.x, position.y, position.z);
    }

    

    // Método de utilidad para imprimir el estado actual en cualquier momento
    void LogState() const
    {
        LOG("=== CAMERA STATE ===");
        LOG("Position : (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
        LOG("Front    : (%.2f, %.2f, %.2f)", front.x, front.y, front.z);
        LOG("Yaw/Pitch: Yaw = %.2f, Pitch = %.2f", yaw, pitch);
        LOG("FOV/Aspect: FOV = %.1f, AspectRatio = %.2f", fov, aspectRatio);
        LOG("====================");
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
public:
    void ProcessMousePan(float xoffset, float yoffset, float dt);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void UpdateCameraVectors();
};


// ============================================================
// MÓDULO EDITOR
// ============================================================
class Editor : public Module
{
public:
    Editor();
    ~Editor();

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update(float dt) override;
    bool PostUpdate() override;
    bool CleanUp() override;

public:
    Camera camera;
};