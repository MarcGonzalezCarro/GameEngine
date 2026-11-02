#pragma once

#include <glm/glm.hpp>
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

    // Orbit camera
    glm::vec3 orbitTarget = glm::vec3(0.0f);
    float orbitDistance = 5.0f;
    bool orbitEnabled = false;

    // Velocidad rápida con SHIFT
    float fastMovementMultiplier = 2.0f;

    Camera(
        glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f),
        float startYaw = -90.0f,
        float startPitch = 0.0f
    );

    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(
        const std::string& direction,
        float deltaTime,
        bool fast
    );

    bool IsOrbitEnabled() const;
    void LogState() const;

    void ProcessMousePan(float xoffset, float yoffset, float dt);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void UpdateCameraVectors();
    void ProcessMouseOrbit(float xoffset, float yoffset);

    void Focus(const glm::vec3& target, float distance = 5.0f);
    void ClearOrbitTarget();
    void UpdateOrbitPosition();
    void SetAspectRatio(float width, float height);
private:
    void updateCameraVectors();
};