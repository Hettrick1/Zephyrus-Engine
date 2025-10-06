#pragma once
#include <string>
#include "Vector3D.h"
#include "Vector2D.h"

namespace Serialization
{

	class ISerializer
	{
	public:
		virtual ~ISerializer() = default;

		virtual void BeginObject(const char* pKey = nullptr) = 0;
		virtual void EndObject() = 0;
		virtual void BeginArray(const char* pKey) = 0;
		virtual void EndArray() = 0;
		virtual void WriteString(const char* pKey, const std::string& pValue) = 0;
		virtual void WriteFloat(const char* pKey, float pValue) = 0;
		virtual void WriteInt(const char* pKey, int pValue) = 0;
		virtual void WriteBool(const char* pKey, bool pValue) = 0;
		virtual void WriteVector3D(const char* pKey, const Vector3D& pVec) = 0;
		virtual void WriteVector2D(const char* pKey, const Vector2D& pVec) = 0;
		virtual void PushString(const std::string& pValue) = 0;
		virtual void PushFloat(float pValue) = 0;
		virtual void PushInt(int pValue) = 0;
		virtual void PushBool(bool pValue) = 0;
		virtual bool SaveDocument(const std::string& pFilepath) = 0;
	};
}