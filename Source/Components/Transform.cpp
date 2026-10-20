
#include "Components/Transform.h"
#include "Scene/GameObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform(GameObject* owner)
    : Component(owner, ComponentType::Transform)
{
}

const glm::vec3& Transform::GetPosition() const
{
    return position;
}

const glm::vec3& Transform::GetRotation() const
{
    return rotation;
}

const glm::vec3& Transform::GetScale() const
{
    return scale;
}

void Transform::SetPosition(const glm::vec3& value)
{
    position = value;
}

void Transform::SetRotation(const glm::vec3& degrees)
{
    rotation = degrees;
}

void Transform::SetScale(const glm::vec3& value)
{
    scale = value;
}

glm::mat4 Transform::GetLocalMatrix() const
{
    const glm::mat4 translation =
        glm::translate(glm::mat4(1.0f), position);

    const glm::mat4 rotationMatrix =
        glm::mat4_cast(glm::quat(glm::radians(rotation)));

    const glm::mat4 scaleMatrix =
        glm::scale(glm::mat4(1.0f), scale);

    return translation * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::GetWorldMatrix() const
{
    const glm::mat4 local = GetLocalMatrix();

    if (owner == nullptr || owner->GetParent() == nullptr)
        return local;

    Component* component =
        owner->GetParent()->GetComponent(ComponentType::Transform);

    if (component == nullptr)
        return local;

    const auto* parentTransform =
        static_cast<const Transform*>(component);

    return parentTransform->GetWorldMatrix() * local;
}
