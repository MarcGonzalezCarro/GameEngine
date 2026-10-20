
#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Module.h"
#include "Scene/GameObject.h"

// ============================================================
// SCENE
// ============================================================

class Scene : public Module
{
public:

    Scene();
    ~Scene();

    // Ciclo de vida
    bool Awake() override;
    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    // Creación de objetos
    GameObject& CreateGameObject(
        const std::string& name,
        GameObject* parent = nullptr
    );

    // Gestión de la escena
    void Clear();

    const std::vector<std::unique_ptr<GameObject>>&
        GetRootObjects() const;

private:

    // Objetos raíz de la escena
    std::vector<std::unique_ptr<GameObject>> rootObjects;
};
