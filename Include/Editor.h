#pragma once

#include "Module.h"
#include "Log.h" // Incluimos tu sistema de logs
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class GameObject;
// ============================================================
// MÓDULO EDITOR
// ============================================================
class Editor : public Module
{
public:
    Editor();
    ~Editor();

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update(float dt) override;
    bool PostUpdate() override;
    bool CleanUp() override;

    // Selección
    GameObject* GetSelectedObject() const;

    // Consola
    void AddConsoleLog(const std::string& message);

private:
    void DrawHierarchy();
    void DrawHierarchyNode(GameObject& object);
    void DrawInspector();
    void DrawConsole();
    void DrawSceneWindow();
    void DrawMainMenu();
    void DrawDockspace();
    void DrawConfiguration();
    void DrawAbout();
    bool imguiInitialized = false;

    void CreatePrimitive(const std::string& primitiveName);

    GameObject* selectedObject = nullptr;

    GameObject* pendingReparentObject = nullptr;
    GameObject* pendingReparentParent = nullptr;

    std::vector<std::string> consoleLogs;

    bool showScene = true;
    bool showHierarchy = true;
    bool showInspector = true;
    bool showConsole = true;
    bool sceneHovered = false;
    bool sceneFocused = false;

    bool showConfiguration = false;
    bool showAbout = false;

    static constexpr int FPS_HISTORY_SIZE = 120;

    float fpsHistory[FPS_HISTORY_SIZE] = {};
    int fpsHistoryOffset = 0;

    // Rendering configuration
    bool vsyncEnabled = true;
    bool depthTestEnabled = true;
    bool cullFaceEnabled = true;
    bool blendingEnabled = true;
    bool multisamplingEnabled = true;
    bool wireframeEnabled = false;
    bool scissorTestEnabled = false;
    bool stencilTestEnabled = false;

    int cullFaceMode = 0;  // 0 = BACK, 1 = FRONT, 2 = FRONT_AND_BACK
    int frontFaceMode = 0; // 0 = CCW, 1 = CW
public:
    
};