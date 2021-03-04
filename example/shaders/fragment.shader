#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentLightPos2;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec4 lightColor;
uniform vec4 lightColor2;

uniform float heightScale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;

    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    discard;

    // obtain normal from normal map
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // get diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.01 * color;
    // diffuse
    vec3 lightDir1 = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff1 = max(dot(lightDir1, normal), 0.0);

    vec3 lightDir2 = normalize(fs_in.TangentLightPos2 - fs_in.TangentFragPos);
    float diff2 = max(dot(lightDir2, normal), 0.0);

    vec3 diffuse = diff1 * mix(vec3(lightColor), color, 0.8) + diff2 * mix(vec3(lightColor2), color, 0.8);
    // specular
    vec3 reflectDir1 = reflect(-lightDir1, normal);
    vec3 halfwayDir1 = normalize(lightDir1 + viewDir);
    float spec1 = pow(max(dot(normal, halfwayDir1), 0.0), 64.0);

    vec3 reflectDir2 = reflect(-lightDir2, normal);
    vec3 halfwayDir2 = normalize(lightDir2 + viewDir);
    float spec2 = pow(max(dot(normal, halfwayDir2), 0.0), 64.0);

    vec3 specular = vec3(lightColor) * lightColor.a * spec1 + vec3(lightColor2) * lightColor2.a * spec2;
    color = ambient + diffuse + specular;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));

    color = vec3(1.0) - exp(-color * 0.25);

    FragColor = vec4(color, 1.0);

    if(brightness > 1.0) {
        BrightColor = FragColor;
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

