
#include "Scene/GameObject.h"

#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/TextureComponent.h"

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
    CreateComponent(ComponentType::Texture);
}

// ============================================================
// CREATE COMPONENT
// ============================================================

Component& GameObject::CreateComponent(ComponentType type)
{
    // Evitar componentes únicos duplicados.
    if (type == ComponentType::Transform || type == ComponentType::MeshRenderer || type == ComponentType::Texture)
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

    case ComponentType::Texture:

        component = std::make_unique<TextureComponent>(this);
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

// ============================================================
// BORRAR HIJOS
// ============================================================

bool GameObject::RemoveChild(GameObject* child)
{
    if (child == nullptr)
        return false;

    auto it = std::find_if(
        children.begin(),
        children.end(),
        [child](const std::unique_ptr<GameObject>& object)
        {
            return object.get() == child;
        }
    );

    if (it == children.end())
        return false;

    children.erase(it);

    return true;
}

// ============================================================
// DUPLICAR GAMEOBJECTS
// ============================================================
GameObject* GameObject::Duplicate(GameObject* newParent)
{
    GameObject* destinationParent =
        newParent ? newParent : parent;

    // No podemos duplicar un objeto raíz desde GameObject.
    if (destinationParent == nullptr)
        return nullptr;

    GameObject& duplicate =
        destinationParent->CreateChild(name + " Copy");

    // Copiar componentes
    CopyComponentsTo(duplicate);

    // Duplicar hijos recursivamente
    for (const auto& child : children)
    {
        child->Duplicate(&duplicate);
    }

    return &duplicate;
}

void GameObject::CopyComponentsTo(GameObject& target)
{
    for (const auto& component : components)
    {
        switch (component->GetType())
        {
        case ComponentType::Transform:
        {
            auto* source =
                static_cast<Transform*>(component.get());

            auto* destination =
                static_cast<Transform*>(
                    target.GetComponent(ComponentType::Transform)
                    );

            if (destination != nullptr)
            {
                destination->SetPosition(source->GetPosition());
                destination->SetRotation(source->GetRotation());
                destination->SetScale(source->GetScale());
            }

            break;
        }

        case ComponentType::MeshRenderer:
        {
            auto* source =
                static_cast<MeshRenderer*>(component.get());

            auto& newComponent =
                target.CreateComponent(ComponentType::MeshRenderer);

            auto* destination =
                static_cast<MeshRenderer*>(&newComponent);

            destination->SetMeshIndex(source->GetMeshIndex());

            break;
        }

        default:
            break;
        }
    }
}

void GameObject::SetName(const std::string& newName)
{
    if (!newName.empty())
    {
        name = newName;
    }
}

// ============================================================
// DETACH CHILD
// ============================================================

std::unique_ptr<GameObject> GameObject::DetachChild(GameObject* child)
{
    if (child == nullptr)
        return nullptr;

    auto it = std::find_if(
        children.begin(),
        children.end(),
        [child](const std::unique_ptr<GameObject>& object)
        {
            return object.get() == child;
        }
    );

    if (it == children.end())
        return nullptr;

    std::unique_ptr<GameObject> detached = std::move(*it);

    children.erase(it);

    detached->parent = nullptr;

    return detached;
}

// ============================================================
// ADD CHILD
// ============================================================

bool GameObject::AddChild(std::unique_ptr<GameObject> child)
{
    if (!child || child.get() == this)
        return false;

    if (IsDescendantOf(child.get()))
        return false;

    child->parent = this;

    children.push_back(std::move(child));

    return true;
}

// ============================================================
// IS DESCENDANT OF
// ============================================================

bool GameObject::IsDescendantOf(GameObject* object) const
{
    GameObject* current = parent;

    while (current != nullptr)
    {
        if (current == object)
            return true;

        current = current->parent;
    }

    return false;
}