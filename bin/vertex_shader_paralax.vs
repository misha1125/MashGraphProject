#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoord;
out vec3 FragPos;
out mat3 normalChange;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 cameraView;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * cameraView * view * model * vec4(position, 1.0f);
    TexCoord = texCoord;
    normalChange = mat3(transpose(inverse(model)));
    FragPos = vec3(view * model * vec4(position, 1.0f));

    vec3 T = normalize(mat3(model) * tangent);
    vec3 B = normalize(mat3(model) * bitangent);
    vec3 N = normalize(mat3(model) * normal);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
}