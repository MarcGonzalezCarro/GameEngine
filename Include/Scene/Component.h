#pragma once

#include <string>

class GameObject;

enum class ComponentType
{
    Transform,
    MeshRenderer,
    Camera,
    Unknown
};

class Component
{
public:
    Component(GameObject* owner, ComponentType type);
    virtual ~Component();

    virtual void Enable();
    virtual void Update();
    virtual void Disable();

    void SetActive(bool active);

    bool IsActive() const;
    ComponentType GetType() const;
    GameObject* GetOwner() const;

    void Tick(bool ownerActive);

protected:
    ComponentType type = ComponentType::Unknown;
    bool active = true;
    bool enabled = false;

    GameObject* owner = nullptr;
};