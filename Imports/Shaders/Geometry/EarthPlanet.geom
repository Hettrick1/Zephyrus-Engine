#version 450 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 24) out;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

out vec4 color;

uniform samplerCube uTexture;

in TESE_OUT{
    vec4 color;
    vec3 normal;
    vec3 texCoord;
} gs_in[];


void EmitCubeFace(vec4 bottomLeft, vec4 bottomRight, vec4 topLeft, vec4 topRight, vec4 color1, vec4 color2, vec4 color3, vec4 color4)
{
    gl_Position = bottomLeft;
    color = color1;
    EmitVertex();
    
    gl_Position = topLeft;
    color = color2;
    EmitVertex();
    
    gl_Position = bottomRight;
    color = color3;
    EmitVertex();
    
    gl_Position = topRight;
    color = color4;
    EmitVertex();
    
    EndPrimitive();
}

void CreateCube(vec4 position, vec4 normal, float size, float height)
{
    // Base du cube (face inférieure)
    vec4 base[4] = vec4[4](
        position + vec4(-size, -size*2, 0.0, 0.0),
        position + vec4( size, -size*2, 0.0, 0.0),
        position + vec4(-size,  size*2, 0.0, 0.0),
        position + vec4( size,  size*2, 0.0, 0.0)
    );
    
    // Sommet du cube (face supérieure extrudée)
    vec4 top[4] = vec4[4](
        base[0] + normal * height,
        base[1] + normal * height,
        base[2] + normal * height,
        base[3] + normal * height
    );

    top[0] = top[0] - vec4(-0.4, -0.8, 0 , 0);
    top[1] = top[1] - vec4(0.4, -0.8, 0 , 0);
    top[2] = top[2] - vec4(-0.4, 0.8, 0 , 0);
    top[3] = top[3] - vec4(0.4, 0.8, 0 , 0);
    
    // Faces latérales
    EmitCubeFace(base[0], base[1], top[0], top[1], vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.23, 1.0), vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.23, 1.0)); // Face avant
    EmitCubeFace(base[1], base[3], top[1], top[3], vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.23, 1.0), vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.23, 1.0)); // Face droite
    EmitCubeFace(base[3], base[2], top[3], top[2], vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.23, 1.0), vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.23, 1.0)); // Face arrière
    EmitCubeFace(base[2], base[0], top[2], top[0], vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.23, 1.0), vec4(0.03, 0.28, 0.03, 1.0), vec4(0.23, 0.68, 0.3, 1.0)); // Face gauche
    
    // Faces horizontales (bas et haut)
    EmitCubeFace(base[0], base[2], base[1], base[3], vec4(0.03, 0.28, 0.03, 1.0), vec4(0.03, 0.28, 0.03, 1.0), vec4(0.03, 0.28, 0.03, 1.0), vec4(0.03, 0.28, 0.03, 1.0)); // Face inférieure
    EmitCubeFace(top[0], top[1], top[2], top[3], vec4(0.23, 0.68, 0.23, 1.0), vec4(0.23, 0.68, 0.23, 1.0), vec4(0.23, 0.68, 0.23, 1.0), vec4(0.23, 0.68, 0.23, 1.0)); // Face supérieure
}

void main() {
    vec4 noise = texture(uTexture, gs_in[0].texCoord);
    float n = 1.0 - noise.r;
    n = (n - 0.45) / (1.0 - 0.45);
    n = clamp(n, 0.0, 1.0);

    vec4 interpPos = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
    vec3 interpNormal = normalize((gs_in[0].normal + gs_in[1].normal + gs_in[2].normal) / 3.0);
    vec4 interpNormal4 = vec4(interpNormal, 0.0) * uWorldTransform * uViewProj;
    
    if(n > 0.4 && n < 0.6) {
        CreateCube(interpPos, interpNormal4, 0.8, 1.0);
    }
}