#version 450 core

out vec4 FragColor;
in vec4 color;
uniform samplerCube uTexture;

void main()
{
   FragColor = color;
}
