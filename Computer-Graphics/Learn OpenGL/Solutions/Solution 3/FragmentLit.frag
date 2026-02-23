#version 330 core


in vec3 LightingColor;

out vec4 FragColor;

uniform vec3 objectColor;

/*
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
in vec3 Normal;
in vec3 FragPos;
*/

void main()
{
	FragColor = vec4(LightingColor * objectColor, 1.0f);
	
	/*
	// Ambient light contribution Amb = objectColor * AmbientLight
	float ambientLight = 0.1;
	vec3 ambient = ambientLight * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// Diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;


	// Specular
	float specularStrength = 0.7;
	vec3 viewDir = normalize( - FragPos); // if in view space, viewDir = (0,0,0) so viewDir = -fragPos
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 6);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0f);
	*/

}