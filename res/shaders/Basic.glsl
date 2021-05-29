#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform bool showBlinn;

struct Material{
    sampler2D texture;
    float shininess;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights;
uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform sampler2D texture1_diffuse1;

void main()
{             
    // FragColor = vec4(texture(texture1_diffuse1, TexCoords));
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);    

    vec3 result = vec3(0.0);
    result += CalcPointLight(pointLights, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = 0.0;
    if (showBlinn)
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    else
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);

    float distance      = length(light.position - fragPos);
    float attenuation   = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient    = light.ambient * vec3(texture(texture1_diffuse1, TexCoords));
    vec3 diffuse    = light.diffuse * diff * vec3(texture(texture1_diffuse1, TexCoords));
    vec3 specular   = light.specular * spec * vec3(texture(texture1_diffuse1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
