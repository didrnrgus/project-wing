#pragma once

#include "GameInfo.h"

namespace ECCWResult
{
	enum Type
	{
		CW = -1,
		Line,
		CCW,
	};
}

class CCollision
{
public:
	static bool CollisionAABB2DToAABB2D(FVector3D& HitPoint,
		class CColliderAABB2D* Src,
		class CColliderAABB2D* Dest);
	static bool CollisionSphere2DToSphere2D(FVector3D& HitPoint,
		class CColliderSphere2D* Src,
		class CColliderSphere2D* Dest);
	static bool CollisionAABB2DToSphere2D(FVector3D& HitPoint,
		class CColliderAABB2D* Src,
		class CColliderSphere2D* Dest);
	static bool CollisionOBB2DToOBB2D(FVector3D& HitPoint,
		class CColliderOBB2D* Src,
		class CColliderOBB2D* Dest);
	static bool CollisionAABB2DToOBB2D(FVector3D& HitPoint,
		class CColliderAABB2D* Src,
		class CColliderOBB2D* Dest);
	static bool CollisionSphere2DToOBB2D(FVector3D& HitPoint,
		class CColliderSphere2D* Src,
		class CColliderOBB2D* Dest);
	static bool CollisionLine2DToLine2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderLine2D* Dest);
	static bool CollisionLine2DToAABB2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderAABB2D* Dest);
	static bool CollisionLine2DToOBB2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderOBB2D* Dest);
	static bool CollisionLine2DToSphere2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderSphere2D* Dest);

	static bool CollisionAABB2DToAABB2D(FVector3D& HitPoint,
		const FAABB2D& Src,	const FAABB2D& Dest);
	static bool CollisionSphere2DToSphere2D(FVector3D& HitPoint,
		const FVector3D& SrcCenter, float SrcRadius,
		const FVector3D& DestCenter, float DestRadius);
	static bool CollisionAABB2DToSphere2D(FVector3D& HitPoint,
		const FAABB2D& Src,
		const FVector3D& DestCenter, float DestRadius);
	static bool CollisionOBB2DToOBB2D(FVector3D& HitPoint,
		const FOBB2D& Src, const FOBB2D& Dest);
	static bool CollisionAABB2DToOBB2D(FVector3D& HitPoint,
		const FAABB2D& Src, const FOBB2D& Dest);
	static bool CollisionSphere2DToOBB2D(FVector3D& HitPoint,
		const FVector3D& SrcCenter, float SrcRadius,
		const FOBB2D& Dest);
	static bool CollisionLine2DToLine2D(FVector3D& HitPoint,
		const FLine2D& Src, const FLine2D& Dest);
	static bool CollisionLine2DToAABB2D(FVector3D& HitPoint,
		const FLine2D& Src, const FAABB2D& Dest);
	static bool CollisionLine2DToOBB2D(FVector3D& HitPoint,
		const FLine2D& Src, const FOBB2D& Dest);
	static bool CollisionLine2DToSphere2D(FVector3D& HitPoint,
		const FLine2D& Src, const FVector3D& Center,
		float Radius);

private:
	static bool ComputeAxisProjection(const FVector2D& CenterLine,
		const FVector2D& SeparationAxis,
		float AxisHalfSize, const FVector2D* DestAxis,
		const FVector2D& DestHalfSize);

	static FAABB2D CreateAABB2D(const FOBB2D& Info);
	static FAABB2D CreateAABB2D(const FVector2D& Center,
		float Radius);
	static FOBB2D CreateOBB2D(const FAABB2D& Info);

public:
	static bool CollisionPointToAABB2D(
		const FVector2D& Point, const FAABB2D& Info);

	static bool CollisionPointToOBB2D(
		const FVector2D& Point, const FOBB2D& Info);

	static bool CollisionPointToSphere2D(
		const FVector2D& Point, const FVector2D& Center,
		float Radius);

	static bool CollisionPointToLine2D(
		const FVector2D& Point, const FLine2D& Info);

	// 인자로 들어오는 3개의 점이 시계방향인지, 
	// 반시계 방향인지, 일직선상에 존재하는지를 판단한다.
	static ECCWResult::Type CCW2D(const FVector2D& P1,
		const FVector2D& P2, const FVector2D& P3);

	static bool PointOnLine2D(const FVector2D& LineStart,
		const FVector2D& LineEnd,
		const FVector2D& Point);
};

