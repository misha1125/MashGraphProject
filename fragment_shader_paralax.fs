#version 330 core
in vec2 TexCoord;
in vec3 FragPos;
in mat3 normalChange;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    float shininess;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D normalTexture;
uniform bool enableGama;

uniform float heightScale;
uniform sampler2D depthMap;

float getColor(vec2 currentTexCoords){
    vec4 depthVal = texture(depthMap, currentTexCoords);
    return (depthVal.r + depthVal.g + depthVal.b)/3.0f ;
}

vec2 ParallaxMapping(vec2 texCoord, vec3 viewDir)
{
    float max_ = 32,min_ = 4;
    float layerDepth = 1.0 / mix(max_, min_, abs(viewDir.z));
    float currentLayerDepth = 0.0;
    vec2 deltaTexCoord =  viewDir.xy / viewDir.z * heightScale * layerDepth;
    vec2  currentTexCoord = texCoord;
    float currentDepthMapVal = getColor(currentTexCoord);
    while(currentLayerDepth < currentDepthMapVal)
    {
        currentTexCoord -= deltaTexCoord;
        currentDepthMapVal = getColor(currentTexCoord);
        currentLayerDepth += layerDepth;
    }
    deltaTexCoord /= 2.0f;
    currentTexCoord += deltaTexCoord;
    currentLayerDepth -= currentDepthMapVal/2;
    int currentStep = 5;
    while (currentStep-- > 0) {
        currentDepthMapVal = getColor(currentTexCoord);
        deltaTexCoord /= 2.0f;
        if (currentDepthMapVal / 2 > currentLayerDepth) {
            currentTexCoord -= deltaTexCoord;
            currentLayerDepth += layerDepth;
        } else {
            currentTexCoord += deltaTexCoord;
            currentLayerDepth -= layerDepth;
        }
    }
    return currentTexCoord;
}


//Модель Блинна-Фонга
void main()
{
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoord = TexCoord;

    texCoord = ParallaxMapping(TexCoord,  viewDir);
    if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0){
       discard;
    }
    vec3 ambient = material.ambient * lightColor * vec3(texture(ourTexture, texCoord));
    vec3 norm = texture(normalTexture, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(norm);
    vec3 lightDir = normalize(TangentLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse * vec3(texture(ourTexture, texCoord));

    //vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * lightColor*0.5f;

    float distance    = length(TangentLightPos - FragPos);
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