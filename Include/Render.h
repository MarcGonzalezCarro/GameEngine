#pragma once

#include "Module.h"
#include "ResourceManager.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


// ============================================================
// MESH GPU
// ============================================================

struct MeshGPU
{
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    unsigned int num_indices = 0;
};


// ============================================================
// RENDER
// ============================================================

class Render : public Module
{
public:

    Render();
    ~Render();

    bool Awake() override;
    bool Start() override;

    bool PreUpdate() override;
    bool Update(float dt) override;
    bool PostUpdate() override;

    bool CleanUp() override;


    // ========================================================
    // MESH
    // ========================================================

    MeshGPU UploadMesh(const Mesh& mesh);

    void DrawMesh(const MeshGPU& mesh);

    void DeleteMesh(MeshGPU& mesh);


    // ========================================================
    // OPENGL
    // ========================================================

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLuint shaderProgram = 0;
    GLuint textureID = 0;

    std::vector<MeshGPU> modelMeshes;

private:

    


    GLuint CompileShader(
        GLenum type,
        const char* source
    );

    std::string LoadShaderSource(
        const char* path
    );
};
