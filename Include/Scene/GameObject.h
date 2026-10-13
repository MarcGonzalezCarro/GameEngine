#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Component.h"

class GameObject
{
public:
    explicit GameObject(
        const std::string& name,
        GameObject* parent = nullptr
    );

    ~GameObject() = default;

    void Update();

    Component& CreateComponent(ComponentType type);

    GameObject& CreateChild(const std::string& name);

    void UpdateHierarchy(bool parentActive);

    const std::string& GetName() const;
    GameObject* GetParent() const;

    const std::vector<std::unique_ptr<GameObject>>&
        GetChildren() const;

    const std::vector<std::unique_ptr<Component>>&
        GetComponents() const;

    Component* GetComponent(ComponentType type);

    void SetActive(bool active);
    bool IsActive() const;

private:
    std::string name;

    bool active = true;

    GameObject* parent = nullptr;

    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<GameObject>> children;
};