#pragma once

#include "Module.h"

class Editor : public Module
{
public:
    Editor();
    ~Editor();

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update(float dt) override;
    bool PostUpdate() override;
    bool CleanUp() override;
};