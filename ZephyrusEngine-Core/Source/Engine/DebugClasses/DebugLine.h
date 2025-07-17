#pragma once

#include "Vector3D.h"
#include "Physics/HitResult.h"

struct DebugLine
{
	DebugLine(Vector3D pStart,Vector3D pEnd,HitResult pHit)
		: Start(pStart), End(pEnd), Hit(pHit)
	{
	}
	Vector3D Start;
	Vector3D End;
	HitResult Hit;
};