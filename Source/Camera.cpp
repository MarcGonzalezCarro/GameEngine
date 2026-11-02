#include "Camera.h"

#include "Log.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/common.hpp>

#include <cmath>

// ============================================================
// CONSTRUCTOR
// ============================================================

Camera::Camera(
    glm::vec3 startPosition,
    glm::vec3 startUp,
    float startYaw,
    float startPitch
)
    : position(startPosition),
    worldUp(startUp),
    yaw(startYaw),
    pitch(startPitch),
    movementSpeed(5.0f),
    mouseSensitivity(0.1f),
    fov(45.0f),
    aspectRatio(800.0f / 600.0f)
{
    updateCameraVectors();

    LOG(
        "Camera initialized at Pos: (%.2f, %.2f, %.2f) | Yaw: %.2f | Pitch: %.2f",
        position.x, position.y, position.z,
        yaw, pitch
    );
}

// ============================================================
// MATRICES
// ============================================================

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(
        glm::radians(zoom),
        aspectRatio,
        nearPlane,
        farPlane
    );
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(
        position,
        position + front,
        up
    );
}

// ============================================================
// MOVIMIENTO
// ============================================================

void Camera::ProcessKeyboard(
    const std::string& direction,
    float deltaTime,
    bool fast
)
{
    float speed = movementSpeed;

    if (fast)
        speed *= fastMovementMultiplier;

    const float velocity = speed * deltaTime;

    if (direction == "FORWARD")
        position += front * velocity;

    if (direction == "BACKWARD")
        position -= front * velocity;

    if (direction == "LEFT")
        position -= right * velocity;

    if (direction == "RIGHT")
        position += right * velocity;

    if (direction == "UP")
        position += worldUp * velocity;

    if (direction == "DOWN")
        position -= worldUp * velocity;
}

// ============================================================
// ESTADO
// ============================================================

bool Camera::IsOrbitEnabled() const
{
    return orbitEnabled;
}

void Camera::LogState() const
{
    LOG("=== CAMERA STATE ===");
    LOG("Position : (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    LOG("Front    : (%.2f, %.2f, %.2f)", front.x, front.y, front.z);
    LOG("Yaw/Pitch: Yaw = %.2f, Pitch = %.2f", yaw, pitch);
    LOG("FOV/Aspect: FOV = %.1f, AspectRatio = %.2f", fov, aspectRatio);
    LOG("====================");
}

// ============================================================
// VECTORES DE CÁMARA
// ============================================================

void Camera::updateCameraVectors()
{
    glm::vec3 newFront;

    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::SetAspectRatio(float width, float height)
{
    if (height <= 0.0f || width <= 0.0f)
        return;

    aspectRatio = width / height;
}