#include "Scene/GameObject.h"

GameObject::GameObject(
    const std::string& name,
    GameObject* parent
)
    : name(name), parent(parent)
{
}

Component& GameObject::CreateComponent(ComponentType type)
{
    // Evitamos crear dos Transform en el mismo objeto.
    if (type == ComponentType::Transform)
    {
        Component* existing = GetComponent(type);

        if (existing != nullptr)
            return *existing;
    }

    auto component = std::make_unique<Component>(
        this,
        type
    );

    Component& reference = *component;

    components.push_back(std::move(component));

    return reference;
}

GameObject& GameObject::CreateChild(
    const std::string& childName
)
{
    auto child = std::make_unique<GameObject>(
        childName,
        this
    );

    GameObject& reference = *child;

    children.push_back(std::move(child));

    return reference;
}

void GameObject::UpdateHierarchy(bool parentActive)
{
    const bool hierarchyActive = active && parentActive;

    for (auto& component : components)
    {
        component->Tick(hierarchyActive);
    }

    for (auto& child : children)
    {
        child->UpdateHierarchy(hierarchyActive);
    }
}

void GameObject::Update()
{
    UpdateHierarchy(true);
}

Component* GameObject::GetComponent(ComponentType type)
{
    for (auto& component : components)
    {
        if (component->GetType() == type)
            return component.get();
    }

    return nullptr;
}

void GameObject::SetActive(bool value)
{
    active = value;
}

bool GameObject::IsActive() const
{
    return active;
}

const std::string& GameObject::GetName() const
{
    return name;
}

GameObject* GameObject::GetParent() const
{
    return parent;
}

const std::vector<std::unique_ptr<GameObject>>&
GameObject::GetChildren() const
{
    return children;
}

const std::vector<std::unique_ptr<Component>>&
GameObject::GetComponents() const
{
    return components;
}