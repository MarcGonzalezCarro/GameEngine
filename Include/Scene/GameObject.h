
#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Scene/Component.h"

class Transform;
class MeshRenderer;

// ============================================================
// GAME OBJECT
// ============================================================

class GameObject
{
public:

    // Constructor / Destructor
    explicit GameObject(
        const std::string& name,
        GameObject* parent = nullptr
    );

    ~GameObject() = default;

    // Actualización
    void Update();
    void UpdateHierarchy(bool parentActive);

    // Creación
    Component& CreateComponent(ComponentType type);

    GameObject& CreateChild(const std::string& name);

    // Información
    const std::string& GetName() const;
    GameObject* GetParent() const;

    const std::vector<std::unique_ptr<GameObject>>&
        GetChildren() const;

    const std::vector<std::unique_ptr<Component>>&
        GetComponents() const;

    // Búsqueda de componentes
    Component* GetComponent(ComponentType type);

    // Activación
    void SetActive(bool active);
    bool IsActive() const;

private:

    std::string name;

    bool active = true;

    GameObject* parent = nullptr;

    std::vector<std::unique_ptr<Component>> components;

    std::vector<std::unique_ptr<GameObject>> children;
};
