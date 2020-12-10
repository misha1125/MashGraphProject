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
    vec3 norm = normalChange * texture(normalTexture, TexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse * vec3(texture(ourTexture, TexCoord));
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * lightColor * vec3(texture(material.specularTex, TexCoord));

    color = vec4(ambient + diffuse + specular,1.0f);
}