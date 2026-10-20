
#include "Components/MeshRenderer.h"

#include <utility>

MeshRenderer::MeshRenderer(GameObject* owner)
    : Component(owner, ComponentType::MeshRenderer)
{
}

void MeshRenderer::SetMeshIndex(std::size_t index)
{
    meshIndex = index;
}

std::size_t MeshRenderer::GetMeshIndex() const
{
    return meshIndex;
}

bool MeshRenderer::HasMesh() const
{
    return meshIndex != InvalidIndex;
}

void MeshRenderer::SetTexturePath(const std::string& path)
{
    texturePath = path;
}

const std::string& MeshRenderer::GetTexturePath() const
{
    return texturePath;
}

bool MeshRenderer::HasTextureOverride() const
{
    return !texturePath.empty();
}

void MeshRenderer::ClearTextureOverride()
{
    texturePath.clear();
}
