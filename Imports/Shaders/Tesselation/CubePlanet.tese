#version 450 core

layout(triangles, fractional_even_spacing, cw) in;

uniform float uTimer;

in TESC_OUT{
   vec4 color;
   vec3 normal;
   vec3 texCoord;

} tese_in[];
out TESE_OUT{
   vec4 color;
   vec3 normal;
   vec3 texCoord;

} tese_out;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) 
{
   return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2; 
}
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main(void)
{
   gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +
   gl_TessCoord.y * gl_in[1].gl_Position +
   gl_TessCoord.z * gl_in[2].gl_Position);

   tese_out.normal = interpolate3D(tese_in[0].normal, tese_in[1].normal, tese_in[2].normal);
   tese_out.color = mix(tese_in[0].color, tese_in[1].color, gl_TessCoord.x);
   tese_out.texCoord = interpolate3D(tese_in[0].texCoord, tese_in[1].texCoord, tese_in[2].texCoord);
}
