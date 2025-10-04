# pragma once

#include <array>

#include "Maths.h"
#include "Quaternion.h"

using std::array;

// Col major order 4*4 matrix
class Matrix4D
{
public:
	array<float, 16> mat;

	Matrix4D()
	{
		*this = Matrix4D::Identity;
	}

	Matrix4D(const array<float, 16>& that)
    {
        for(int n = 0; n < 16; ++n) {
			mat[n] = that[n];
		}
    }

	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&mat[0]);
	}

	inline float& operator()(const int i, const int j)
	{
		return mat[i * 4 + j];
	}

	inline Matrix4D& operator=(const Matrix4D& that)
    {
        for(int n = 0; n < 16; ++n) {
			mat[n] = that.mat[n];
		}
		return *this;
    }

	inline Matrix4D operator+(const Matrix4D& that) const
    {
        Matrix4D result;
        int n;
        for (n = 0; n < 16; n++)
            result.mat[n] = mat[n] + that.mat[n];
        return result;
    }

    inline Matrix4D& operator+=(const Matrix4D& that)
    {
        return (*this = *this + that);
    }

    inline Matrix4D operator-(const Matrix4D& that) const
    {
        Matrix4D result;
        int n;
        for (n = 0; n < 16; n++)
            result.mat[n] = mat[n] - that.mat[n];
        return result;
    }

    inline Matrix4D& operator-=(const Matrix4D& that)
    {
        return (*this = *this - that);
    }

	// Matrix multiplication (a * b)
	friend Matrix4D operator*(Matrix4D& a, Matrix4D& b)
	{
		Matrix4D retVal;

		retVal(0, 0) = 
		a(0,0) * b(0,0) +
		a(1,0) * b(0,1) +
		a(2,0) * b(0,2) +
		a(3,0) * b(0,3);

		retVal(0, 1) = 
		a(0,1) * b(0,0) +
		a(1,1) * b(0,1) +
		a(2,1) * b(0,2) +
		a(3,1) * b(0,3);

		retVal(0, 2) = 
		a(0,2) * b(0,0) +
		a(1,2) * b(0,1) +
		a(2,2) * b(0,2) +
		a(3,2) * b(0,3);

		retVal(0, 3) = 
		a(0,3) * b(0,0) +
		a(1,3) * b(0,1) +
		a(2,3) * b(0,2) +
		a(3,3) * b(0,3);

		retVal(1, 0) = 
		a(0,0) * b(1,0) +
		a(1,0) * b(1,1) +
		a(2,0) * b(1,2) +
		a(3,0) * b(1,3);

		retVal(1, 1) = 
		a(0,1) * b(1,0) +
		a(1,1) * b(1,1) +
		a(2,1) * b(1,2) +
		a(3,1) * b(1,3);

		retVal(1, 2) = 
		a(0,2) * b(1,0) +
		a(1,2) * b(1,1) +
		a(2,2) * b(1,2) +
		a(3,2) * b(1,3);

		retVal(1, 3) = 
		a(0,3) * b(1,0) +
		a(1,3) * b(1,1) +
		a(2,3) * b(1,2) +
		a(3,3) * b(1,3);

		retVal(2, 0) = 
		a(0,0) * b(2,0) +
		a(1,0) * b(2,1) +
		a(2,0) * b(2,2) +
		a(3,0) * b(2,3);

		retVal(2, 1) = 
		a(0,1) * b(2,0) +
		a(1,1) * b(2,1) +
		a(2,1) * b(2,2) +
		a(3,1) * b(2,3);

		retVal(2, 2) = 
		a(0,2) * b(2,0) +
		a(1,2) * b(2,1) +
		a(2,2) * b(2,2) +
		a(3,2) * b(2,3);

		retVal(2, 3) = 
		a(0,3) * b(2,0) +
		a(1,3) * b(2,1) +
		a(2,3) * b(2,2) +
		a(3,3) * b(2,3);

		retVal(3, 0) = 
		a(0,0) * b(3,0) +
		a(1,0) * b(3,1) +
		a(2,0) * b(3,2) +
		a(3,0) * b(3,3);

		retVal(3, 1) = 
		a(0,1) * b(3,0) +
		a(1,1) * b(3,1) +
		a(2,1) * b(3,2) +
		a(3,1) * b(3,3);

		retVal(3, 2) = 
		a(0,2) * b(3,0) +
		a(1,2) * b(3,1) +
		a(2,2) * b(3,2) +
		a(3,2) * b(3,3);

		retVal(3, 3) = 
		a(0,3) * b(3,0) +
		a(1,3) * b(3,1) +
		a(2,3) * b(3,2) +
		a(3,3) * b(3,3);

		return retVal;
	}

	Matrix4D& operator*=(Matrix4D& right)
	{
		*this = *this * right;
		return *this;
	}

	// Invert the matrix - super slow
	void Invert();

	Vector3D GetTranslation() const
	{

		return Vector3D(mat[12], mat[13], mat[14]);
	}

	Vector3D GetXAxis() const
	{
		return Vector3D::Normalize(Vector3D(mat[0], mat[1], mat[2]));
	}

	Vector3D GetYAxis() const
	{
		return Vector3D::Normalize(Vector3D(mat[4], mat[5], mat[6]));
	}

	Vector3D GetZAxis() const
	{
		return Vector3D::Normalize(Vector3D(mat[8], mat[9], mat[10]));
	}

	Vector3D GetScale() const
	{
		Vector3D retVal;
		retVal.x = Vector3D(mat[0], mat[1], mat[2]).Length();
		retVal.y = Vector3D(mat[4], mat[5], mat[6]).Length();
		retVal.z = Vector3D(mat[8], mat[9], mat[10]).Length();
		return retVal;
	}

	static Matrix4D CreateScale(float xScale, float yScale, float zScale)
	{
		array<float, 16> temp =
		{
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, zScale, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreateScale(const Vector3D& scaleVector)
	{
		return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
	}

	static Matrix4D CreateScale(float scale)
	{
		return CreateScale(scale, scale, scale);
	}

	static Matrix4D CreateRotationX(float theta)
	{
		array<float, 16> temp =
		{
			1.0f, 0.0f, 0.0f , 0.0f,
			0.0f, Maths::Cos(theta), -Maths::Sin(theta), 0.0f,
			0.0f, Maths::Sin(theta), Maths::Cos(theta), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreateRotationY(float theta)
	{
		array<float, 16> temp =
		{
			Maths::Cos(theta), 0.0f, Maths::Sin(theta), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-Maths::Sin(theta), 0.0f, Maths::Cos(theta), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreateRotationZ(float theta)
	{
		array<float, 16> temp =
		{
			Maths::Cos(theta), -Maths::Sin(theta), 0.0f, 0.0f,
			Maths::Sin(theta), Maths::Cos(theta), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreateTranslation(const Vector3D& trans)
	{
		array<float, 16> temp =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			trans.x, trans.y, trans.z, 1.0f
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreateSimpleViewProj(float width, float height)
	{
		array<float, 16> temp =
		{
			2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreateFromQuaternion(const Quaternion& q)
	{
		array<float, 16> temp;
		temp[0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
		temp[1] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
		temp[2] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
		temp[3] = 0.0f;

		temp[4] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
		temp[5] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
		temp[6] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
		temp[7] = 0.0f;

		temp[8] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
		temp[9] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
		temp[10] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
		temp[11] = 0.0f;

		temp[12] = 0.0f;
		temp[13] = 0.0f;
		temp[14] = 0.0f;
		temp[15] = 1.0f;

		return Matrix4D(temp);
	}

	static Matrix4D CreateLookAt(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
	{
		Vector3D zaxis = Vector3D::Normalize(eye - target);
		Vector3D normalizedUp = Vector3D::Normalize(up);
		Vector3D xaxis = Vector3D::Normalize(Vector3D::Cross(normalizedUp, zaxis));
		Vector3D yaxis = Vector3D::Normalize(Vector3D::Cross(zaxis, xaxis));

		array<float, 16> temp =
		{
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		Matrix4D t = Matrix4D(temp);
		Matrix4D translation = Matrix4D::CreateTranslation(Vector3D(-eye.x, -eye.y, -eye.z));
		return t* translation;
	}

	static Matrix4D CreateOrtho(float width, float height, float near, float far)
	{
		array<float, 16> temp =
		{
			2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, 2.0f / (near - far), 0.0f,
			0.0f, 0.0f, (far + near) / (far - far), 1.0f
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = Maths::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		array<float, 16> temp =
		{
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, near + far / (near - far), -1.0f,
			0.0f, 0.0f,  2.0f * near * far / (near - far), 0.0f
		};
		return Matrix4D(temp);
	}

	static Matrix4D CreatePerspective(float left, float right, float bottom, float top, float near, float far)
	{
		array<float, 16> temp =
		{
			2 * near / (right - left), 0.0f, 0.0f, 0.0f,
			0.0f, 2 * near / (top - bottom), 0.0f, 0.0f,
			(right + left) / (right - left), (top + bottom) / (top - bottom), (far + near) / (near - far), -1.0f,
			0.0f, 0.0f, 2 * near * far / (near - far), 0.0f
		};
		return Matrix4D(temp);
	}

	static const Matrix4D Identity;
};
