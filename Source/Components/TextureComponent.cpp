
#include "Components/TextureComponent.h"

#include <utility>

TextureComponent::TextureComponent(GameObject* owner)
    : Component(owner, ComponentType::Texture)
{
}

void TextureComponent::SetTexture(
    const std::string& path,
    GLuint id,
    int textureWidth,
    int textureHeight)
{
    texturePath = path;
    textureID = id;
    width = textureWidth;
    height = textureHeight;
}

void TextureComponent::ClearTexture()
{
    texturePath.clear();
    textureID = 0;
    width = 0;
    height = 0;
}

const std::string& TextureComponent::GetTexturePath() const
{
    return texturePath;
}

GLuint TextureComponent::GetTextureID() const
{
    return textureID;
}

int TextureComponent::GetWidth() const
{
    return width;
}

int TextureComponent::GetHeight() const
{
    return height;
}

bool TextureComponent::HasTexture() const
{
    return textureID != 0;
}
