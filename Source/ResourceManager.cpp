// ============================================================
// INCLUDES
// ============================================================

#include "ResourceManager.h"
#include "Log.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Components/MeshRenderer.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <glm/ext/vector_float4.hpp>

// ============================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================

// Constructor
ResourceManager::ResourceManager() : Module()
{
    name = "ResourceManager";
}


// Destructor
ResourceManager::~ResourceManager()
{
}


// ============================================================
// AWAKE
// Inicialización del módulo Assets
// ============================================================

bool ResourceManager::Awake()
{
    LOG("ResourceManager::Awake");

    ilInit();
    iluInit();

    ILenum error = ilGetError();

    if (error != IL_NO_ERROR)
    {
        LOG(
            "ERROR: DevIL initialization failed: %d",
            error
        );

        return false;
    }

    LOG("DevIL initialized successfully");

    return true;
}


// ============================================================
// START
// ============================================================

// Called before the first frame
bool ResourceManager::Start()
{
    LOG("Assets::Start");

    bool ret = true;

    return ret;
}


// ============================================================
// PRE UPDATE
// ============================================================

bool ResourceManager::PreUpdate()
{
    bool ret = true;

    return ret;
}


// ============================================================
// UPDATE
// ============================================================

bool ResourceManager::Update(float dt)
{
    bool ret = true;

    return ret;
}


// ============================================================
// POST UPDATE
// ============================================================

bool ResourceManager::PostUpdate()
{
    bool ret = true;

    return ret;
}


// ============================================================
// LOAD MODEL
// Carga un fichero mediante Assimp
// ============================================================

