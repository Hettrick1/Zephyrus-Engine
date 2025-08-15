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

    // Couleurs par altitude
    vec4 color1 = vec4(1.0f, 1.0f, 1.0f, 1.0f);  // neige
    vec4 color2 = vec4(0.7f, 0.7f, 0.7f, 1.0f);  // roche
    vec4 color3 = vec4(0.0f, 0.7f, 0.0f, 1.0f);  // herbe/plaine
    vec4 color4 = vec4(0.0f, 0.2f, 1.0f, 1.0f);  // eau

    float n = 1.0 - noise.r; // Inversé pour correspondre à hauteur
     n = (n - 0.45) / (1.0 - 0.45);
     n = clamp(n, 0.0, 1.0);

    vec4 resultColor;

     if (n < 0.2) {
     // Eau vers plaine
     resultColor =color4;
     }
     if (n < 0.3) {
     // Eau vers plaine
     float t = smoothstep(0.2, 0.3, n);
     resultColor = mix(color4, color3, t);
     }
     else if (n < 0.5) {
     // Plaine vers roche
     float t = smoothstep(0.5, 0.55, n);
     resultColor = mix(color3, color2, t);
     }
     else if (n < 0.9) {
     // Roche vers neige
     float t = smoothstep(0.65, 0.9, n);
     resultColor = mix(color2, color1, t);
     }
     else {
     resultColor = color1; // Neige
     }
    FragColor = resultColor;
}
