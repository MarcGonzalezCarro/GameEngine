// ============================================================
// INCLUDES
// ============================================================

#include "Render.h"
#include "Log.h"
#include "Engine.h"
#include "Window.h"
#include "Editor.h"
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


    // ========================================================
    // TEXTURA CHECKERS
    // ========================================================

    GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];


    // --------------------------------------------------------
    // Generar patrón de checkers
    // --------------------------------------------------------

    for (int i = 0; i < CHECKERS_HEIGHT; i++)
    {
        for (int j = 0; j < CHECKERS_WIDTH; j++)
        {
            int c = ((((i & 0x8) == 0) ^
                ((j & 0x8) == 0))) * 255;

            checkerImage[i][j][0] = (GLubyte)c;
            checkerImage[i][j][1] = (GLubyte)c;
            checkerImage[i][j][2] = (GLubyte)c;
            checkerImage[i][j][3] = 255;
        }
    }


    // --------------------------------------------------------
    // Configuración de lectura de píxeles
    // --------------------------------------------------------

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Mirar


    // --------------------------------------------------------
    // Crear textura
    // --------------------------------------------------------

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);


    // --------------------------------------------------------
    // Wrap
    // --------------------------------------------------------

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    // --------------------------------------------------------
    // Filtros
    // --------------------------------------------------------

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    // --------------------------------------------------------
    // Cargar imagen en textura
    // --------------------------------------------------------

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        CHECKERS_WIDTH,
        CHECKERS_HEIGHT,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        checkerImage
    );

    glBindTexture(GL_TEXTURE_2D, 0);


    // ========================================================
    // FIN AWAKE
    // ========================================================

    return ret;
}


// ============================================================
// START
// ============================================================

// Called before the first frame
bool Render::Start()
{
    LOG("Render::Start");

    bool ret = true;


    // ========================================================
    // LOAD TEST MODEL
    // ========================================================

    if (Engine::GetInstance()
        .resourceManager
        ->LoadModel("Assets/Models/BakerHouse.FBX"))
    {
        LOG("Warrior loaded successfully");


        const std::vector<Mesh>& meshes =
            Engine::GetInstance()
            .resourceManager
            ->GetMeshes();


        // ====================================================
        // UPLOAD ALL MESHES
        // ====================================================

        for (const Mesh& mesh : meshes)
        {
            MeshGPU gpuMesh =
                UploadMesh(mesh);

            modelMeshes.push_back(
                gpuMesh
            );
        }


        LOG(
            "Uploaded %d meshes to GPU",
            modelMeshes.size()
        );
    }
    else
    {
        LOG("Failed to load warrior.fbx");
    }


    return ret;
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
        mesh.num_vertices * 3 * sizeof(float),
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

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


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


    mesh.num_indices = 0;
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
    bool ret = true;


    return ret;
}


// ============================================================
// POST UPDATE
// Renderizado del cubo
// ============================================================

// Called after each loop iteration
bool Render::PostUpdate()
{
    //glUseProgram(shaderProgram);

    //glBindVertexArray(VAO);

    ////glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //glBindVertexArray(0);

    //return true;


    // ========================================================
    // SHADER
    // ========================================================

    glUseProgram(shaderProgram);


    // ========================================================
    // TEXTURA
    // ========================================================

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLint textureLoc =
        glGetUniformLocation(
            shaderProgram,
            "checkerTexture"
        );

    glUniform1i(textureLoc, 0);


    // ========================================================
    // MATRICES DE TRANSFORMACIÓN 3D
    // ========================================================
    
    // Obtener la cámara del módulo Editor
    Camera& camera = Engine::GetInstance().editor->camera;
    static float angle = 0.0f;

    angle += 0.01f; // Velocidad de rotación


    // --------------------------------------------------------
    // MODEL
    // --------------------------------------------------------

    glm::mat4 model = glm::mat4(1.0f);

    // --------------------------------------------------------
    // VIEW
    // --------------------------------------------------------

    glm::mat4 view = camera.GetViewMatrix();

    // --------------------------------------------------------
    // PROJECTION
    // --------------------------------------------------------

    glm::mat4 projection = camera.GetProjectionMatrix();

    // --------------------------------------------------------
    // MVP
    // P * V * M
    // --------------------------------------------------------

    // Multiplicamos en orden P * V * M
    glm::mat4 mvp =
        projection * view * model;


    // --------------------------------------------------------
    // Enviar MVP al shader
    // --------------------------------------------------------

    // Pasamos la matriz MVP al uniform del shader
    GLint mvpLoc =
        glGetUniformLocation(
            shaderProgram,
            "mvp"
        );

    glUniformMatrix4fv(
        mvpLoc,
        1,
        GL_FALSE,
        glm::value_ptr(mvp)
    );


    // ========================================================
    // DIBUJAR CUBO
    // ========================================================

    //glBindVertexArray(VAO);

    //glDrawElements(
    //    GL_TRIANGLES,
    //    36,
    //    GL_UNSIGNED_INT,
    //    0
    //); // 36 Índices

    //glBindVertexArray(0);

    // ========================================================
    // DIBUJAR MODELO IMPORTADO
    // ========================================================

    for (const MeshGPU& mesh : modelMeshes)
    {
        DrawMesh(mesh);
    }

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


    // --------------------------------------------------------
    // Liberar Shader Program
    // --------------------------------------------------------

    glDeleteProgram(shaderProgram);


    return ret;
}

//Framebuffers para el imgui
//Utilizar stack/LIFO para gameObjects (DFS)
//Memoria cache lifo, mirar
//Guardar indices de gameobjects para evitar punteros apuntando a null al hacer reparenting
//Guardar indices a materiales para no cargar a memoria materiales repetidos
//Separar importers de save/load | Solo Engine necesita import