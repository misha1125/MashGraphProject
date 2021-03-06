#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool enableGama;
//Модель Блинна-Фонга
void main()
{

    vec3 ambient = material.ambient * lightColor * vec3(texture(ourTexture, TexCoord));
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse * vec3(texture(ourTexture, TexCoord));


    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * lightColor;
    float distance    = length(lightPos - FragPos);
        float attenuation = 1.0 / (material.constant + material.linear * distance +
        		    material.quadratic * (distance * distance));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    //гамма коррекция
    if (enableGama) {
        float gamma = 2.2;
        diffuse = pow(diffuse, vec3(gamma));
        color = vec4(ambient + diffuse + specular,1.0f);
        color.rgb = pow(color.rgb, vec3(1.0/gamma));
    }else {
        color = vec4(ambient + diffuse + specular,1.0f);
    }
}