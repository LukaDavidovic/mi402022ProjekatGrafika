//#shader vertex

// Transformiše poziciju i normale modela u svet koordinatni prostor, prosleđuje teksturne koordinate
// Postavlja gl_Position za rasterizaciju koristeći projekciju i view matricu

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
//out vec3 Normal;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = mat3(transpose(inverse(model))) * aNormal; // ispravno transformisanje normale

    TexCoords = aTexCoords;

    //TBN matrica za normal mapu

    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);

    TBN = mat3(T, B, N);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment

// Izračunava boju piksela sa osnovnim osvetljenjem (ambient, diffuse, specular)
// Koristi poziciju svetla, kameru i teksturu da bi dobio realističan izgled površine
// Kombinuje teksturu i svetlosne komponente za finalnu boju piksela

#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D albedoMap;
uniform sampler2D aoMap;
uniform sampler2D bumpMap;
uniform sampler2D metalnessMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;

// svetlo
uniform vec3 lightPos1;
uniform vec3 lightDir1;
uniform vec3 viewPos;

uniform vec3 lightColor1;
uniform vec3 lightColor2;

uniform float lightIntensity;
uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;


void main()
{
    vec2 uv = TexCoords;

    uv.y = 1.0 - uv.y;

    // učitavanje svih tekstura
    vec3 albedo = texture(albedoMap, uv).rgb;
    float ao = texture(aoMap, uv).r;
    float bump = texture(bumpMap, uv).r;
    float metalness = texture(metalnessMap, uv).r;
    float roughness = texture(roughnessMap, uv).r;

    vec3 normal = texture(normalMap, uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    // vektori
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 totalLight = vec3(0.3);

    // parametri osvetljenja
    float ambientStrength = 0.4;   // malo jače
    float specularStrength = 0.1;  // sjaj naglašeniji

    // POINT LIGHT
    {
        vec3 lightDir = normalize(lightPos1 - FragPos);
        float distance = length(lightPos1 - FragPos);
        float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 ambient = ambientStrength * lightColor1;
        vec3 diffuse = diff * lightColor1;
        vec3 specular = specularStrength * spec * lightColor1;

        totalLight += (ambient + diffuse + specular) * lightIntensity * attenuation;
    }

    // DIREKCIONO SVETLO – pojačano
    {
        vec3 lightDir = normalize(-lightDir1);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 ambient = ambientStrength * lightColor2;
        vec3 diffuse = diff * lightColor2;      // ×2 jače osvetljenje
        vec3 specular = specularStrength * spec * lightColor2;// sjajnije refleksije

        totalLight += (ambient + diffuse + specular);
    }

    // konačna boja
    vec3 result = totalLight * albedo * ao * bump * roughness;
    FragColor = vec4(result, 1.0);
}