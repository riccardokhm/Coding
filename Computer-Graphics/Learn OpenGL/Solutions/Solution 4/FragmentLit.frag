#version 330 core

struct Material {
	sampler2d diffuse;
	vec3 specular;
	float shininess;
	};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;



void main()
{
	// Ambient light contribution Amb = objectColor * AmbientLight
	vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TextCoords));


	// Specular
	vec3 viewDir = normalize( - FragPos); // if in view space, viewDir = (0,0,0) so viewDir = -fragPos
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0f);

}