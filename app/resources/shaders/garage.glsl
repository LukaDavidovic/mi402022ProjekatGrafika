//#shader vertex

// Transformiše poziciju i normale modela u svet koordinatni prostor, prosleđuje teksturne koordinate
// Postavlja gl_Position za rasterizaciju koristeći projekciju i view matricu

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

// Izračunava boju piksela sa osnovnim osvetljenjem (ambient, diffuse, specular)
// Koristi poziciju svetla, kameru i teksturu da bi dobio realističan izgled površine
// Kombinuje teksturu i svetlosne komponente za finalnu boju piksela

#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;



// pozicija svetla

uniform vec3 lightPos1;
uniform vec3 lightDir1;

uniform vec3 viewPos;

uniform vec3 lightColor1;
uniform vec3 lightColor2;

uniform float lightIntensity;//ovo svetlo ce moci da se menja

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 textureColor = texture(texture_diffuse1, TexCoords).rgb;

    float ambientStrength = 0.3;
    float specularStrength = 0.3;

    vec3 totalLight = vec3(0.05);

    // svetlo1 - point light(lightcolor1,lightPos1)

    {
        vec3 lightDir = normalize(lightPos1 - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 ambient = ambientStrength * lightColor1;
        vec3 diffuse = diff * lightColor1;
        vec3 specular = specularStrength * spec * lightColor1;

        float distance = length(lightPos1 - FragPos);
        float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);

        totalLight += (ambient + diffuse + specular) * lightIntensity * attenuation;
    }
    // svetlo 2 direkciono
    {
        vec3 lightDir = normalize(-lightDir1);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 ambient = ambientStrength * lightColor2;
        vec3 diffuse = diff * lightColor2;
        vec3 specular = specularStrength * spec * lightColor2;

        totalLight += (ambient + diffuse + specular);
    }

    //konacno
    vec3 result = totalLight * textureColor;
    FragColor = vec4(result, 1.0);


}