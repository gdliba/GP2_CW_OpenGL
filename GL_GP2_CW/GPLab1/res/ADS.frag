#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos1;
uniform vec3 lightColor1;

uniform vec3 lightPos2;
uniform vec3 lightColor2;

uniform vec3 emissive;

uniform vec3 viewPos;
uniform sampler2D diffuse;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    float shininess = 32.0;

    vec3 texColor = texture(diffuse, TexCoord).rgb;

    vec3 result = vec3(0.0);

    // Light 1
    {
        vec3 lightDir = normalize(lightPos1 - FragPos);
        vec3 ambient = ambientStrength * lightColor1;
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuseLight = diff * lightColor1;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor1;

        result += (ambient + diffuseLight + specular) * texColor;
    }

    // Light 2
    {
        vec3 lightDir = normalize(lightPos2 - FragPos);
        vec3 ambient = ambientStrength * lightColor2;
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuseLight = diff * lightColor2;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor2;

        result += (ambient + diffuseLight + specular) * texColor;
    }

    // FragColor = vec4(result, texture(diffuse, TexCoord).a);
    FragColor = vec4(result + emissive, texture(diffuse, TexCoord).a);
}