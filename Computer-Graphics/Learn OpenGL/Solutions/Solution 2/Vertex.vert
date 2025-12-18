#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;

out vec3 vertexPos;
out vec2 TexCoord;
out vec3 ourColor;

void main()
{
  gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  vertexPos = gl_Position.xyz;
  ourColor = vertexPos;
  TexCoord = aTexCoord;
}; 