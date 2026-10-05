// ============================================================
// INCLUDES
// ============================================================

#include "ResourceManager.h"
#include "Log.h"


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
    LOG("Assets::Awake");

    bool ret = true;

    return ret;
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


    // ========================================================
    // IMPORTAR ESCENA
    // ========================================================

    const aiScene* scene = aiImportFile(
        file_path,
        aiProcessPreset_TargetRealtime_MaxQuality |
        aiProcess_Triangulate
    );


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
    // LIMPIAR MESHES ANTERIORES
    // ========================================================

    ClearMeshes();


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
                new float[mesh.num_vertices * 3];


            memcpy(
                mesh.vertices,
                ai_mesh->mVertices,
                sizeof(float) *
                mesh.num_vertices *
                3
            );
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


        // ====================================================
        // GUARDAR MESH
        // ====================================================

        meshes.push_back(std::move(mesh));
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
    LOG("Assets::CleanUp");

    ClearMeshes();

    return true;
}
