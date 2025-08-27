#include "Vector3D.h"
#include "Matrix4D.h"
#include "Quaternion.h"

const Vector3D Vector3D::zero(0.0f, 0.0f, 0.f);
const Vector3D Vector3D::unitX(1.0f, 0.0f, 0.0f);
const Vector3D Vector3D::unitY(0.0f, 1.0f, 0.0f);
const Vector3D Vector3D::unitZ(0.0f, 0.0f, 1.0f);
const Vector3D Vector3D::negUnitX(-1.0f, 0.0f, 0.0f);
const Vector3D Vector3D::negUnitY(0.0f, -1.0f, 0.0f);
const Vector3D Vector3D::negUnitZ(0.0f, 0.0f, -1.0f);
const Vector3D Vector3D::infinity(Maths::INFINITY_POS, Maths::INFINITY_POS, Maths::INFINITY_POS);
const Vector3D Vector3D::negInfinity(Maths::INFINITY_NEG, Maths::INFINITY_NEG, Maths::INFINITY_NEG); 

void Vector3D::Set(float xP, float yP, float zP)
{
	x = xP;
	y = yP;
	z = zP;
}

float Vector3D::LengthSq() const
{
	return (x * x + y * y + z * z);
}

float Vector3D::Length() const
{
	return (Maths::Sqrt(LengthSq()));
}

void Vector3D::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
}



Vector3D Vector3D::Transform(Vector3D& vec, Matrix4D& mat, float w)
{
	Vector3D retVal;
	retVal.x = vec.x * mat(0, 0) + vec.y * mat(1, 0) +
		vec.z * mat(2, 0) + w * mat(3, 0);
	retVal.y = vec.x * mat(0, 1) + vec.y * mat(1, 1) +
		vec.z * mat(2, 1) + w * mat(3, 1);
	retVal.z = vec.x * mat(0, 2) + vec.y * mat(1, 2) +
		vec.z * mat(2, 2) + w * mat(3, 2);
	//ignore w since we aren't returning a new value for it...
	return retVal;
}

Vector3D Vector3D::TransformWithPerspDiv(Vector3D& vec, Matrix4D& mat, float w)
{
	Vector3D retVal;
	retVal.x = vec.x * mat(0, 0) + vec.y * mat(1, 0) +
		vec.z * mat(2, 0) + w * mat(3, 0);
	retVal.y = vec.x * mat(0, 1) + vec.y * mat(1, 1) +
		vec.z * mat(2, 1) + w * mat(3, 1);
	retVal.z = vec.x * mat(0, 2) + vec.y * mat(1, 2) +
		vec.z * mat(2, 2) + w * mat(3, 2);
	float transformedW = vec.x * mat(0, 3) + vec.y * mat(1, 3) +
		vec.z * mat(2, 3) + w * mat(3, 3);
	if (!Maths::NearZero(Maths::Abs(transformedW)))
	{
		transformedW = 1.0f / transformedW;
		retVal *= transformedW;
	}
	return retVal;
}


Vector3D Vector3D::Transform(const Vector3D& v, const Quaternion& q)
{
	// v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	Vector3D qv(q.x, q.y, q.z);
	Vector3D retVal = v;
	retVal += 2.0f * Vector3D::Cross(qv, Vector3D::Cross(qv, v) + q.w * v);
	return retVal;
}
