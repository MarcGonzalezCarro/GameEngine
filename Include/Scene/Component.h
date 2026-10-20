
#pragma once

class GameObject;

// ============================================================
// COMPONENT TYPE
// ============================================================

enum class ComponentType
{
    Transform,
    MeshRenderer,
    Camera,
    Unknown
};

// ============================================================
// COMPONENT
// Clase base de todos los componentes
// ============================================================

class Component
{
public:

    // Constructor / Destructor
    Component(GameObject* owner, ComponentType type);
    virtual ~Component();

    // Ciclo de vida
    virtual void Enable();
    virtual void Update();
    virtual void Disable();

    // Activación
    void SetActive(bool active);
    bool IsActive() const;

    // Información
    ComponentType GetType() const;
    GameObject* GetOwner() const;

    // Actualización controlada
    void Tick(bool ownerActive);

protected:

    ComponentType type = ComponentType::Unknown;

    bool active = true;
    bool enabled = false;

    GameObject* owner = nullptr;
};
