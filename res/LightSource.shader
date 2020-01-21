#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core
// #undef lowp
// #undef mediump
// #undef highp
// precision lowp float;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.38f, 0.87f, 0.16f,1.0);
}