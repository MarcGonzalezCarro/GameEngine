#include "Scene/Scene.h"
#include "Log.h"

Scene::Scene() : Module()
{
    name = "Scene";
}

Scene::~Scene() {}

bool Scene::Awake()
{
    LOG("ModuleScene::Awake");
    return true;
}

bool Scene::Start()
{
    LOG("ModuleScene::Start");

    // Objeto de prueba para comprobar la jerarquía.
    GameObject& house = CreateGameObject("BakerHouse");

    house.CreateChild("Roof");
    house.CreateChild("Door");

    return true;
}

bool Scene::Update(float dt)
{
    for (auto& object : rootObjects)
    {
        object->Update();
    }

    return true;
}

GameObject& Scene::CreateGameObject(
    const std::string& objectName,
    GameObject* parent
)
{
    if (parent != nullptr)
        return parent->CreateChild(objectName);

    auto object = std::make_unique<GameObject>(
        objectName
    );

    GameObject& reference = *object;

    rootObjects.push_back(std::move(object));

    return reference;
}

void Scene::Clear()
{
    rootObjects.clear();
}

const std::vector<std::unique_ptr<GameObject>>&
Scene::GetRootObjects() const
{
    return rootObjects;
}

bool Scene::CleanUp()
{
    LOG("ModuleScene::CleanUp");

    Clear();

    return true;
}