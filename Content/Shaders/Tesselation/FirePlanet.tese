#version 450 core

layout(triangles, fractional_even_spacing, cw) in;

uniform float uTimer;
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

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

uniform samplerCube uTexture;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) 
{
   return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2; 
}
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

float Distance(vec3 v0)
{
   return abs(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);
}

void main(void)
{
   vec4 test = (gl_TessCoord.x * gl_in[0].gl_Position +
   gl_TessCoord.y * gl_in[1].gl_Position +
   gl_TessCoord.z * gl_in[2].gl_Position);

   tese_out.texCoord = interpolate3D(tese_in[0].texCoord, tese_in[1].texCoord, tese_in[2].texCoord);

   vec4 noise = texture(uTexture, tese_out.texCoord) * -4;

   vec4 pos = test;

   pos.x = test.x * sqrt(1 - (test.y*test.y)*0.5 - (test.z*test.z)*0.5 + (test.y*test.y * test.z*test.z) * 0.3333333333333);
   pos.y = test.y * sqrt(1 - (test.z*test.z)*0.5 - (test.x*test.x)*0.5 + (test.z*test.z * test.x*test.x) * 0.3333333333333);
   pos.z = test.z * sqrt(1 - (test.x*test.x)*0.5 - (test.y*test.y)*0.5 + (test.x*test.x * test.y*test.y) * 0.3333333333333);

   pos *= noise + 15;

   gl_Position = pos;
   tese_out.normal = interpolate3D(tese_in[0].normal, tese_in[1].normal, tese_in[2].normal);

   gl_Position *= uWorldTransform * uViewProj;

   tese_out.color = mix(tese_in[0].color, tese_in[1].color, gl_TessCoord.x);
}