bool ResourceManager::LoadModel(const char* file_path)
{
    LOG("Loading model: %s", file_path);

    std::string modelPath = file_path;

    size_t lastSlash = modelPath.find_last_of("/\\");

    std::string modelDirectory;

    if (lastSlash != std::string::npos)
    {
        modelDirectory = modelPath.substr(0, lastSlash + 1);
    }

    // ========================================================
    // IMPORTAR ESCENA
    // ========================================================

    const aiScene* scene = aiImportFile(
        file_path,
        aiProcessPreset_TargetRealtime_MaxQuality |
        aiProcess_Triangulate
    );

    if (scene == nullptr)
    {
        LOG(
            "Error loading scene %s: %s",
            file_path,
            aiGetErrorString()
        );

        return false;
    }

    if (!scene->HasMeshes())
    {
        LOG("Scene contains no meshes: %s", file_path);

        aiReleaseImport(scene);
        return false;
    }

    auto engineScene = Engine::GetInstance().scene;

    if (!engineScene)
    {
        LOG("ERROR: No active scene available");

        aiReleaseImport(scene);
        return false;
    }
    
    std::string modelName = modelPath.substr(lastSlash + 1);

    const size_t dot = modelName.find_last_of('.');

    if (dot != std::string::npos)
    {
        modelName = modelName.substr(0, dot);
    }

    GameObject& modelObject =
        engineScene->CreateGameObject(modelName);

    // ========================================================
    // COMPROBAR RESULTADO
    // ========================================================

    if (scene == nullptr)
    {
        LOG(
            "Error loading scene %s: %s",
            file_path,
            aiGetErrorString()
        );

        return false;
    }


    if (!scene->HasMeshes())
    {
        LOG(
            "Error: scene %s does not contain meshes",
            file_path
        );

        aiReleaseImport(scene);

        return false;
    }


    LOG(
        "Scene loaded successfully: %s",
        file_path
    );

    LOG(
        "Number of meshes: %d",
        scene->mNumMeshes
    );


    // ========================================================
    // ITERAR MESHES
    // ========================================================

    for (GLuint i = 0; i < scene->mNumMeshes; ++i)
    {
        const aiMesh* ai_mesh = scene->mMeshes[i];



        if (ai_mesh == nullptr)
        {
            LOG(
                "WARNING: Mesh %d is null",
                i
            );

            continue;
        }


        Mesh mesh;


        // ====================================================
        // VÉRTICES
        // ====================================================

        mesh.num_vertices = ai_mesh->mNumVertices;

        if (mesh.num_vertices > 0)
        {
            mesh.vertices =
                new float[mesh.num_vertices * 5];

            for (GLuint v = 0; v < mesh.num_vertices; ++v)
            {
                mesh.vertices[v * 5 + 0] =
                    ai_mesh->mVertices[v].x;

                mesh.vertices[v * 5 + 1] =
                    ai_mesh->mVertices[v].y;

                mesh.vertices[v * 5 + 2] =
                    ai_mesh->mVertices[v].z;

                if (ai_mesh->HasTextureCoords(0))
                {
                    mesh.vertices[v * 5 + 3] =
                        ai_mesh->mTextureCoords[0][v].x;

                    mesh.vertices[v * 5 + 4] =
                        ai_mesh->mTextureCoords[0][v].y;
                }
                else
                {
                    mesh.vertices[v * 5 + 3] = 0.0f;
                    mesh.vertices[v * 5 + 4] = 0.0f;
                }
            }

            
        }


        LOG(
            "New mesh %d with %d vertices",
            i,
            mesh.num_vertices
        );


        // ====================================================
        // ÍNDICES
        // ====================================================

        if (ai_mesh->HasFaces())
        {
            mesh.num_indices =
                ai_mesh->mNumFaces * 3;

            mesh.indices =
                new GLuint[mesh.num_indices];


            for (GLuint j = 0;
                j < ai_mesh->mNumFaces;
                ++j)
            {
                const aiFace& face =
                    ai_mesh->mFaces[j];


                if (face.mNumIndices != 3)
                {
                    LOG(
                        "WARNING: Mesh %d face %d has %d indices",
                        i,
                        j,
                        face.mNumIndices
                    );

                    // Evitamos dejar basura en el buffer
                    mesh.indices[j * 3 + 0] = 0;
                    mesh.indices[j * 3 + 1] = 0;
                    mesh.indices[j * 3 + 2] = 0;

                    continue;
                }


                memcpy(
                    &mesh.indices[j * 3],
                    face.mIndices,
                    3 * sizeof(GLuint)
                );
            }


            LOG(
                "Mesh %d with %d indices",
                i,
                mesh.num_indices
            );
        }
        else
        {
            LOG(
                "WARNING: Mesh %d has no faces",
                i
            );
        }

        // ========================================================
        // MATERIAL / TEXTURA
        // ========================================================

        if (ai_mesh->mMaterialIndex < scene->mNumMaterials)
        {
            const aiMaterial* material =
                scene->mMaterials[ai_mesh->mMaterialIndex];

            // ========================================================
            // COLOR DIFUSO DEL MATERIAL
            // ========================================================

            aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

            if (aiGetMaterialColor(
                material,
                AI_MATKEY_COLOR_DIFFUSE,
                &diffuseColor) == AI_SUCCESS)
            {
                mesh.diffuseColor = glm::vec4(
                    diffuseColor.r,
                    diffuseColor.g,
                    diffuseColor.b,
                    diffuseColor.a
                );
            }
            else
            {
                mesh.diffuseColor = glm::vec4(1.0f);
            }

            LOG("Mesh %d diffuse color: R=%.2f G=%.2f B=%.2f",
                i,
                mesh.diffuseColor.r,
                mesh.diffuseColor.g,
                mesh.diffuseColor.b);

            if (material->GetTextureCount(
                aiTextureType_DIFFUSE) > 0)
            {
                aiString texturePath;

                if (material->GetTexture(
                    aiTextureType_DIFFUSE,
                    0,
                    &texturePath) == AI_SUCCESS)
                {
                    std::string texturePathString =
                        texturePath.C_Str();


                    // Si Assimp devuelve una ruta absoluta,
                    // usamos esa ruta directamente.
                    if (texturePathString.size() > 1 &&
                        texturePathString[1] == ':')
                    {
                        mesh.diffuseTexture =
                            texturePathString;
                    }
                    else
                    {
                        mesh.diffuseTexture =
                            modelDirectory +
                            texturePathString;
                    }

                    LOG(
                        "Mesh %d diffuse texture: %s",
                        i,
                        mesh.diffuseTexture.c_str()
                    );
                }
            }
        }


        // ====================================================
        // GUARDAR MESH
        // ====================================================

        const size_t meshIndex = meshes.size();

        meshes.push_back(std::move(mesh));

        // Obtener el nombre original del mesh desde Assimp.
        std::string meshName = ai_mesh->mName.C_Str();

        // Si el mesh no tiene nombre, utilizar uno por defecto.
        if (meshName.empty())
        {
            meshName = "Mesh" + std::to_string(i);
        }

        GameObject& meshObject =
            modelObject.CreateChild(meshName);

        Component& component =
            meshObject.CreateComponent(ComponentType::MeshRenderer);

        auto* meshRenderer =
            static_cast<MeshRenderer*>(&component);

        meshRenderer->SetMeshIndex(meshIndex);
    }


    // ========================================================
    // LIBERAR ASSIMP
    // ========================================================

    aiReleaseImport(scene);


    LOG(
        "Model loaded. Total meshes: %d",
        meshes.size()
    );


    return true;
}

bool ResourceManager::LoadTexture(
    const char* file_path,
    Texture& texture)
{
    LOG("Loading texture: %s", file_path);

    ILuint imageID = 0;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage(file_path))
    {
        LOG("ERROR: Could not load texture: %s", file_path);
        ilDeleteImages(1, &imageID);
        return false;
    }

    if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
    {
        LOG("ERROR: Could not convert texture to RGBA");
        ilDeleteImages(1, &imageID);
        return false;
    }

    // Normalizar el origen de la imagen para OpenGL.
    iluFlipImage();

    texture.width = ilGetInteger(IL_IMAGE_WIDTH);
    texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
    texture.channels = 4;

    const size_t size =
        static_cast<size_t>(texture.width) *
        static_cast<size_t>(texture.height) * 4;

    texture.pixels.resize(size);

    memcpy(texture.pixels.data(), ilGetData(), size);

    LOG("Texture loaded: %dx%d",
        texture.width, texture.height);

    ilDeleteImages(1, &imageID);
    return true;
}

// ============================================================
// GET MESHES
// ============================================================

const std::vector<Mesh>& ResourceManager::GetMeshes() const
{
    return meshes;
}


// ============================================================
// CLEAR MESHES
// ============================================================

void ResourceManager::ClearMeshes()
{
    meshes.clear();
}


// ============================================================
// CLEAN UP
// ============================================================

// Called before quitting
bool ResourceManager::CleanUp()
{
    LOG("ResourceManager::CleanUp");

    ClearMeshes();

    ilShutDown();

    return true;
}
