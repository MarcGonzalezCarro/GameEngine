// ============================================================
// INCLUDES
// ============================================================
#include <SDL3/SDL.h>
#include "Render.h"
#include "Log.h"
#include "Engine.h"
#include "Window.h"
#include "Editor.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// ============================================================
// CONSTANTES
// ============================================================

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64


// ============================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================

// Constructor
Render::Render() : Module()
{
    name = "render";
}

// Destructor
Render::~Render()
{
}


// ============================================================
// AWAKE
// Inicialización de OpenGL, geometría, shaders y textura
// ============================================================

// Called before render is available
bool Render::Awake()
{
    LOG("Render::Awake");
    LOG("Initializing OpenGL");

    bool ret = true;


    // --------------------------------------------------------
    // Inicialización de GLAD
    // --------------------------------------------------------

    int version = gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)
    );

    if (version == 0)
    {
        LOG("Error loading the GLAD library");
        return false;
    }

    LOG("GLAD initialized successfully");


    // --------------------------------------------------------
    // Información de OpenGL
    // --------------------------------------------------------

    LOG("Vendor: %s", glGetString(GL_VENDOR));
    LOG("Renderer: %s", glGetString(GL_RENDERER));
    LOG("OpenGL version supported: %s", glGetString(GL_VERSION));
    LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


    // --------------------------------------------------------
    // Configuración inicial de OpenGL
    // --------------------------------------------------------

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    // ========================================================
    // VÉRTICES DEL CUBO
    // ========================================================

    /*float vertices[] =
    {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f
    };*/

    float vertices[] =
    {
        // =========================
        // CARA TRASERA
        // =========================

        // Posición              // Color              // UV
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,


        // =========================
        // CARA FRONTAL
        // =========================

        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.5f, 0.5f, 0.5f,    0.0f, 1.0f,


        // =========================
        // CARA IZQUIERDA
        // =========================

        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.5f, 0.5f, 0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,


        // =========================
        // CARA DERECHA
        // =========================

         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f,


         // =========================
         // CARA ABAJO
         // =========================

         -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
          0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,    0.0f, 1.0f,


         // =========================
         // CARA ARRIBA
         // =========================

         -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 0.0f,
          0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,    0.5f, 0.5f, 0.5f,    0.0f, 1.0f
    };


    // ========================================================
    // ÍNDICES DEL CUBO
    // ========================================================

    // 36 Índices (6 caras * 2 triángulos * 3 vértices)
    unsigned int indices[] =
    {
        // =========================
        // CARA TRASERA (-Z)
        // =========================

        0, 2, 1,
        0, 3, 2,


        // =========================
        // CARA FRONTAL (+Z)
        // =========================

        4, 5, 6,
        4, 6, 7,


        // =========================
        // CARA IZQUIERDA (-X)
        // =========================

        8, 9, 10,
        8, 10, 11,


        // =========================
        // CARA DERECHA (+X)
        // =========================

        12, 13, 14,
        12, 14, 15,


        // =========================
        // CARA ABAJO (-Y)
        // =========================

        16, 17, 18,
        16, 18, 19,


        // =========================
        // CARA ARRIBA (+Y)
        // =========================

        20, 22, 21,
        20, 23, 22
    };


    // ========================================================
    // VAO
    // ========================================================

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    // ========================================================
    // VBO
    // ========================================================

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    /*unsigned int indices[] = {
        0,1,2,2,1,3
    };*/


    // ========================================================
    // EBO / INDEX BUFFER
    // ========================================================

    GLuint iBuff;

    glGenBuffers(1, &iBuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuff);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );


    // ========================================================
    // ATRIBUTOS DE VÉRTICES
    // ========================================================

    //glVertexAttribPointer(
    //    0,
    //    3, //Esto es el size del vector de vertex
    //    GL_FLOAT,
    //    GL_FALSE,
    //    6 * sizeof(float),
    //    (void*)0
    //);


    // --------------------------------------------------------
    // Stride
    // --------------------------------------------------------

    // Stride = 6 floats (3 para Posición + 3 para Color)
    GLsizei stride = 8 * sizeof(float);


    // --------------------------------------------------------
    // Atributo 0: Posición
    // --------------------------------------------------------

    // Atributo 0: Posición (location = 0)
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        stride,
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // --------------------------------------------------------
    // Atributo 1: Color
    // --------------------------------------------------------

    // Atributo 1: Color (location = 1) ---> AQUÍ ESTÁ EL CAMBIO <---
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        stride,
        (void*)(3 * sizeof(float)) // Offset de 3 floats
    );

    glEnableVertexAttribArray(1);


    // --------------------------------------------------------
    // Atributo 2: Coordenadas UV
    // --------------------------------------------------------

    // Atributo 2: Coordenadas de textura (UV)
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        stride,
        (void*)(6 * sizeof(float))
    );

    glEnableVertexAttribArray(2);


    // ========================================================
    // SHADERS
    // ========================================================

    std::string vertexSource =
        LoadShaderSource("Assets/Shaders/default.vert");

    std::string fragmentSource =
        LoadShaderSource("Assets/Shaders/default.frag");


    // --------------------------------------------------------
    // Compilar Vertex Shader
    // --------------------------------------------------------

    GLuint vertexShader =
        CompileShader(
            GL_VERTEX_SHADER,
            vertexSource.c_str()
        );


    // --------------------------------------------------------
    // Compilar Fragment Shader
    // --------------------------------------------------------

    GLuint fragmentShader =
        CompileShader(
            GL_FRAGMENT_SHADER,
            fragmentSource.c_str()
        );


    // --------------------------------------------------------
    // Crear Shader Program
    // --------------------------------------------------------

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);


    // --------------------------------------------------------
    // Comprobar Link
    // --------------------------------------------------------

    GLint success = 0;

    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    if (success == GL_FALSE)
    {
        LOG("Error linking shader program");
        ret = false;
    }


    // --------------------------------------------------------
    // Liberar shaders
    // --------------------------------------------------------

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindVertexArray(0);

    //Shader Grid
    std::string gridVertexSource =
        LoadShaderSource("Assets/Shaders/grid.vert");

    std::string gridFragmentSource =
        LoadShaderSource("Assets/Shaders/grid.frag");

    GLuint gridVertexShader = CompileShader(
        GL_VERTEX_SHADER,
        gridVertexSource.c_str()
    );

    GLuint gridFragmentShader = CompileShader(
        GL_FRAGMENT_SHADER,
        gridFragmentSource.c_str()
    );

    if (gridVertexShader == 0 || gridFragmentShader == 0)
    {
        LOG("Failed to compile grid shaders");

        if (gridVertexShader != 0)
            glDeleteShader(gridVertexShader);

        if (gridFragmentShader != 0)
            glDeleteShader(gridFragmentShader);

        return false;
    }

    gridShaderProgram = glCreateProgram();

    glAttachShader(gridShaderProgram, gridVertexShader);
    glAttachShader(gridShaderProgram, gridFragmentShader);

    glLinkProgram(gridShaderProgram);

    GLint successs = GL_FALSE;

    glGetProgramiv(
        gridShaderProgram,
        GL_LINK_STATUS,
        &successs
    );

    glDeleteShader(gridVertexShader);
    glDeleteShader(gridFragmentShader);

    if (successs == GL_FALSE)
    {
        LOG("Failed to link grid shader");
        return false;
    }

    CreateGrid();
    
    //// ========================================================
    //// TEXTURA CHECKERS
    //// ========================================================

    //GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];


    //// --------------------------------------------------------
    //// Generar patrón de checkers
    //// --------------------------------------------------------

    //for (int i = 0; i < CHECKERS_HEIGHT; i++)
    //{
    //    for (int j = 0; j < CHECKERS_WIDTH; j++)
    //    {
    //        int c = ((((i & 0x8) == 0) ^
    //            ((j & 0x8) == 0))) * 255;

    //        checkerImage[i][j][0] = (GLubyte)c;
    //        checkerImage[i][j][1] = (GLubyte)c;
    //        checkerImage[i][j][2] = (GLubyte)c;
    //        checkerImage[i][j][3] = 255;
    //    }
    //}


    //// --------------------------------------------------------
    //// Configuración de lectura de píxeles
    //// --------------------------------------------------------

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Mirar


    //// --------------------------------------------------------
    //// Crear textura
    //// --------------------------------------------------------

    //glGenTextures(1, &textureID);
    //glBindTexture(GL_TEXTURE_2D, textureID);


    //// --------------------------------------------------------
    //// Wrap
    //// --------------------------------------------------------

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    //// --------------------------------------------------------
    //// Filtros
    //// --------------------------------------------------------

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    //// --------------------------------------------------------
    //// Cargar imagen en textura
    //// --------------------------------------------------------

    //glTexImage2D(
    //    GL_TEXTURE_2D,
    //    0,
    //    GL_RGBA,
    //    CHECKERS_WIDTH,
    //    CHECKERS_HEIGHT,
    //    0,
    //    GL_RGBA,
    //    GL_UNSIGNED_BYTE,
    //    checkerImage
    //);

    //glBindTexture(GL_TEXTURE_2D, 0);


    //// ========================================================
    //// FIN AWAKE
    //// ========================================================

    return ret;
}


