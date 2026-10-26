
#include "Scene/Scene.h"
#include "Log.h"

// ============================================================
// CONSTRUCTOR
// ============================================================

Scene::Scene() : Module()
{
    name = "Scene";

    rootObjects.push_back(
        std::make_unique<GameObject>("Scene")
    );
}

Scene::~Scene() = default;

// ============================================================
// AWAKE
// ============================================================

bool Scene::Awake()
{
    LOG("Scene::Awake");

    return true;
}

// ============================================================
// START
// ============================================================

bool Scene::Start()
{
    LOG("Scene::Start");


    return true;
}

// ============================================================
// CREATE GAME OBJECT
// ============================================================

GameObject& Scene::CreateGameObject(
    const std::string& name,
    GameObject* parent)
{
    if (parent != nullptr)
    {
        return parent->CreateChild(name);
    }

    return rootObjects.front()->CreateChild(name);
}

// ============================================================
// UPDATE
// ============================================================

bool Scene::Update(float dt)
{
    for (auto& object : rootObjects)
    {
        object->Update();
    }

    return true;
}

// ============================================================
// CLEAR
// ============================================================

void Scene::Clear()
{
    rootObjects.clear();
}

// ============================================================
// GET ROOT OBJECTS
// ============================================================

const std::vector<std::unique_ptr<GameObject>>&
Scene::GetRootObjects() const
{
    return rootObjects;
}

// ============================================================
// CLEAN UP
// ============================================================

bool Scene::CleanUp()
{
    LOG("Scene::CleanUp");

    Clear();

    return true;
}
