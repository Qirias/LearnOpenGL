#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aNormal;
layout(location = 1) in vec2 aTexCoords;


out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

in vec2 TexCoords;
uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float LenearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{

    // gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
    // float depth = LenearizeDepth(gl_FragCoord.z) / far;
    // gl_FragColor = vec4(vec3(depth), 1.0);
    gl_FragColor = texture(texture1, TexCoords);
}