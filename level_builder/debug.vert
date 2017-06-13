#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform vec2 uPos;
uniform float uScale;

out vec2 vTex;

void main() {
	gl_Position = vec4((aPos + vec3(1, 1, 0)) * uScale - vec3(1, 1, 0) + vec3(uPos, 0), 1);
	vTex = aTex;
}