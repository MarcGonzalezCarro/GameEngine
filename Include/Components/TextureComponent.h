
#pragma once

#include "Scene/Component.h"

#include <glad/glad.h>
#include <string>

class TextureComponent final : public Component
{
public:
    explicit TextureComponent(GameObject* owner);

    void SetTexture(
        const std::string& path,
        GLuint textureID,
        int width,
        int height
    );

    void ClearTexture();

    const std::string& GetTexturePath() const;
    GLuint GetTextureID() const;

    int GetWidth() const;
    int GetHeight() const;

    bool HasTexture() const;

private:
    std::string texturePath;
    GLuint textureID = 0;

    int width = 0;
    int height = 0;
};
