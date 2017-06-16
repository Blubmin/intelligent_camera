#version 330

in vec2 vTex;

uniform sampler2D uTex;

out vec4 color;

void main()
{
    color = vec4(vec3(texture(uTex, vTex)), 1);
} 