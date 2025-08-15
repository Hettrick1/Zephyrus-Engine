#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform vec2 uTiling;
uniform float uTime;

out VS_OUT{
   vec4 color;
   vec3 normal;
   vec3 texCoord;
} vs_out;

void main()
{
   gl_Position = vec4(pos, 1.0);
   vs_out.color = vec4(pos, 1.0);
   vs_out.normal = normal;
   vs_out.texCoord = vec3(pos.x, pos.y, pos.z);
}
