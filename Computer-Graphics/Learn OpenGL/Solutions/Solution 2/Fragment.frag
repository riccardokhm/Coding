#version 330 core

in vec3 vertexPos;
in vec2 TexCoord;
in vec3 ourColor;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // Mixing the two textures
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 1.0f);

    // Using only the second texture multiplied by vertex color
    FragColor = texture(texture2, TexCoord) * vec4(ourColor, 1.0);
};