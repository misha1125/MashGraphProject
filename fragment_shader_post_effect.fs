#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D dephTexture;
uniform bool enablePostEffect;
uniform bool enableFocusing;

void main()
{
    color = texture(ourTexture, TexCoord);
    if (enableFocusing){
        float deph = texture(dephTexture, TexCoord).r;
        if (deph == 1){
            float offset = 1.0f / 850.0f;
            vec2 offset_array[9] = vec2[](
                    vec2(-offset,  offset),
                    vec2( 0.0f,    offset),
                    vec2( offset,  offset),
                    vec2(-offset,  0.0f),
                    vec2( 0.0f,    0.0f),
                    vec2( offset,  0.0f),
                    vec2(-offset, -offset),
                    vec2( 0.0f,   -offset),
                    vec2( offset, -offset)
                );
            float kern[9] = float[](
                 1.0 / 16, 2.0 / 16, 1.0 / 16,
                 2.0 / 16, 4.0 / 16, 2.0 / 16,
                 1.0 / 16, 2.0 / 16, 1.0 / 16
            );
            color.rgb  = vec3(0.0);
            for(int i = 0; i < 9; i++)
            {
               color.rgb += vec3(texture(ourTexture, TexCoord.st + offset_array[i])) * kern[i];
            }
        }
    }
    if (enablePostEffect){
        color = vec4(vec3(1.0 - texture(ourTexture, TexCoord)), 1.0);
    }

}