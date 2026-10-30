
#include "Scene/GameObject.h"

#include "Components/Transform.h"
#include "Components/MeshRenderer.h"

// ============================================================
// CONSTRUCTOR
// ============================================================

GameObject::GameObject(
    const std::string& name,
    GameObject* parent
)
    : name(name), parent(parent)
{
    
    CreateComponent(ComponentType::Transform);
}

// ============================================================
// CREATE COMPONENT
// ============================================================

Component& GameObject::CreateComponent(ComponentType type)
{
    // Evitar componentes únicos duplicados.
    if (type == ComponentType::Transform || type == ComponentType::MeshRenderer)
    {
        Component* existing = GetComponent(type);

        if (existing != nullptr)
        {
            return *existing;
        }
    }

    std::unique_ptr<Component> component;

    switch (type)
    {
    case ComponentType::Transform:

        component = std::make_unique<Transform>(this);
        break;

    case ComponentType::MeshRenderer:

        component = std::make_unique<MeshRenderer>(this);
        break;

    case ComponentType::Camera:

        
        return CreateComponent(ComponentType::Unknown);

    case ComponentType::Unknown:

    default:

        
        return *components.front();
    }

    Component& reference = *component;

    components.push_back(std::move(component));

    return reference;
}

// ============================================================
// CREATE CHILD
// ============================================================

GameObject& GameObject::CreateChild(
    const std::string& childName
)
{
    auto child = std::make_unique<GameObject>(childName, this);

    GameObject& reference = *child;

    children.push_back(std::move(child));

    return reference;
}

// ============================================================
// UPDATE HIERARCHY
// ============================================================

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

// ============================================================
// UPDATE
// ============================================================

void GameObject::Update()
{
    UpdateHierarchy(true);
}

// ============================================================
// GET COMPONENT
// ============================================================

Component* GameObject::GetComponent(ComponentType type)
{
    for (auto& component : components)
    {
        if (component->GetType() == type)
        {
            return component.get();
        }
    }

    return nullptr;
}

// ============================================================
// SET ACTIVE
// ============================================================

void GameObject::SetActive(bool value)
{
    active = value;
}

// ============================================================
// IS ACTIVE
// ============================================================

bool GameObject::IsActive() const
{
    return active;
}

// ============================================================
// GET NAME
// ============================================================

const std::string& GameObject::GetName() const
{
    return name;
}

// ============================================================
// GET PARENT
// ============================================================

GameObject* GameObject::GetParent() const
{
    return parent;
}

// ============================================================
// GET CHILDREN
// ============================================================

const std::vector<std::unique_ptr<GameObject>>&
GameObject::GetChildren() const
{
    return children;
}

// ============================================================
// GET COMPONENTS
// ============================================================

const std::vector<std::unique_ptr<Component>>&
GameObject::GetComponents() const
{
    return components;
}
