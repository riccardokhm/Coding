#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1;
float far  = 100.0;

float LinearizedDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{    
    float depth = LinearizedDepth(gl_FragCoord.z) / far; // divide by far for demonstration purposes
    vec4 texColor = texture(texture1, TexCoords);

    FragColor = texColor;
    //FragColor = vec4(vec3(depth), 1.0);
}