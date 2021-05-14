#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aNormal;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;

void main()
{             
    FragColor = vec4(texture(texture1, TexCoords));
}
