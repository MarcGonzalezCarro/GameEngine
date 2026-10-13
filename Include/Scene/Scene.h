#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Module.h"
#include "GameObject.h"

class Scene : public Module
{
public:
    Scene();
    ~Scene();

    bool Awake() override;
    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    GameObject& CreateGameObject(
        const std::string& name,
        GameObject* parent = nullptr
    );

    void Clear();

    const std::vector<std::unique_ptr<GameObject>>&
        GetRootObjects() const;

private:
    std::vector<std::unique_ptr<GameObject>> rootObjects;
};