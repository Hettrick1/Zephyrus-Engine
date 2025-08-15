#version 330 core

layout(location = 0) in vec3 pos;

out vec4 inColor;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform vec3 uColor = vec3(0, 1, 0);

void main()
{
   gl_Position = vec4(pos, 1.0) * uWorldTransform * uViewProj;
   inColor = vec4(uColor, 1);
}