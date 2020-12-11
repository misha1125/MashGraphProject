#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec3 FragPos;
in mat3 normalChange;

out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    float shininess;
    sampler2D specularTex;

     float constant;
    float linear;
    float quadratic;
};

uniform Material material;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D normalTexture;

void main()
{
    vec3 ambient = material.ambient * lightColor * vec3(texture(ourTexture, TexCoord));
    vec3 norm = texture(normalTexture, TexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(normalChange * norm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse * vec3(texture(ourTexture, TexCoord));
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specText = vec3(texture(material.specularTex, TexCoord));
    vec3 specular = spec * lightColor * specText;
    specular = spec * lightColor*0.2f;

    float distance    = length(lightPos - FragPos);
            float attenuation = 1.0 / (material.constant + material.linear * distance +
            		    material.quadratic * (distance * distance));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

    color = vec4(ambient + diffuse + specular,1.0f);
}