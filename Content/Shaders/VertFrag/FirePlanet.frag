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
   vec4 noise = texture(uTexture, frag_in.texCoord);

    vec4 color1 = vec4(1.0f, 0.0f, 0.0f, 1.0f);  // montagne rouge vif
    vec4 color2 = vec4(0.8f, 0.5f, 0.0f, 1.0f);  // plaine orange
    vec4 color3 = vec4(0.9f, 1.0f, 0.0f, 1.0f);  // rivière de lave (plus sombre)

    float n = noise.r;

    vec4 resultColor;
    if (n > 0.35)
    {
         // Rivières de lave
         float t = smoothstep(0.3, 0.7, n);
         resultColor = mix(color2, color3, t);
    }
    else if (n > 0.1)
    {
         // Plaine
         float t = smoothstep(0.1, 0.4, n);
         resultColor = mix(color1, color2, t);
    }
    else
    {
         // Montagnes
         float t = smoothstep(0.2, 0.6, n);
         resultColor = mix(color1, color3, t);
    }

    FragColor = resultColor;
}
