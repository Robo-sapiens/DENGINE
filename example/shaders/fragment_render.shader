#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenTextureBright;

void main()
{
    vec3 color = vec3(texture(screenTexture, TexCoords));

    vec3 bloom_color = vec3(0.0);
    float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    float offset = vec2(1.0 / textureSize(screenTexture, 0)).x;

    if (length(vec3(texture(screenTextureBright, TexCoords))) > 0.0) {
        for (int i = 0; i < 5; i++) {
            bloom_color += 1.63 * weight[i] * vec3(texture(screenTexture, TexCoords + vec2(offset * (i - 2), 0.0)));
        }
        color = bloom_color;
    }

    FragColor = vec4(color, 1.0);
}