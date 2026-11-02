#pragma once

#include "Module.h"
#include "ResourceManager.h"
#include "Scene/GameObject.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <unordered_map>

#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Camera;

// ============================================================
// MESH GPU
// ============================================================

struct MeshGPU
{
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLuint textureID = 0;

    int textureWidth = 0;
    int textureHeight = 0;

    glm::vec4 diffuseColor = glm::vec4(1.0f);

    unsigned int num_indices = 0;

    // Normales de las caras
    GLuint faceNormalVAO = 0;
    GLuint faceNormalVBO = 0;
    GLsizei faceNormalVertexCount = 0;

    // Normales de los vértices
    GLuint vertexNormalVAO = 0;
    GLuint vertexNormalVBO = 0;
    GLsizei vertexNormalVertexCount = 0;
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

    GLuint GetTexturePreview(const std::string& path);
    // ========================================================
    // MESH
    // ========================================================

    MeshGPU UploadMesh(const Mesh& mesh);

    void DrawMesh(const MeshGPU& mesh);

    void DeleteMesh(MeshGPU& mesh);

    GLuint UploadTexture(const Texture& texture);

    GLuint GetCheckerboardTexture() const
    {
        return checkerboardTexture;
    }

    bool GetOverrideTextureInfo(
        const std::string& path,
        int& width,
        int& height
    ) const;

    // Obtiene una textura de la caché o la carga si hace falta.
    GLuint GetOrLoadOverrideTexture(const std::string& path);

    // ========================================================
    // NORMAL VISUALIZATION
    // ========================================================

    bool showFaceNormals = false;
    bool showVertexNormals = false;

    float normalLength = 0.5f;

    glm::vec3 normalColor = glm::vec3(
        0.0f, 1.0f, 0.0f
    );


    // ========================================================
    // OPENGL
    // ========================================================

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLuint shaderProgram = 0;
    GLuint textureID = 0;

    std::vector<MeshGPU> modelMeshes;

    void SetCamera(const Camera* camera);
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

    // ========================================================
    // TEXTURAS ASIGNADAS MANUALMENTE
    // ========================================================

    GLuint checkerboardTexture = 0;

    struct TextureOverride
    {
        GLuint textureID = 0;
        int width = 0;
        int height = 0;
    };

    std::unordered_map<std::string, TextureOverride> textureOverrides;

    const Camera* sceneCamera = nullptr;

    void DrawGrid(const Camera& camera);
    
    // ========================================================
    // NORMALS SHADER
    // ========================================================

    GLuint normalShaderProgram = 0;

    GLuint CompileNormalShader();
    void DrawNormals(
        const MeshGPU& mesh,
        const glm::mat4& viewProjection,
        const glm::mat4& model,
        bool faceNormals
    );

    void CreateCheckerboardTexture();
    
};
