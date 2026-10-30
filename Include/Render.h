#pragma once

#include "Module.h"
#include "ResourceManager.h"
#include "Scene/GameObject.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
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

    GLuint textureID = 0;

    glm::vec4 diffuseColor = glm::vec4(1.0f);

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

    void CreateGrid();

    GLuint GetSceneTexture() const
    {
        return sceneTexture;
    }

    void SetSceneViewportSize(int width, int height);
    // ========================================================
    // MESH
    // ========================================================

    MeshGPU UploadMesh(const Mesh& mesh);

    void DrawMesh(const MeshGPU& mesh);

    void DeleteMesh(MeshGPU& mesh);

    GLuint UploadTexture(const Texture& texture);


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

    GLuint gridVAO = 0;
    GLuint gridVBO = 0;
    GLuint gridShaderProgram = 0;

    GLsizei gridVertexCount = 0;


    GLuint CompileShader(
        GLenum type,
        const char* source
    );

    std::string LoadShaderSource(
        const char* path
    );

    void DrawGrid();

    void CreateMeshObjects(GameObject& parent, std::size_t firstMesh, std::size_t meshCount);

    // Framebuffer del Scene Viewport
    GLuint sceneFBO = 0;
    GLuint sceneTexture = 0;
    GLuint sceneDepthRBO = 0;

    int sceneWidth = 1;
    int sceneHeight = 1;

    bool CreateSceneFramebuffer(int width, int height);
    void ResizeSceneFramebuffer(int width, int height);
    void BindSceneFramebuffer();
    void UnbindSceneFramebuffer();

    
    
};
