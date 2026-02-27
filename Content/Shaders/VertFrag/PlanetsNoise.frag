#version 450 core

in TESE_OUT{
   vec4 color;
   vec3 normal;
   vec3 texCoord;
} frag_in;

out vec4 FragColor;

uniform samplerCube uTexture;

void main()
{
   FragColor = texture(uTexture, frag_in.texCoord);
}
