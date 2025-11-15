#version 420 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform vec2 uTiling;

out vec2 fragTexCoord;
out vec3 vWorldPos;

void main()
{
   vec4 worldPos =  vec4(pos, 1.0) * uWorldTransform;
   vWorldPos = worldPos.xyz;
   gl_Position = worldPos * uViewProj;
   fragTexCoord = texCoord * uTiling;
}
