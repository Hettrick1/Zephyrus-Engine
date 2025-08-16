#include "Quaternion.h"
#include "Matrix4D.h"
#include "Matrix4DRow.h"
#include <array>

using std::array;

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion(float xP, float yP, float zP, float wP)
{
	Set(xP, yP, zP, wP);
}

Quaternion::Quaternion(const Vector3D& axis, float angle)
{
	float scalar = Maths::Sin(angle / 2.0f);
	x = axis.x * scalar;
	y = axis.y * scalar;
	z = axis.z * scalar;
	w = Maths::Cos(angle / 2.0f);
}

Quaternion::Quaternion(const Vector3D& axis)
{
	float cy = cos(Maths::ToRad(axis.z) * 0.5f);
	float sy = sin(Maths::ToRad(axis.z) * 0.5f);
	float cp = cos(Maths::ToRad(axis.y) * 0.5f);
	float sp = sin(Maths::ToRad(axis.y) * 0.5f);
	float cr = cos(Maths::ToRad(axis.x) * 0.5f);
	float sr = sin(Maths::ToRad(axis.x) * 0.5f);

	w = cr * cp * cy + sr * sp * sy;
	x = sr * cp * cy - cr * sp * sy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
}

void Quaternion::Set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

void Quaternion::Conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

Quaternion Quaternion::ConjugateQuat()
{
	return Quaternion(-x, -y, -z, w);
}

Vector3D Quaternion::ToEuler() const
{
	Vector3D euler;

	// --- Pitch (X axis) ---
	float sinp = 2.0f * (w * x + y * z);
	float cosp = 1.0f - 2.0f * (x * x + y * y);
	euler.x = Maths::ToDeg(std::atan2(sinp, cosp));

	// --- Roll (Y axis) ---
	float sinr = 2.0f * (w * y - z * x);
	if (std::fabs(sinr) >= 1.0f)
		euler.y = Maths::ToDeg(std::copysign(Maths::PI / 2.0f, sinr)); // clamp à 90°
	else
		euler.y = Maths::ToDeg(std::asin(sinr));

	// --- Yaw (Z axis) ---
	float siny = 2.0f * (w * z + x * y);
	float cosy = 1.0f - 2.0f * (y * y + z * z);
	euler.z = Maths::ToDeg(std::atan2(siny, cosy));

	return euler;
}

void Quaternion::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

Matrix4D Quaternion::AsMatrix() const
{
	// Transposed?

	const float xx = x * x;
	const float yy = y * y;
	const float zz = z * z;
	const float ww = w * w;
	const float xy = x * y;
	const float xz = x * z;
	const float xw = x * w;
	const float yz = y * z;
	const float yw = y * w;
	const float zw = z * w;


	array<float,16> temp;

	temp[0] = 1.0f - 2.0f * (yy + zz);
	temp[1] =        2.0f * (xy - zw);
	temp[2] =        2.0f * (xz + yw);
	temp[3] =        0.0f;

	temp[4] =        2.0f * (xy + zw);
	temp[5] = 1.0f - 2.0f * (xx + zz);
	temp[6] =        2.0f * (yz - xw);
	temp[7] =        0.0f;

	temp[8] =        2.0f * (xz - yw);
	temp[9] =        2.0f * (yz + xw);
	temp[10] = 1.0f - 2.0f * (xx + yy);
	temp[11] =        0.0f;

	temp[12] =        0.0f;
	temp[13] =        0.0f;
	temp[14] =        0.0f;
	temp[15] =        1.0f;

	Matrix4D m = Matrix4D(temp);
	return m;
}

Matrix4DRow Quaternion::AsMatrixRow() const
{
	Matrix4DRow m;

	const float xx = x * x;
	const float yy = y * y;
	const float zz = z * z;
	const float ww = w * w;
	const float xy = x * y;
	const float xz = x * z;
	const float xw = x * w;
	const float yz = y * z;
	const float yw = y * w;
	const float zw = z * w;

	m.mat[0][0] = 1.0f - 2.0f * (yy + zz);
	m.mat[0][1] =        2.0f * (xy - zw);
	m.mat[0][2] =        2.0f * (xz + yw);
	m.mat[0][3] =        0.0f;

	m.mat[1][0] =        2.0f * (xy + zw);
	m.mat[1][1] = 1.0f - 2.0f * (xx + zz);
	m.mat[1][2] =        2.0f * (yz - xw);
	m.mat[1][3] =        0.0f;

	m.mat[2][0] =        2.0f * (xz - yw);
	m.mat[2][1] =        2.0f * (yz + xw);
	m.mat[2][2] = 1.0f - 2.0f * (xx + yy);
	m.mat[2][3] =        0.0f;

	m.mat[3][0] =        0.0f;
	m.mat[3][1] =        0.0f;
	m.mat[3][2] =        0.0f;
	m.mat[3][3] =        1.0f;

	return m;
}
