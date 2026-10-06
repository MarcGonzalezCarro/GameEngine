#pragma once

#include "Module.h"

#include <vector>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>

struct Texture
{
    int width = 0;
    int height = 0;
    int channels = 0;

    std::vector<unsigned char> pixels;
};

// ============================================================
// MESH
// Datos de geometría almacenados en CPU
// ============================================================

struct Mesh
{
    // --------------------------------------------------------
    // Vértices
    // --------------------------------------------------------

    GLuint num_vertices = 0;
    float* vertices = nullptr;


    // --------------------------------------------------------
    // Índices
    // --------------------------------------------------------

    GLuint num_indices = 0;
    GLuint* indices = nullptr;

    std::string diffuseTexture;

    // --------------------------------------------------------
    // Constructor
    // --------------------------------------------------------

    Mesh() = default;


    // --------------------------------------------------------
    // Destructor
    // --------------------------------------------------------

    ~Mesh()
    {
        delete[] vertices;
        delete[] indices;

        vertices = nullptr;
        indices = nullptr;
    }


    // --------------------------------------------------------
    // Evitar copias accidentales
    // --------------------------------------------------------

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;


    // --------------------------------------------------------
    // Permitir movimiento
    // --------------------------------------------------------

    Mesh(Mesh&& other) noexcept
    {
        num_vertices = other.num_vertices;
        vertices = other.vertices;

        num_indices = other.num_indices;
        indices = other.indices;

        diffuseTexture = std::move(other.diffuseTexture);

        other.num_vertices = 0;
        other.vertices = nullptr;

        other.num_indices = 0;
        other.indices = nullptr;
    }

    Mesh& operator=(Mesh&& other) noexcept
    {
        if (this != &other)
        {
            delete[] vertices;
            delete[] indices;

            num_vertices = other.num_vertices;
            vertices = other.vertices;

            num_indices = other.num_indices;
            indices = other.indices;

            diffuseTexture = std::move(other.diffuseTexture);

            other.num_vertices = 0;
            other.vertices = nullptr;

            other.num_indices = 0;
            other.indices = nullptr;
        }

        return *this;
    }
};


// ============================================================
// ASSETS
// ============================================================

class ResourceManager : public Module
{
public:

    // Constructor
    ResourceManager();

    // Destructor
    ~ResourceManager();


    // --------------------------------------------------------
    // Module lifecycle
    // --------------------------------------------------------

    bool Awake() override;

    bool Start() override;

    bool PreUpdate() override;

    bool Update(float dt) override;

    bool PostUpdate() override;

    bool CleanUp() override;


    // --------------------------------------------------------
    // MODEL LOADING
    // --------------------------------------------------------

    bool LoadModel(const char* file_path);


    // --------------------------------------------------------
    // ACCESS TO LOADED MESHES
    // --------------------------------------------------------

    const std::vector<Mesh>& GetMeshes() const;


    // --------------------------------------------------------
    // Clear current meshes
    // --------------------------------------------------------

    void ClearMeshes();


    bool LoadTexture(
        const char* file_path,
        Texture& texture
    );

private:

    // --------------------------------------------------------
    // Meshes currently loaded
    // --------------------------------------------------------

    std::vector<Mesh> meshes;
};
