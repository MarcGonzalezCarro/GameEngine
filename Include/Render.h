#pragma once

#include "Module.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
class Render : public Module
{
public:

    // Constructor
    Render();

    // Destructor
    ~Render();

    // Called before render is available
    bool Awake() override;

    // Called before the first frame
    bool Start() override;

    // Called each loop iteration
    bool PreUpdate() override;

    // Called each loop iteration
    bool Update(float dt) override;

    // Called each loop iteration
    bool PostUpdate() override;

    // Called before quitting
    bool CleanUp() override;
};