// ============================================================
// START
// ============================================================

// Called before the first frame
bool Render::Start()
{
    LOG("Render::Start");

    auto& resourceManager =
        *Engine::GetInstance().resourceManager;

    auto scene = Engine::GetInstance().scene;

    if (!scene)
    {
        LOG("ERROR: Scene is null");
        return false;
    }

    // Cargar el modelo una sola vez.
    if (!resourceManager.LoadModel(
        "Assets/Models/BakerHouse.FBX"))
    {
        LOG("ERROR: Failed to load BakerHouse.FBX");
        return false;
    }

    const auto& meshes = resourceManager.GetMeshes();

    if (meshes.empty())
    {
        LOG("ERROR: Model contains no meshes");
        return false;
    }

    // Subir todos los meshes a la GPU.
    modelMeshes.clear();

    for (const Mesh& mesh : meshes)
    {
        modelMeshes.push_back(UploadMesh(mesh));
    }

    LOG("Uploaded %d meshes to GPU",
        static_cast<int>(modelMeshes.size()));

    // Asociar cada mesh a un GameObject.
    const auto& roots = scene->GetRootObjects();

    if (roots.empty())
    {
        LOG("WARNING: Scene has no root objects");
        return false;
    }

    CreateMeshObjects(
        *roots.front(),
        0,
        modelMeshes.size()
    );

    return true;
}

