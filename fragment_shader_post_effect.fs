#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform bool enablePostEffect;

void main()
{
    if (enablePostEffect){
        color = vec4(vec3(1.0 - texture(ourTexture, TexCoord)), 1.0);
    } else {
        color = texture(ourTexture, TexCoord);
    }
}