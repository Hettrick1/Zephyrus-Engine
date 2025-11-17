#pragma once
#include <string>

enum class PropertyType 
{ 
    Float, 
    Int, 
    Bool, 
    String,
    Color,
    Vec4,
    Vec3, 
    Vec2, 
    Quaternion, 
    Texture, 
    Font, 
    Mesh,
    MaterialInstance,
    VectorTexture,
    Prefab,
    CubeMap,
    Component,
    Shader,
};

struct PropertyDescriptor {
    std::string name;
    void* field;
    PropertyType type;

    bool isPointer = false;
};