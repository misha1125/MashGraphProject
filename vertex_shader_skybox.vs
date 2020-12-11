#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

out vec3 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 cameraView;

void main()
{
    gl_Position = projection * cameraView * vec4(position, 1.0f);
    TexCoord = position;
}