#version 330

uniform vec3 uDiffuseColor;

out vec3 finalColor;

void main()
{
	finalColor = uDiffuseColor;
}