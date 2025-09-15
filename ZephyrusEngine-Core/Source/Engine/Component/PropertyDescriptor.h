#pragma once
#include <string>

enum class PropertyType 
{ 
    Float, 
    Int, 
    Bool, 
    String, 
    Vec3, 
    Vec2, 
    Quaternion, 
    Texture, 
    Font, 
    Mesh,
    VectorTexture
};

struct PropertyDescriptor {
    std::string name;
    void* field;
    PropertyType type;

    bool isPointer = false;
};