// ============================================================
// UPLOAD MESH
// Copia un Mesh de CPU a VRAM
// ============================================================

MeshGPU Render::UploadMesh(const Mesh& mesh)
{
    MeshGPU gpuMesh;


    // ========================================================
    // VAO
    // ========================================================

    glGenVertexArrays(
        1,
        &gpuMesh.VAO
    );

    glBindVertexArray(
        gpuMesh.VAO
    );


    // ========================================================
    // VBO
    // ========================================================

    glGenBuffers(
        1,
        &gpuMesh.VBO
    );

    glBindBuffer(
        GL_ARRAY_BUFFER,
        gpuMesh.VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh.num_vertices * 5 * sizeof(float),
        mesh.vertices,
        GL_STATIC_DRAW
    );


    // ========================================================
    // EBO
    // ========================================================

    glGenBuffers(
        1,
        &gpuMesh.EBO
    );

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        gpuMesh.EBO
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh.num_indices * sizeof(GLuint),
        mesh.indices,
        GL_STATIC_DRAW
    );

    if (!mesh.diffuseTexture.empty())
    {
        Texture texture;

        if (Engine::GetInstance()
            .resourceManager
            ->LoadTexture(
                mesh.diffuseTexture.c_str(),
                texture))
        {
            gpuMesh.textureID =
                UploadTexture(texture);

            LOG(
                "Texture uploaded for mesh: %s",
                mesh.diffuseTexture.c_str()
            );
        }
        else
        {
            LOG(
                "Could not load mesh texture: %s",
                mesh.diffuseTexture.c_str()
            );
        }
    }

    // ========================================================
    // VERTEX ATTRIBUTE
    //
    // De momento Assimp nos da únicamente:
    //
    // position.x
    // position.y
    // position.z
    //
    // ========================================================

    // ========================================================
    // POSITION
    // ========================================================

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ========================================================
    // UV
    // ========================================================

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(2);


    // ========================================================
    // FINAL
    // ========================================================

    gpuMesh.num_indices =
        mesh.num_indices;


    glBindVertexArray(0);


    LOG(
        "Mesh uploaded to GPU: %d vertices, %d indices",
        mesh.num_vertices,
        mesh.num_indices
    );


    return gpuMesh;
}

