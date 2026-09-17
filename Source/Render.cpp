#include "Render.h"
#include "Log.h"
#include "Engine.h"
#include "Window.h"
#include <string>
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

    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
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

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

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
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    Engine::GetInstance().window->SwapBuffers();

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