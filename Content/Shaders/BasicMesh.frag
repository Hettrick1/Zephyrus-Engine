#version 420 core

in vec2 fragTexCoord;
in vec3 vWorldPos;

out vec4 outColor;

uniform sampler2D uTexture;

// FrameData UBO binding 0
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
   vec3 baseColor = texture(uTexture, fragTexCoord).rgb;

   // distance caméra -> fragment
   float dist = length(vWorldPos - uCameraPos);

   // fog factor linéaire : 0 = avant fogStart, 1 = après fogEnd
   float fogFactor = clamp((dist - uFogStart) / (uFogEnd - uFogStart), 0.0, 1.0);

   // mix texture avec fogColor
   vec3 finalColor = mix(baseColor, uFogColor, fogFactor);

   outColor = vec4(finalColor, 1.0);

   // vec3 baseColor = texture(uTexture, fragTexCoord).rgb;
   // float fogFactor = 0.0; // ignore le fog
   // vec3 finalColor = baseColor;
   // outColor = vec4(finalColor, 1.0);
}
