
#pragma once

#include "Scene/Component.h"

#include <cstddef>
#include <limits>
#include <string>

class MeshRenderer final : public Component
{
public:
    explicit MeshRenderer(GameObject* owner);

    void SetMeshIndex(std::size_t index);
    std::size_t GetMeshIndex() const;
    bool HasMesh() const;

    void SetTexturePath(const std::string& path);
    const std::string& GetTexturePath() const;
    bool HasTextureOverride() const;

    void ClearTextureOverride();

private:
    static constexpr std::size_t InvalidIndex =
        std::numeric_limits<std::size_t>::max();

    std::size_t meshIndex = InvalidIndex;

    // Empty means use the diffuse texture from the imported mesh.
    std::string texturePath;
};
