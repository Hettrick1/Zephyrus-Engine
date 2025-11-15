#version 420 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 texCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

out vec2 fragTexCoord;
out vec3 fragWorldPos;

void main()
{
   // gl_Position = vec4(pos, 1.0) * uWorldTransform * uViewProj;
   // fragTexCoord = texCoord;

   vec4 worldPos = vec4(pos, 1.0) * uWorldTransform;

   gl_Position = worldPos * uViewProj;

   fragTexCoord = texCoord;
   fragWorldPos = worldPos.xyz;
}

