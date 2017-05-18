#version 330

uniform vec3 uAmbientColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uShininess;
uniform vec3 uDirLight;
uniform vec3 uCameraPos;

in vec3 vNormal;
in vec3 vPos;

out vec3 finalColor;

void main()
{
	// vec3 dirLight = vec3(0, -1, 0);
	// vec3 dirDiffuse = uDiffuseColor * max(0.0, dot(normalize(vNormal), normalize(dirLight)));
	// vec3 dirSpecular = uSpecularColor * max(0.0, pow(dot(normalize(vNormal), normalize(dirLight)), uShininess));

	//vec3 specular = uSpecularColor * max(0.0, pow(dot(normalize(vNormal), normalize(vLight)), uShininess));

	vec3 light = normalize(-uDirLight);
	vec3 view = normalize(uCameraPos - vPos);

	//Flat shading
	//vec3 norm = normalize(cross(dFdx(vPos), dFdy(vPos)));

	vec3 ambient = uDiffuseColor * .4;
	vec3 diffuse = uDiffuseColor * max(0.0, dot(vNormal, light)) * .6;
	vec3 specular = vec3(1) * pow(max(0.0, dot(vNormal, normalize(light + view))), 50) * 0; 
	finalColor = ambient + diffuse + specular;
}