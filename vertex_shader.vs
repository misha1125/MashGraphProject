#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 cameraView;

void main()
{

    gl_Position = projection * cameraView * view * model * vec4(position, 1.0f);
    ourColor = color;
    TexCoord = texCoord;
}