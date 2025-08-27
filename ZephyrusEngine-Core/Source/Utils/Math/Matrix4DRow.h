#pragma once

#include <memory.h>
#include "Vector3D.h"
#include "Quaternion.h"

// Row major order 4*4 matrix
class Matrix4DRow
{
public:
	float mat[4][4];

	Matrix4DRow()
	{
		*this = Matrix4DRow::Identity;
	}

	explicit Matrix4DRow(float inMat[4][4])
	{
		memcpy(mat, inMat, 16 * sizeof(float));

	}

	float EPSILON = 1e-6f;

	bool operator==(const Matrix4DRow& other) const
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (std::fabs(mat[i][j] - other.mat[i][j]) > EPSILON)
				{
					return false;
				}
			}
		}
		return true;
	}

	// Comparaison de différence
	bool operator!=(const Matrix4DRow& other) const
	{
		return !(*this == other);
	}

	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&mat[0][0]);
	}

	// Matrix multiplication (a * b)
	friend Matrix4DRow operator*(const Matrix4DRow& a, const Matrix4DRow& b)
	{
		Matrix4DRow retVal;
		// row 0
		retVal.mat[0][0] =
			a.mat[0][0] * b.mat[0][0] +
			a.mat[0][1] * b.mat[1][0] +
			a.mat[0][2] * b.mat[2][0] +
			a.mat[0][3] * b.mat[3][0];

		retVal.mat[0][1] =
			a.mat[0][0] * b.mat[0][1] +
			a.mat[0][1] * b.mat[1][1] +
			a.mat[0][2] * b.mat[2][1] +
			a.mat[0][3] * b.mat[3][1];

		retVal.mat[0][2] =
			a.mat[0][0] * b.mat[0][2] +
			a.mat[0][1] * b.mat[1][2] +
			a.mat[0][2] * b.mat[2][2] +
			a.mat[0][3] * b.mat[3][2];

		retVal.mat[0][3] =
			a.mat[0][0] * b.mat[0][3] +
			a.mat[0][1] * b.mat[1][3] +
			a.mat[0][2] * b.mat[2][3] +
			a.mat[0][3] * b.mat[3][3];

		// row 1
		retVal.mat[1][0] =
			a.mat[1][0] * b.mat[0][0] +
			a.mat[1][1] * b.mat[1][0] +
			a.mat[1][2] * b.mat[2][0] +
			a.mat[1][3] * b.mat[3][0];

		retVal.mat[1][1] =
			a.mat[1][0] * b.mat[0][1] +
			a.mat[1][1] * b.mat[1][1] +
			a.mat[1][2] * b.mat[2][1] +
			a.mat[1][3] * b.mat[3][1];

		retVal.mat[1][2] =
			a.mat[1][0] * b.mat[0][2] +
			a.mat[1][1] * b.mat[1][2] +
			a.mat[1][2] * b.mat[2][2] +
			a.mat[1][3] * b.mat[3][2];

		retVal.mat[1][3] =
			a.mat[1][0] * b.mat[0][3] +
			a.mat[1][1] * b.mat[1][3] +
			a.mat[1][2] * b.mat[2][3] +
			a.mat[1][3] * b.mat[3][3];

		// row 2
		retVal.mat[2][0] =
			a.mat[2][0] * b.mat[0][0] +
			a.mat[2][1] * b.mat[1][0] +
			a.mat[2][2] * b.mat[2][0] +
			a.mat[2][3] * b.mat[3][0];

		retVal.mat[2][1] =
			a.mat[2][0] * b.mat[0][1] +
			a.mat[2][1] * b.mat[1][1] +
			a.mat[2][2] * b.mat[2][1] +
			a.mat[2][3] * b.mat[3][1];

		retVal.mat[2][2] =
			a.mat[2][0] * b.mat[0][2] +
			a.mat[2][1] * b.mat[1][2] +
			a.mat[2][2] * b.mat[2][2] +
			a.mat[2][3] * b.mat[3][2];

		retVal.mat[2][3] =
			a.mat[2][0] * b.mat[0][3] +
			a.mat[2][1] * b.mat[1][3] +
			a.mat[2][2] * b.mat[2][3] +
			a.mat[2][3] * b.mat[3][3];

		// row 3
		retVal.mat[3][0] =
			a.mat[3][0] * b.mat[0][0] +
			a.mat[3][1] * b.mat[1][0] +
			a.mat[3][2] * b.mat[2][0] +
			a.mat[3][3] * b.mat[3][0];

		retVal.mat[3][1] =
			a.mat[3][0] * b.mat[0][1] +
			a.mat[3][1] * b.mat[1][1] +
			a.mat[3][2] * b.mat[2][1] +
			a.mat[3][3] * b.mat[3][1];

		retVal.mat[3][2] =
			a.mat[3][0] * b.mat[0][2] +
			a.mat[3][1] * b.mat[1][2] +
			a.mat[3][2] * b.mat[2][2] +
			a.mat[3][3] * b.mat[3][2];

		retVal.mat[3][3] =
			a.mat[3][0] * b.mat[0][3] +
			a.mat[3][1] * b.mat[1][3] +
			a.mat[3][2] * b.mat[2][3] +
			a.mat[3][3] * b.mat[3][3];

		return retVal;
	}

	Matrix4DRow& operator*=(const Matrix4DRow& right)
	{
		*this = *this * right;
		return *this;
	}

	// Invert the matrix - super slow
	void Invert();

	Vector3D GetTranslation() const
	{
		return Vector3D(mat[3][0], mat[3][1], mat[3][2]);
	}

	Vector3D TransformVector(const Vector3D& vec) const
	{
		return Vector3D(
			mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z,
			mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z,
			mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z
		);
	}

	Vector3D TransformPoint(const Vector3D& point) const
	{
		Vector3D result;
		result.x = mat[0][0] * point.x + mat[0][1] * point.y + mat[0][2] * point.z + mat[0][3];
		result.y = mat[1][0] * point.x + mat[1][1] * point.y + mat[1][2] * point.z + mat[1][3];
		result.z = mat[2][0] * point.x + mat[2][1] * point.y + mat[2][2] * point.z + mat[2][3];
		return result;
	}

	Vector3D GetXAxis() const
	{
		return Vector3D::Normalize(Vector3D(mat[0][0], mat[0][1], mat[0][2]));
	}

	Vector3D GetYAxis() const
	{
		return Vector3D::Normalize(Vector3D(mat[1][0], mat[1][1], mat[1][2]));
	}

	Vector3D GetZAxis() const
	{
		return Vector3D::Normalize(Vector3D(mat[2][0], mat[2][1], mat[2][2]));
	}

	Vector3D GetScale() const
	{
		Vector3D retVal;
		retVal.x = Vector3D(mat[0][0], mat[0][1], mat[0][2]).Length();
		retVal.y = Vector3D(mat[1][0], mat[1][1], mat[1][2]).Length();
		retVal.z = Vector3D(mat[2][0], mat[2][1], mat[2][2]).Length();
		return retVal;
	}

	static Matrix4DRow DeleteTranslation(Matrix4DRow& matrix) 
	{
		float temp[4][4] =
		{
			{ matrix.mat[0][0], matrix.mat[0][1], matrix.mat[0][2], matrix.mat[0][3]},
			{ matrix.mat[1][0], matrix.mat[1][1], matrix.mat[1][2], matrix.mat[1][3]},
			{ matrix.mat[2][0], matrix.mat[2][1], matrix.mat[2][2], matrix.mat[2][3]},
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateScale(float xScale, float yScale, float zScale)
	{
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, zScale, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateScale(const Vector3D& scaleVector)
	{
		return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
	}

	static Matrix4DRow CreateScale(float scale)
	{
		return CreateScale(scale, scale, scale);
	}

	static Matrix4DRow CreateRotationX(float theta)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f , 0.0f },
			{ 0.0f, Maths::Cos(theta), -Maths::Sin(theta), 0.0f },
			{ 0.0f, Maths::Sin(theta), Maths::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateRotationY(float theta)
	{
		float temp[4][4] =
		{
			{ Maths::Cos(theta), 0.0f, Maths::Sin(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ -Maths::Sin(theta), 0.0f, Maths::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateRotationZ(float theta)
	{
		float temp[4][4] =
		{
			{ Maths::Cos(theta), -Maths::Sin(theta), 0.0f, 0.0f },
			{ Maths::Sin(theta), Maths::Cos(theta), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateTranslation(const Vector3D& trans)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateSimpleViewProj(float width, float height)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateFromQuaternion(const Quaternion& q)
	{
		float mat[4][4];

		mat[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
		mat[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
		mat[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
		mat[0][3] = 0.0f;

		mat[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
		mat[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
		mat[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
		mat[1][3] = 0.0f;

		mat[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
		mat[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
		mat[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
		mat[2][3] = 0.0f;

		mat[3][0] = 0.0f;
		mat[3][1] = 0.0f;
		mat[3][2] = 0.0f;
		mat[3][3] = 1.0f;

		return Matrix4DRow(mat);
	}

	static Matrix4DRow CreateLookAt(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
	{
		Vector3D zaxis = Vector3D::Normalize(target - eye);
		Vector3D xaxis = Vector3D::Normalize(Vector3D::Cross(up, zaxis));
		Vector3D yaxis = Vector3D::Normalize(Vector3D::Cross(zaxis, xaxis));
		Vector3D trans;
		trans.x = -Vector3D::Dot(xaxis, eye);
		trans.y = -Vector3D::Dot(yaxis, eye);
		trans.z = -Vector3D::Dot(zaxis, eye);

		float temp[4][4] =
		{
			{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
			{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
			{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreateOrtho(float width, float height, float near, float far)
	{
		float temp[4][4] =
		{
			{ 1.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, -2.0f / (far - near), 0.0f },
			{ 0.0f, 0.0f, (far + near) / (near - far), 1.0f }
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = Maths::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, far / (far - near), 1.0f },
			{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
		};
		return Matrix4DRow(temp);
	}

	static Matrix4DRow CreatePerspective(float left, float right, float bottom, float top, float near, float far)
	{
		float temp[4][4] =
		{
			{ 2 * near / (right - left), 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2 * near / (top - bottom), 0.0f, 0.0f },
			{ (right + left) / (right - left), (top + bottom) / (top - bottom), (far + near) / (near - far), -1.0f },
			{ 0.0f, 0.0f, 2 * near * far / (near - far), 0.0f }
		};
		return Matrix4DRow(temp);
	}

	static const Matrix4DRow Identity;
};