#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 texCoord;

uniform mat4 uViewProj;
uniform mat4 uWorld;
uniform vec2 uTiling;

out vec2 fragTexCoord;

void main()
{
   gl_Position = vec4(pos, 1.0) * uWorld * uViewProj ;
   fragTexCoord = texCoord;
}
