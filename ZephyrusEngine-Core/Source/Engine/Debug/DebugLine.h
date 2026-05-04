#pragma once

#include "Vector3D.h"
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
		
		std::vector<float> getBoxVertices() const
		{
			return Vertices;
		}
		
		std::vector<float> Vertices;
		Vector3D Position;
		Vector3D Extents;
		Vector3D Color;
		HitResult Hit;
	};
}