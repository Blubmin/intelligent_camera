#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 0) in vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uLightPos;

out vec3 vNormal;
out vec3 vPos;

void main()
{
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	vNormal = normalize(aNormal);
	vPos = (uModelMatrix * vec4(aPosition, 1.0)).xyz;
}