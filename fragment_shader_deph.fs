#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPos;


uniform float dist;



void main()
{
    float depth =0;
    if(gl_FragCoord.z > dist) {
    depth = 1;
    }
    color = vec4(vec3(depth), 1.0);
}