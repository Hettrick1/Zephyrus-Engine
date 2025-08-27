#pragma once
#include <cmath>
#include <string>
#include "Vector2D.h"

struct Vector3D
{
	float x, y, z;
	Vector3D() :x(0.0), y(0.0), z(0.0){}
	Vector3D(float pX, float pY, float pZ) : x(pX), y(pY), z(pZ){}
	Vector3D(float size) : x(size), y(size), z(size){}

	void Set(float xP, float yP, float zP);
	float LengthSq() const;
	float Length() const;
	void Normalize();

	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}

	// Vector addition (a + b)
	friend Vector3D operator+(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	// Vector subtraction (a - b)
	friend Vector3D operator-(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	// Component-wise multiplication
	friend Vector3D operator*(const Vector3D& left, const Vector3D& right)
	{
		return Vector3D(left.x * right.x, left.y * right.y, left.z * right.z);
	}

	// Scalar multiplication
	friend Vector3D operator*(const Vector3D& vec, float scalar)
	{
		return Vector3D(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar multiplication
	friend Vector3D operator*(float scalar, const Vector3D& vec)
	{
		return Vector3D(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	friend Vector3D operator/(const Vector3D& left, const Vector3D& right)
	{
		return Vector3D(left.x / right.x, left.y / right.y, left.z / right.z);
	}

	// Scalar division
	friend Vector3D operator/(const Vector3D& vec, float scalar)
	{
		return Vector3D(vec.x / scalar, vec.y / scalar, vec.z / scalar);
	}

	// Scalar /=
	Vector3D& operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Scalar *=
	Vector3D& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	// Vector +=
	Vector3D& operator+=(const Vector3D& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	// Vector -=
	Vector3D& operator-=(const Vector3D& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	Vector3D operator-()
	{
		Vector3D temp;
		temp.x = -x;
		temp.y = -y;
		temp.z = -z;
		return temp;
	}

	// Normalize the provided vector
	static Vector3D Normalize(const Vector3D& vec)
	{
		Vector3D temp = vec;
		temp.Normalize();
		return temp;
	}

	// Dot product between two vectors (a dot b)
	static float Dot(const Vector3D& a, const Vector3D& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	static Vector3D Min(const Vector3D& a, const Vector3D& b) {
		return Vector3D(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}
	static Vector3D Max(const Vector3D& a, const Vector3D& b) {
		return Vector3D(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}

	// Cross product between two vectors (a cross b)
	static Vector3D Cross(const Vector3D& a, const Vector3D& b)
	{
		Vector3D temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}

	friend Vector3D Normalize(Vector3D temp) { 
		return temp / temp.Length(); 
	}

	inline void Clamp(Vector3D& min, Vector3D& max) {
		if ((*this).x < min.x) (*this).x = min.x;
		if ((*this).x > max.x) (*this).x = max.x;

		if ((*this).y < min.y) (*this).y = min.y;
		if ((*this).y > max.y) (*this).y = max.y;

		if ((*this).z < min.z) (*this).z = min.z;
		if ((*this).z > max.z) (*this).z = max.z;
	}

	friend Vector3D Clamp(Vector3D temp, Vector3D& min, Vector3D& max) {
		if (temp.x < min.x) temp.x = min.x;
		if (temp.x > max.x) temp.x = max.x;

		if (temp.y < min.y) temp.y = min.y;
		if (temp.y > max.y) temp.y = max.y;

		if (temp.z < min.z) temp.z = min.z;
		if (temp.z > max.z) temp.z = max.z;

		return temp;
	}

	inline float Distance(Vector3D& vec) const {
		return sqrt(((*this).x - vec.x) * ((*this).x - vec.x) + ((*this).y - vec.y) * ((*this).y - vec.y) + ((*this).z - vec.z) * ((*this).z - vec.z));
	}

	static float Distance(const Vector3D& a, const Vector3D& b) {
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}

	inline bool Equals(Vector3D& right, float acceptance) {
		if (x < right.x - acceptance || x > right.y + acceptance || y < right.y - acceptance || y > right.y + acceptance || z < right.z - acceptance || z > right.z + acceptance) return true;
		else return false;
	}

	inline std::string ToString() {
		return "( " + std::to_string(x) + " , " + std::to_string(y) + " , " + std::to_string(z) + " )";
	}

	// Lerp from A to B by f
	static Vector3D Lerp(const Vector3D& a, const Vector3D& b, float f)
	{
		return Vector3D(a + f * (b - a));
	}

	// Reflect V about (normalized) N
	static Vector3D Reflect(const Vector3D& v, const Vector3D& n)
	{
		return v - 2.0f * Vector3D::Dot(v, n) * n;
	}

	static Vector3D Transform(Vector3D& vec, class Matrix4D& mat, float w = 1.0f);

	// This will transform the vector and renormalize the w component
	static Vector3D TransformWithPerspDiv(Vector3D& vec, class Matrix4D& mat, float w = 1.0f);

	// Transform a Vector3 by a quaternion
	static Vector3D Transform(const Vector3D& v, const class Quaternion& q);

	static const Vector3D zero;
	static const Vector3D unitX;
	static const Vector3D unitY;
	static const Vector3D unitZ;
	static const Vector3D negUnitX;
	static const Vector3D negUnitY;
	static const Vector3D negUnitZ;
	static const Vector3D infinity;
	static const Vector3D negInfinity;
};
