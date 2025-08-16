#pragma once
#include <optional>
#include "Vector3D.h"
#include "rapidjson/document.h"

std::optional<Vector3D> ReadVector3D(const rapidjson::Value& obj, const char* key);