// ============================================================
// DRAW MESH
// ============================================================

void Render::DrawMesh(const MeshGPU& mesh)
{
    glBindVertexArray(
        mesh.VAO
    );


    glDrawElements(
        GL_TRIANGLES,
        mesh.num_indices,
        GL_UNSIGNED_INT,
        nullptr
    );


    glBindVertexArray(0);
}

// ============================================================
// DELETE MESH
// Libera los recursos OpenGL de un mesh
// ============================================================

void Render::DeleteMesh(MeshGPU& mesh)
{
    if (mesh.EBO != 0)
    {
        glDeleteBuffers(
            1,
            &mesh.EBO
        );

        mesh.EBO = 0;
    }


    if (mesh.VBO != 0)
    {
        glDeleteBuffers(
            1,
            &mesh.VBO
        );

        mesh.VBO = 0;
    }


    if (mesh.VAO != 0)
    {
        glDeleteVertexArrays(
            1,
            &mesh.VAO
        );

        mesh.VAO = 0;
    }

    if (mesh.textureID != 0)
    {
        glDeleteTextures(
            1,
            &mesh.textureID
        );

        mesh.textureID = 0;
    }

    mesh.num_indices = 0;


}

GLuint Render::UploadTexture(const Texture& texture)
{
    GLuint id = 0;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );


    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        texture.width,
        texture.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        texture.pixels.data()
    );


    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

// ============================================================
// PRE UPDATE
// Limpieza del framebuffer
// ============================================================

// Called before each loop iteration
bool Render::PreUpdate()
{
    bool ret = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return ret;
}


// ============================================================
// UPDATE
// ============================================================

// Called each loop iteration
bool Render::Update(float dt)
{
    // ========================================================
    // BUSCAR EL GAMEOBJECT DE LA CASA
    // ========================================================

    auto scene = Engine::GetInstance().scene;

    if (scene == nullptr)
        return true;

    GameObject* house = nullptr;

    for (const auto& object : scene->GetRootObjects())
    {
        if (object->GetName() == "BakerHouse")
        {
            house = object.get();
            break;
        }
    }

    if (house == nullptr)
        return true;

    // ========================================================
    // OBTENER EL TRANSFORM
    // ========================================================

    Component* component =
        house->GetComponent(ComponentType::Transform);

    if (component == nullptr)
        return true;

    Transform* transform =
        static_cast<Transform*>(component);

    // ========================================================
    // LEER TECLADO
    // ========================================================

    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    // Velocidad de movimiento en unidades por segundo.
    const float speed = 3.0f;

    glm::vec3 movement(0.0f);

    // Eje X
    if (keyboard[SDL_SCANCODE_LEFT])
        movement.x -= speed * dt;

    if (keyboard[SDL_SCANCODE_RIGHT])
        movement.x += speed * dt;

    // Eje Z
    if (keyboard[SDL_SCANCODE_UP])
        movement.z -= speed * dt;

    if (keyboard[SDL_SCANCODE_DOWN])
        movement.z += speed * dt;

    // Eje Y
    if (keyboard[SDL_SCANCODE_PAGEUP])
        movement.y += speed * dt;

    if (keyboard[SDL_SCANCODE_PAGEDOWN])
        movement.y -= speed * dt;

    // ========================================================
    // APLICAR MOVIMIENTO AL TRANSFORM
    // ========================================================

    if (movement.x != 0.0f ||
        movement.y != 0.0f ||
        movement.z != 0.0f)
    {
        glm::vec3 position = transform->GetPosition();

        position += movement;

        transform->SetPosition(position);
    }

    return true;
}

// ============================================================
// POST UPDATE
// Renderizado del cubo
// ============================================================

