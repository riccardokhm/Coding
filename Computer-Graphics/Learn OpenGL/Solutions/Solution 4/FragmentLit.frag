#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
	};

struct Light {
	vec3 position; // for point light
	vec3 direction; // for directional light

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;



void main()
{

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// Ambient light contribution Amb = objectColor * AmbientLight
	vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));


	// Specular
	vec3 viewDir = normalize( - FragPos); // if in view space, viewDir = (0,0,0) so viewDir = -fragPos
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

	vec3 emission = vec3(texture(material.emission, TexCoords)); 

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = attenuation * (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0f);

}