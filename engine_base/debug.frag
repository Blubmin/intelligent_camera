#version 330

in vec2 vTex;

uniform sampler2D uTex;

out vec4 color;

void main()
{
    color = texture(uTex, vTex);
} 