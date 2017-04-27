#version 330

uniform vec3 uAmbientColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uShininess;

in vec3 vNormal;
in vec3 vLight;

out vec4 finalColor;

void main()
{
	// vec3 dirLight = vec3(0, -1, 0);
	// vec3 dirDiffuse = uDiffuseColor * max(0.0, dot(normalize(vNormal), normalize(dirLight)));
	// vec3 dirSpecular = uSpecularColor * max(0.0, pow(dot(normalize(vNormal), normalize(dirLight)), uShininess));

	//vec3 specular = uSpecularColor * max(0.0, pow(dot(normalize(vNormal), normalize(vLight)), uShininess));
	vec3 diffuse = uDiffuseColor * max(0.0, dot(normalize(vNormal), normalize(vLight)));
	finalColor = vec4((diffuse + uDiffuseColor) * 2, 1);
}