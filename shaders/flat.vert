#version 330

layout(location = 0) in vec3 aPosition;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

out vec3 vView;
out vec3 vPos;

void main()
{
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	vPos = (uModelMatrix * vec4(aPosition, 1.0)).xyz;
	vView = (uViewMatrix * vec4(vPos, 1.0)).xyz;
}