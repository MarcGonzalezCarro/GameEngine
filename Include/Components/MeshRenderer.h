#pragma once

#include "Scene/Component.h"

#include <cstddef>
#include <limits>
#include <string>

class MeshRenderer final : public Component
{
public:
    explicit MeshRenderer(GameObject* owner);

    // Mesh que se dibujará.
    void SetMeshIndex(std::size_t index);
    std::size_t GetMeshIndex() const;
    bool HasMesh() const;

    // Textura asignada manualmente.
    void SetTexturePath(const std::string& path);
    const std::string& GetTexturePath() const;
    bool HasTextureOverride() const;

    // Volver a la textura original del FBX.
    void ClearTextureOverride();

private:
    static constexpr std::size_t InvalidIndex =
        std::numeric_limits<std::size_t>::max();

    std::size_t meshIndex = InvalidIndex;

    // Vacío = utilizar la textura original del mesh.
    std::string texturePath;
};