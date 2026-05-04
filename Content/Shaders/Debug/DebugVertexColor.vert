#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec4 inColor;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

void main()
{
   gl_Position = vec4(pos, 1.0) * uWorldTransform * uViewProj;
   inColor = vec4(color, 1);
}