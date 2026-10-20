
#include "Scene/Scene.h"
#include "Log.h"

// ============================================================
// CONSTRUCTOR
// ============================================================

Scene::Scene() : Module()
{
    name = "Scene";
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

    // Crear objeto de prueba.
    GameObject& house = CreateGameObject("BakerHouse");

    house.CreateChild("Roof");
    house.CreateChild("Door");

    LOG("Created GameObject: %s", house.GetName().c_str());

    return true;
}

// ============================================================
// CREATE GAME OBJECT
// ============================================================

GameObject& Scene::CreateGameObject(
    const std::string& objectName,
    GameObject* parent
)
{
    // Si tiene padre, se crea como hijo.
    if (parent != nullptr)
    {
        return parent->CreateChild(objectName);
    }

    // Si no tiene padre, es un objeto raíz.
    auto object = std::make_unique<GameObject>(
        objectName
    );

    GameObject& reference = *object;

    rootObjects.push_back(std::move(object));

    return reference;
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
