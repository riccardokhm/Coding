#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model, view, projection;

out vec3 vertexPos;
out vec2 TexCoord;
out vec3 ourColor;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0f);

  vertexPos = gl_Position.xyz;
  ourColor = vertexPos;
  TexCoord = aTexCoord;
}; 