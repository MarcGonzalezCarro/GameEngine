
#include "Scene/Component.h"

// ============================================================
// CONSTRUCTOR
// ============================================================

Component::Component(GameObject* owner, ComponentType type)
    : type(type), owner(owner)
{
}

// ============================================================
// DESTRUCTOR
// ============================================================

Component::~Component()
{
    if (enabled)
    {
        Disable();
        enabled = false;
    }
}

// ============================================================
// ENABLE
// ============================================================

void Component::Enable()
{
}

// ============================================================
// UPDATE
// ============================================================

void Component::Update()
{
}

// ============================================================
// DISABLE
// ============================================================

void Component::Disable()
{
}

// ============================================================
// SET ACTIVE
// ============================================================

void Component::SetActive(bool value)
{
    active = value;
}

// ============================================================
// IS ACTIVE
// ============================================================

bool Component::IsActive() const
{
    return active;
}

// ============================================================
// GET TYPE
// ============================================================

ComponentType Component::GetType() const
{
    return type;
}

// ============================================================
// GET OWNER
// ============================================================

GameObject* Component::GetOwner() const
{
    return owner;
}

// ============================================================
// TICK
// Gestiona el ciclo de vida del componente
// ============================================================

void Component::Tick(bool ownerActive)
{
    const bool shouldRun = active && ownerActive;

    if (shouldRun && !enabled)
    {
        enabled = true;
        Enable();
    }
    else if (!shouldRun && enabled)
    {
        enabled = false;
        Disable();
    }

    if (shouldRun)
    {
        Update();
    }
}
