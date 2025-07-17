#pragma once

#include "Vector3D.h"

class Vector4D
{
    friend class Matrix4D;

public:
	float x;
	float y;
	float z;
	float w;

	Vector3D xyz;

	Vector4D()
		:x(0.0f)
		,y(0.0f)
		,z(0.0f)
		,w(0.0f)
		,xyz(Vector3D(0,0,0))
	{}

	explicit Vector4D(float inX, float inY, float inZ, float inW)
		:x(inX)
		,y(inY)
		,z(inZ)
		,w(inW)
		, xyz(Vector3D(inX, inY, inZ))
	{}
	explicit Vector4D(float in)
		:x(in)
		, y(in)
		, z(in)
		, w(in)
		, xyz(Vector3D(in, in, in))
	{}
	explicit Vector4D(Vector3D in, float x = 1.0f)
		:x(in.x)
		, y(in.y)
		, z(in.z)
		, w(x)
		, xyz(Vector3D(in.x, in.y, in.z))
	{
	}

	float LengthSqr() const;
	float Length() const;
	void Normalize();

	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}

	// Set all four components in one line
	void Set(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	float& operator[](int i)
	{
		return *(reinterpret_cast<float*>(&x) + i);
	}

		// Vector addition (a + b)
	friend Vector4D operator+(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	// Vector subtraction (a - b)
	friend Vector4D operator-(const Vector4D& a, const Vector4D& b)
	{
		return Vector4D(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	// Component-wise multiplication
	friend Vector4D operator*(const Vector4D& left, const Vector4D& right)
	{
		return Vector4D(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	}

	// Scalar multiplication
	friend Vector4D operator*(const Vector4D& vec, float scalar)
	{
		return Vector4D(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
	}

	// Scalar multiplication
	friend Vector4D operator*(float scalar, const Vector4D& vec)
	{
		return Vector4D(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
	}

	// Component-wise division
	friend Vector4D operator/(const Vector4D& left, const Vector4D& right)
	{
		return Vector4D(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
	}

	// Scalar multiplication
	friend Vector4D operator/(const Vector4D& vec, float scalar)
	{
		return Vector4D(vec.x / scalar, vec.y / scalar, vec.z / scalar, vec.w / scalar);
	}

	// Scalar /=
	Vector4D& operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	// Scalar *=
	Vector4D& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	// Vector +=
	Vector4D& operator+=(const Vector4D& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		w += right.w;
		return *this;
	}

	// Vector -=
	Vector4D& operator-=(const Vector4D& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		w -= right.w;
		return *this;
	}

	// Normalize the provided vector
	static Vector4D Normalize(const Vector4D& vec)
	{
		Vector4D temp = vec;
		temp.Normalize();
		return temp;
	}

	// Dot product between two vectors (a dot b)
	static float Dot(const Vector4D& a, const Vector4D& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	// Cross product between two vectors (a cross b)
	static Vector4D Cross(const Vector4D& a, const Vector4D& b)
	{
		Vector4D temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}

	// Lerp from A to B by f
	static Vector4D Lerp(const Vector4D& a, const Vector4D& b, float f)
	{
		return Vector4D(a + f * (b - a));
	}

	// Reflect V about (normalized) N
	static Vector4D Reflect(const Vector4D& v, const Vector4D& n)
	{
		return v - 2.0f * Vector4D::Dot(v, n) * n;
	}

};
