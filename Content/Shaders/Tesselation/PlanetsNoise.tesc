#version 450 core

layout(vertices = 3) out;

in VS_OUT{
   vec4 color;
   vec3 normal;
   vec3 texCoord;
} tesc_in[];
out TESC_OUT{
   vec4 color;
   vec3 normal;
   vec3 texCoord;
} tesc_out[];

uniform float uLod;

void main(void)
{
   if (gl_InvocationID == 0)
   {
       gl_TessLevelInner[0] = uLod;
       gl_TessLevelOuter[0] = uLod;
       gl_TessLevelOuter[1] = uLod;
       gl_TessLevelOuter[2] = uLod;
   }
   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
   tesc_out[gl_InvocationID].color = tesc_in[gl_InvocationID].color;
   tesc_out[gl_InvocationID].normal = tesc_in[gl_InvocationID].normal;
   tesc_out[gl_InvocationID].texCoord = tesc_in[gl_InvocationID].texCoord;
}
