#pragma once

#include "Vector3D.h"
#include "Matrix4DRow.h"
#include "Physics/HitResult.h"

using Zephyrus::Physics::HitResult;

namespace Zephyrus::Debug
{
	constexpr float BoxVertices[] = {
		-0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,  -0.5f,  0.5f,  0.5f
	};
	
	struct DebugLine
	{
		DebugLine(Vector3D pStart, Vector3D pEnd, const HitResult& pHit = {}, const Vector3D& pColor = Vector3D::unitX)
			: Start(pStart), End(pEnd), Hit(pHit), Color(pColor)
		{
		}

		friend auto operator<=>(const DebugLine& a, const DebugLine& b) = default;

		Vector3D Start;
		Vector3D End;
		Vector3D Color;
		HitResult Hit;
	};
	
	struct DebugBox
	{
		DebugBox(Vector3D pPosition, Vector3D pExtents, const HitResult& pHit = {}, const Vector3D& pColor = Vector3D::unitY)
			: Position(pPosition), Extents(pExtents), Hit(pHit), Color(pColor)
		{
			for (int i = 0; i < 72; i += 3)
			{
				Vertices.push_back((Extents.x * 2 * BoxVertices[i]) + Position.x);
				Vertices.push_back((Extents.y * 2 * BoxVertices[i+1]) + Position.y);
				Vertices.push_back((Extents.z * 2 * BoxVertices[i+2]) + Position.z);
				Vertices.push_back(Color.x);
				Vertices.push_back(Color.y);
				Vertices.push_back(Color.z);
			}
		}
		
		friend auto operator<=>(const DebugBox& a, const DebugBox& b) = default;
		
		std::vector<float> GetBoxVertices() const
		{
			return Vertices;
		}
		
		std::vector<float> Vertices;
		Vector3D Position;
		Vector3D Extents;
		Vector3D Color;
		HitResult Hit;
	};

	struct PersistantDebugBox
	{
		PersistantDebugBox(const Matrix4DRow& pMatrix, const Vector3D& pColor = Vector3D::unitY, unsigned pLineWidth = 1)
			: WorldTransform(pMatrix), Color(pColor), LineWidth(pLineWidth)
		{
		}

		friend auto operator<=>(const PersistantDebugBox& a, const PersistantDebugBox& b) = default;

		Matrix4DRow WorldTransform;
		Vector3D Color;
		unsigned LineWidth;
	};

	struct Debug2DArrow
	{
		Debug2DArrow(Vector3D pStart, Vector3D pEnd, const Vector3D& pColor = Vector3D::unitX, float pArrowSize = 2.0f)
			: Start(pStart), End(pEnd), Color(pColor), ArrowSize(pArrowSize)
		{
			LineVertices.push_back(Start.x);
			LineVertices.push_back(Start.y);
			LineVertices.push_back(Start.z);

			LineVertices.push_back(Color.x);
			LineVertices.push_back(Color.y);
			LineVertices.push_back(Color.z);

			LineVertices.push_back(End.x);
			LineVertices.push_back(End.y);
			LineVertices.push_back(End.z);

			LineVertices.push_back(Color.x);
			LineVertices.push_back(Color.y);
			LineVertices.push_back(Color.z);

			Vector3D dir = Vector3D::Normalize(Start - End);
			Vector3D tempUp = Vector3D(0, 0, 1);
			if (zpMaths::Abs(Vector3D::Dot(dir, tempUp)) > 0.99f) {
				tempUp = Vector3D(0, 1, 0);
			}

			Vector3D right = Vector3D::Normalize(Vector3D::Cross(dir, tempUp));
			Vector3D up = Vector3D::Normalize(Vector3D::Cross(right, dir));

			Vector3D arrowBase = End + (dir * 0.25 * ArrowSize) + (dir * 0.1);

			Vector3D v1 = arrowBase + (right * 0.125 * ArrowSize);
			Vector3D v2 = arrowBase - (right * 0.125 * ArrowSize);

			TriangleVertices.push_back(End.x + (dir.x * 0.1));
			TriangleVertices.push_back(End.y + (dir.y * 0.1));
			TriangleVertices.push_back(End.z + (dir.z * 0.1));

			TriangleVertices.push_back(Color.x);
			TriangleVertices.push_back(Color.y);
			TriangleVertices.push_back(Color.z);

			TriangleVertices.push_back(v1.x);
			TriangleVertices.push_back(v1.y);
			TriangleVertices.push_back(v1.z);

			TriangleVertices.push_back(Color.x);
			TriangleVertices.push_back(Color.y);
			TriangleVertices.push_back(Color.z);

			TriangleVertices.push_back(v2.x);
			TriangleVertices.push_back(v2.y);
			TriangleVertices.push_back(v2.z);

			TriangleVertices.push_back(Color.x);
			TriangleVertices.push_back(Color.y);
			TriangleVertices.push_back(Color.z);
		}

		friend auto operator<=>(const Debug2DArrow& a, const Debug2DArrow& b) = default;

		std::vector<float> LineVertices;
		std::vector<float> TriangleVertices;

		Vector3D Start;
		Vector3D End;
		Vector3D Color;

		float ArrowSize;

		std::vector<float> GetLineVertices() const
		{
			return LineVertices;
		}

		std::vector<float> GetTriangleVertices() const
		{
			return TriangleVertices; 
		}
	};
}