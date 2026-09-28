#include "Render.h"
#include "Log.h"
#include "Engine.h"
#include "Window.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64
// Constructor
Render::Render() : Module()
{
    name = "render";
}

// Destructor
Render::~Render()
{
}

// Called before render is available
bool Render::Awake()
{
    LOG("Render::Awake");
    LOG("Initializing OpenGL");

    bool ret = true;

    int version = gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)
    );

    if (version == 0)
    {
        LOG("Error loading the GLAD library");
        return false;
    }

    LOG("GLAD initialized successfully");

    LOG("Vendor: %s", glGetString(GL_VENDOR));
    LOG("Renderer: %s", glGetString(GL_RENDERER));
    LOG("OpenGL version supported: %s", glGetString(GL_VERSION));
    LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // -------------------------
    // Triangle
    // -------------------------

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


    // 36 Índices (6 caras * 2 triángulos * 3 vértices)
    unsigned int indices[] =
    {
        // Trasera
        0, 1, 2,
        0, 2, 3,

        // Frontal
        4, 5, 6,
        4, 6, 7,

        // Izquierda
        8, 9, 10,
        8, 10, 11,

        // Derecha
        12, 13, 14,
        12, 14, 15,

        // Abajo
        16, 17, 18,
        16, 18, 19,

        // Arriba
        20, 21, 22,
        20, 22, 23
    };


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

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

    GLuint iBuff;

    glGenBuffers(1, &iBuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //glVertexAttribPointer(
    //    0,
    //    3, //Esto es el size del vector de vertex
    //    GL_FLOAT,
    //    GL_FALSE,
    //    6 * sizeof(float),
    //    (void*)0
    //);

    // Stride = 6 floats (3 para Posición + 3 para Color)
    GLsizei stride = 8 * sizeof(float);

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

    // -------------------------
    // Shaders
    // -------------------------

    std::string vertexSource =
        LoadShaderSource("Assets/Shaders/default.vert");

    std::string fragmentSource =
        LoadShaderSource("Assets/Shaders/default.frag");

    GLuint vertexShader =
        CompileShader(
            GL_VERTEX_SHADER,
            vertexSource.c_str()
        );

    GLuint fragmentShader =
        CompileShader(
            GL_FRAGMENT_SHADER,
            fragmentSource.c_str()
        );

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

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

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindVertexArray(0);

    //Textura checkers
    GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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

    return ret;
}

// Called before the first frame
bool Render::Start()
{
    LOG("Render::Start");

    bool ret = true;

    return ret;
}

// Called before each loop iteration
bool Render::PreUpdate()
{
    bool ret = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return ret;
}

// Called each loop iteration
bool Render::Update(float dt)
{
    bool ret = true;

    
    return ret;
}

// Called after each loop iteration
bool Render::PostUpdate()
{
    //glUseProgram(shaderProgram);

    //glBindVertexArray(VAO);

    ////glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //glBindVertexArray(0);

    //return true;

    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLint textureLoc = glGetUniformLocation(shaderProgram, "checkerTexture");

    glUniform1i(textureLoc, 0);
    // --- Matriz de transformación 3D (MVP) ---
    static float angle = 0.0f;
    angle += 0.01f; // Velocidad de rotación

    // 1. Model: Rotar y posicionar el cubo
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));

    // 2. View: Alejamos la cámara 3 unidades en Z
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    // 3. Projection: Perspectiva 3D (FOV 45°, Aspect Ratio 4:3 o según ventana)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Multiplicamos en orden P * V * M
    glm::mat4 mvp = projection * view * model;

    // Pasamos la matriz MVP al uniform del shader
    GLint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    // --- Dibujar el Cubo ---
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36 Índices
    glBindVertexArray(0);

    return true;
}

GLuint Render::CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

std::string Render::LoadShaderSource(const char* path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        LOG("Could not open shader file: %s", path);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

// Called before quitting
bool Render::CleanUp()
{
    LOG("Render::CleanUp");

    bool ret = true;

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    return ret;
}