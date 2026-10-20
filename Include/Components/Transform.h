#pragma once

#include "Scene/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

class Transform final : public Component
{
public:
    explicit Transform(GameObject* owner);

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;

    void SetPosition(const glm::vec3& value);
    void SetRotation(const glm::vec3& degrees);
    void SetScale(const glm::vec3& value);

    glm::mat4 GetLocalMatrix() const;
    glm::mat4 GetWorldMatrix() const;

private:
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f }; 
    glm::vec3 scale{ 1.0f };
};