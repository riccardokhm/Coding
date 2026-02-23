#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

//uniform mat4 model, view, projection;

//out vec3 Normal;
//out vec3 FragPos;

out vec3 LightingColor; // resulting color from lighting calculations

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	  gl_Position = projection * view * model * vec4(aPos, 1.0f);

	  vec3 Position = vec3(model * vec4(aPos, 1.0));
      vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

	  //ambient
	  float ambientLight = 0.1;
	  vec3 ambient = ambientLight * lightColor;

	  vec3 norm = normalize(Normal);
	  vec3 lightDir = normalize(lightPos - Position);

	  // Diffuse
	  float diff = max(dot(norm, lightDir), 0.0);
	  vec3 diffuse = diff * lightColor;


	  // Specular
	  float specularStrength = 0.7;
	  vec3 viewDir = normalize( viewPos - Position); // if in view space, viewDir = (0,0,0) so viewDir = -fragPos
	  vec3 reflectDir = reflect(-lightDir, norm);

	  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 6);
	  vec3 specular = specularStrength * spec * lightColor;

	  LightingColor = (ambient + diffuse + specular);
	

	  //FragPos = vec3(view * model * vec4(aPos, 1.0));
	  //Normal = mat3(transpose(inverse(view * model))) * aNormal;
}; 