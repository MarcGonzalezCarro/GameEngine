#include "Scene/Component.h"

Component::Component(GameObject* owner, ComponentType type)
    : type(type), owner(owner)
{
}

Component::~Component()
{
    if (enabled)
        Disable();
}

void Component::Enable()
{
}

void Component::Update()
{
}

void Component::Disable()
{
}

void Component::SetActive(bool value)
{
    active = value;
}

void Component::Tick(bool ownerActive)
{
    const bool shouldRun = active && ownerActive;

    if (shouldRun && !enabled)
    {
        Enable();
        enabled = true;
    }
    else if (!shouldRun && enabled)
    {
        Disable();
        enabled = false;
    }

    if (shouldRun)
        Update();
}

bool Component::IsActive() const
{
    return active;
}

ComponentType Component::GetType() const
{
    return type;
}

GameObject* Component::GetOwner() const
{
    return owner;
}