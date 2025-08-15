#version 330 core

in vec2 fragTexCoord;
out vec4 color;

uniform sampler2D uTexture;
uniform vec4 uTint;

void main()
{
   color = texture(uTexture, fragTexCoord) * uTint;
   if(color.a < 0.01) discard;
}
