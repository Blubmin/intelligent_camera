#version 330

layout(location = 0) in vec3 aPosition;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main()
{
	vec4 pos = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
    gl_Position = pos;
}