// Called after each loop iteration
bool Render::PostUpdate()
{
    // ========================================================
    // CÁMARA
    // ========================================================

    Camera& camera = Engine::GetInstance().editor->camera;

    const glm::mat4 view = camera.GetViewMatrix();
    const glm::mat4 projection = camera.GetProjectionMatrix();

    // ========================================================
    // GRID
    // ========================================================

    DrawGrid();

    // ========================================================
    // SHADER DEL MODELO
    // ========================================================

    glUseProgram(shaderProgram);

    const GLint mvpLoc =
        glGetUniformLocation(shaderProgram, "mvp");

    const GLint textureLoc =
        glGetUniformLocation(shaderProgram, "diffuseTexture");

    glUniform1i(textureLoc, 0);

    // ========================================================
    // ESCENA
    // ========================================================

    auto scene = Engine::GetInstance().scene;

    if (scene)
    {
        // Lambda recursiva para dibujar cada GameObject
        std::function<void(GameObject&)> DrawGameObject;

        DrawGameObject = [&](GameObject& object)
            {
                // ------------------------------------------------
                // Obtener Transform
                // ------------------------------------------------

                Component* transformComponent =
                    object.GetComponent(ComponentType::Transform);

                // ------------------------------------------------
                // Obtener MeshRenderer
                // ------------------------------------------------

                Component* rendererComponent =
                    object.GetComponent(ComponentType::MeshRenderer);

                if (transformComponent != nullptr &&
                    rendererComponent != nullptr)
                {
                    auto* transform =
                        static_cast<Transform*>(transformComponent);

                    auto* meshRenderer =
                        static_cast<MeshRenderer*>(rendererComponent);

                    const std::size_t index =
                        meshRenderer->GetMeshIndex();

                    if (meshRenderer->HasMesh() &&
                        index < modelMeshes.size())
                    {
                        const MeshGPU& gpuMesh = modelMeshes[index];

                        // ----------------------------------------
                        // MATRIZ DEL OBJETO
                        // ----------------------------------------

                        const glm::mat4 model =
                            transform->GetWorldMatrix();

                        const glm::mat4 mvp =
                            projection * view * model;

                        glUniformMatrix4fv(
                            mvpLoc,
                            1,
                            GL_FALSE,
                            glm::value_ptr(mvp)
                        );

                        // ----------------------------------------
                        // TEXTURA
                        // ----------------------------------------

                        glActiveTexture(GL_TEXTURE0);

                        glBindTexture(
                            GL_TEXTURE_2D,
                            gpuMesh.textureID
                        );

                        // ----------------------------------------
                        // DIBUJAR
                        // ----------------------------------------

                        DrawMesh(gpuMesh);
                    }
                }

                // ------------------------------------------------
                // DIBUJAR HIJOS
                // ------------------------------------------------

                for (const auto& child : object.GetChildren())
                {
                    DrawGameObject(*child);
                }
            };

        // ====================================================
        // RECORRER OBJETOS RAÍZ
        // ====================================================

        for (const auto& object : scene->GetRootObjects())
        {
            DrawGameObject(*object);
        }
    }

    // ========================================================
    // LIMPIEZA
    // ========================================================

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    return true;
}


// ============================================================
// COMPILE SHADER
// ============================================================

GLuint Render::CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(
        shader,
        1,
        &source,
        nullptr
    );

    glCompileShader(shader);


    // --------------------------------------------------------
    // Comprobar compilación
    // --------------------------------------------------------

    GLint success = 0;

    glGetShaderiv(
        shader,
        GL_COMPILE_STATUS,
        &success
    );

    if (success == GL_FALSE)
    {
        GLint logLength = 0;

        glGetShaderiv(
            shader,
            GL_INFO_LOG_LENGTH,
            &logLength
        );

        glDeleteShader(shader);

        return 0;
    }


    return shader;
}


// ============================================================
// LOAD SHADER SOURCE
// ============================================================

std::string Render::LoadShaderSource(const char* path)
{
    std::ifstream file(path);


    // --------------------------------------------------------
    // Comprobar archivo
    // --------------------------------------------------------

    if (!file.is_open())
    {
        LOG("Could not open shader file: %s", path);

        return "";
    }


    // --------------------------------------------------------
    // Leer contenido
    // --------------------------------------------------------

    std::stringstream buffer;

    buffer << file.rdbuf();


    return buffer.str();
}


