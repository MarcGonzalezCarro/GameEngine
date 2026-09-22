#include "Render.h"
#include "Log.h"
#include "Engine.h"
#include "Window.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

    float vertices[] = {
        // Posición (X, Y, Z)      // Color (R, G, B)
        -0.5f, -0.5f, -0.5f,      1.0f, 0.0f, 0.0f, // 0: Rojo
         0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 0.0f, // 1: Verde
         0.5f,  0.5f, -0.5f,      0.0f, 0.0f, 1.0f, // 2: Azul
        -0.5f,  0.5f, -0.5f,      1.0f, 1.0f, 0.0f, // 3: Amarillo

        -0.5f, -0.5f,  0.5f,      1.0f, 0.0f, 1.0f, // 4: Magenta
         0.5f, -0.5f,  0.5f,      0.0f, 1.0f, 1.0f, // 5: Cian
         0.5f,  0.5f,  0.5f,      1.0f, 1.0f, 1.0f, // 6: Blanco
        -0.5f,  0.5f,  0.5f,      0.5f, 0.5f, 0.5f  // 7: Gris
    };

    // 36 Índices (6 caras * 2 triángulos * 3 vértices)
    unsigned int indices[] = {
        // Cara trasera
        0, 2, 1,   0, 3, 2,
        // Cara frontal
        4, 5, 6,   4, 6, 7,
        // Cara izquierda
        4, 7, 3,   4, 3, 0,
        // Cara derecha
        1, 2, 6,   1, 6, 5,
        // Cara inferior
        0, 1, 5,   0, 5, 4,
        // Cara superior
        3, 7, 6,   3, 6, 2
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
    GLsizei stride = 6 * sizeof(float);

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