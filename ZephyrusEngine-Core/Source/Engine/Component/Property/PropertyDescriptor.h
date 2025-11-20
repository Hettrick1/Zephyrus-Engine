#pragma once

#include "PropertyMetadata.h"
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
    ShaderVert,
    ShaderFrag,
    ShaderTesc,
    ShaderTese,
    ShaderGeom,
    ArrayFloat,
    ArrayInt,
    ArrayVector2D,
    ArrayVector3D,
    ArrayVector4D,
    ArrayTextureBase,
};

struct PropertyDescriptor {
    std::string name;
    void* field;
    PropertyType type;

    bool isPointer = false;
    Zephyrus::PropertyMetadata metadata = {};
    
    PropertyDescriptor(const std::string& n, void* f, PropertyType t, const Zephyrus::PropertyMetadata& m)
        : name(n), field(f), type(t), metadata(m) {}

    PropertyDescriptor(const std::string& n, void* f, PropertyType t)
        : name(n), field(f), type(t), metadata() {}
};