// ============================================================
// CLEAN UP
// Liberación de recursos de OpenGL
// ============================================================

// Called before quitting
bool Render::CleanUp()
{
    LOG("Render::CleanUp");

    bool ret = true;


    // --------------------------------------------------------
    // Liberar VBO
    // --------------------------------------------------------

    glDeleteBuffers(1, &VBO);


    // --------------------------------------------------------
    // Liberar VAO
    // --------------------------------------------------------

    glDeleteVertexArrays(1, &VAO);

    if (gridVBO != 0)
    {
        glDeleteBuffers(1, &gridVBO);
        gridVBO = 0;
    }

    if (gridVAO != 0)
    {
        glDeleteVertexArrays(1, &gridVAO);
        gridVAO = 0;
    }

    if (gridShaderProgram != 0)
    {
        glDeleteProgram(gridShaderProgram);
        gridShaderProgram = 0;
    }

    // --------------------------------------------------------
    // Liberar Shader Program
    // --------------------------------------------------------

    glDeleteProgram(shaderProgram);

    if (textureID != 0)
    {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    for (MeshGPU& mesh : modelMeshes)
    {
        DeleteMesh(mesh);
    }

    modelMeshes.clear();

    return ret;
}

void Render::CreateGrid()
{
    const int halfSize = 10;
    const float spacing = 1.0f;

    std::vector<glm::vec3> vertices;

    for (int i = -halfSize; i <= halfSize; ++i)
    {
        float p = i * spacing;

        // Líneas paralelas al eje X
        vertices.emplace_back(
            -halfSize * spacing, 0.0f, p);
        vertices.emplace_back(
            halfSize * spacing, 0.0f, p);

        // Líneas paralelas al eje Z
        vertices.emplace_back(
            p, 0.0f, -halfSize * spacing);
        vertices.emplace_back(
            p, 0.0f, halfSize * spacing);
    }

    gridVertexCount = static_cast<GLsizei>(
        vertices.size());

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Render::DrawGrid()
{
    glUseProgram(gridShaderProgram);

    Camera& camera = Engine::GetInstance().editor->camera;

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 mvp = projection * view * model;

    GLint mvpLoc = glGetUniformLocation(
        gridShaderProgram, "mvp");

    glUniformMatrix4fv(
        mvpLoc,
        1,
        GL_FALSE,
        glm::value_ptr(mvp)
    );

    GLint colorLoc = glGetUniformLocation(
        gridShaderProgram, "gridColor");

    glUniform4f(colorLoc, 0.35f, 0.35f, 0.35f, 1.0f);

    glBindVertexArray(gridVAO);

    glDrawArrays(
        GL_LINES,
        0,
        gridVertexCount
    );

    glBindVertexArray(0);
}

void Render::CreateMeshObjects(
    GameObject& parent,
    std::size_t firstMesh,
    std::size_t meshCount)
{
    LOG("CreateMeshObjects: parent=%s, firstMesh=%d, meshCount=%d",
        parent.GetName().c_str(),
        static_cast<int>(firstMesh),
        static_cast<int>(meshCount));

    for (std::size_t i = firstMesh;
        i < firstMesh + meshCount;
        ++i)
    {
        GameObject& object = parent.CreateChild(
            "Mesh_" + std::to_string(i));

        LOG("Created GameObject: %s",
            object.GetName().c_str());

        auto& renderer =
            static_cast<MeshRenderer&>(
                object.CreateComponent(
                    ComponentType::MeshRenderer));

        renderer.SetMeshIndex(i);

        LOG("MeshRenderer created: object=%s, meshIndex=%d",
            object.GetName().c_str(),
            static_cast<int>(renderer.GetMeshIndex()));

        LOG("HasMesh: %s",
            renderer.HasMesh() ? "true" : "false");
    }
}
//Framebuffers para el imgui
//Utilizar stack/LIFO para gameObjects (DFS)
//Memoria cache lifo, mirar
//Guardar indices de gameobjects para evitar punteros apuntando a null al hacer reparenting
//Guardar indices a materiales para no cargar a memoria materiales repetidos
//Separar importers de save/load | Solo Engine necesita import