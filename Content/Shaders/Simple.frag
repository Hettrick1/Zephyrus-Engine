#version 420 core

in vec2 fragTexCoord;
in vec3 fragWorldPos;

out vec4 color;

uniform sampler2D uTexture;
uniform vec4 uTint;

layout(std140, binding = 0) uniform FrameData
{
    vec3 uCameraPos;

    float uTime;
    float uScreenWidth;
    float uScreenHeight;
    float uFogStart;
    float uFogEnd;

    vec3 uFogColor;
};

void main()
{
    vec4 baseColor = texture(uTexture, fragTexCoord) * uTint;
    if (baseColor.a < 0.01) discard;

    float dist = distance(fragWorldPos, uCameraPos);

    // Fog linéaire
    float fogFactor = clamp((dist - uFogStart) / (uFogEnd - uFogStart), 0.0, 1.0);

    vec3 finalColor = mix(baseColor.rgb, uFogColor, fogFactor);

    color = vec4(finalColor, baseColor.a);
}
