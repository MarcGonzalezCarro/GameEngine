#pragma once

#include "Module.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>

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



    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint shaderProgram = 0;

private:

    GLuint CompileShader(GLenum type, const char* source);
    std::string LoadShaderSource(const char* path);

};