
#include "Scene/Scene.h"
#include "Log.h"
#include "Components/Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
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

    rootObjects.push_back(
        std::make_unique<GameObject>("Scene")
    );
}

// ============================================================
// GET ROOT OBJECTS
// ============================================================

const std::vector<std::unique_ptr<GameObject>>&
Scene::GetRootObjects() const
{
    return rootObjects;
}

bool Scene::DeleteGameObject(GameObject* object)
{
    if (object == nullptr)
        return false;

    // No permitir eliminar el objeto raíz Scene
    if (object == rootObjects.front().get())
        return false;

    GameObject* parent = object->GetParent();

    if (parent != nullptr)
    {
        return parent->RemoveChild(object);
    }

    auto it = std::find_if(
        rootObjects.begin(),
        rootObjects.end(),
        [object](const std::unique_ptr<GameObject>& obj)
        {
            return obj.get() == object;
        }
    );

    if (it == rootObjects.end())
        return false;

    rootObjects.erase(it);

    return true;
}

GameObject* Scene::DuplicateGameObject(GameObject* object)
{
    if (object == nullptr)
        return nullptr;

    // No duplicar la raíz Scene
    if (object == rootObjects.front().get())
        return nullptr;

    return object->Duplicate();
}

// ============================================================
// REPARENT GAME OBJECT
// ============================================================

bool Scene::ReparentGameObject(
    GameObject* object,
    GameObject* newParent)
{
    if (object == nullptr)
        return false;

    GameObject* sceneRoot = rootObjects.front().get();

    // No mover la raíz de la escena.
    if (object == sceneRoot)
        return false;

    // nullptr significa volver a la raíz de la escena.
    if (newParent == nullptr)
        newParent = sceneRoot;

    // Evitar ciclos en la jerarquía.
    if (newParent == object ||
        newParent->IsDescendantOf(object))
    {
        return false;
    }

    GameObject* oldParent = object->GetParent();

    if (oldParent == nullptr)
        return false;

    // Obtener el Transform del objeto.
    auto* transform = static_cast<Transform*>(
        object->GetComponent(ComponentType::Transform)
        );

    if (transform == nullptr)
        return false;

    // 1. Guardar la matriz global actual.
    const glm::mat4 oldWorldMatrix =
        transform->GetWorldMatrix();

    // 2. Obtener el Transform del nuevo padre.
    auto* parentTransform = static_cast<Transform*>(
        newParent->GetComponent(ComponentType::Transform)
        );

    if (parentTransform == nullptr)
        return false;

    // 3. Obtener la matriz global del nuevo padre.
    const glm::mat4 parentWorldMatrix =
        parentTransform->GetWorldMatrix();

    // Evitar invertir una matriz singular.
    const float determinant = glm::determinant(parentWorldMatrix);

    if (glm::abs(determinant) < 0.000001f)
        return false;

    // 4. Calcular la nueva matriz local.
    const glm::mat4 newLocalMatrix =
        glm::inverse(parentWorldMatrix) * oldWorldMatrix;

    // 5. Extraer el objeto de su padre anterior.
    std::unique_ptr<GameObject> detached =
        oldParent->DetachChild(object);

    if (!detached)
        return false;

    // 6. Añadirlo al nuevo padre.
    if (!newParent->AddChild(std::move(detached)))
        return false;

    // 7. Aplicar la nueva transformación local.
    transform->SetLocalMatrix(newLocalMatrix);

    return true;
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
