#version 460 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D checkerTexture;

void main()
{
    FragColor = texture(checkerTexture, TexCoord);
}