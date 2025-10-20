//#shader vertex



#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // ispravno transformisanje normale
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment


#version 330 core

out vec4 FragColor;


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

// pozicija svetla
uniform vec3 lightPos1;

uniform vec3 viewPos;      // kamera pozicija

uniform vec3 lightColor1;   // belo


void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);


    //f-ja za jedno svetlo

    float ambientStrength = 0.7;
    float specularStrength = 0.7;

    vec3 totalLight = vec3(0.1);

    // svetlo1

    {
        vec3 lightDir = normalize(lightPos1 - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 ambient = ambientStrength * lightColor1;
        vec3 diffuse = diff * lightColor1;
        vec3 specular = specularStrength * spec * lightColor1;

        totalLight += (ambient + diffuse + specular);
    }

    //konacno
    vec3 result = totalLight;
    FragColor = vec4(result, 1.0);


}