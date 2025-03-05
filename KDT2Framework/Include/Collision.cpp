#include "Collision.h"
#include "Component/ColliderAABB2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderLine2D.h"

bool CCollision::CollisionAABB2DToAABB2D(FVector3D& HitPoint,
    CColliderAABB2D* Src, CColliderAABB2D* Dest)
{
    if(CollisionAABB2DToAABB2D(HitPoint, Src->GetBox(), 
        Dest->GetBox()))
        return true;

    return false;
}

bool CCollision::CollisionSphere2DToSphere2D(
    FVector3D& HitPoint, CColliderSphere2D* Src,
    CColliderSphere2D* Dest)
{
    if (CollisionSphere2DToSphere2D(HitPoint,
        Src->GetWorldPosition(), Src->GetRadius(),
        Dest->GetWorldPosition(),
        Dest->GetRadius()))
        return true;

    return false;
}

bool CCollision::CollisionAABB2DToSphere2D(
    FVector3D& HitPoint, CColliderAABB2D* Src,
    CColliderSphere2D* Dest)
{
    if (CollisionAABB2DToSphere2D(HitPoint,
        Src->GetBox(),
        Dest->GetWorldPosition(),
        Dest->GetRadius()))
        return true;

    return false;
}

bool CCollision::CollisionOBB2DToOBB2D(
    FVector3D& HitPoint, 
    CColliderOBB2D* Src, CColliderOBB2D* Dest)
{
    if (CollisionOBB2DToOBB2D(HitPoint, Src->GetBox(),
        Dest->GetBox()))
        return true;

    return false;
}

bool CCollision::CollisionAABB2DToOBB2D(
    FVector3D& HitPoint, CColliderAABB2D* Src,
    CColliderOBB2D* Dest)
{
    if (CollisionAABB2DToOBB2D(HitPoint, Src->GetBox(),
        Dest->GetBox()))
        return true;

    return false;
}

bool CCollision::CollisionSphere2DToOBB2D(
    FVector3D& HitPoint, CColliderSphere2D* Src,
    CColliderOBB2D* Dest)
{
    if (CollisionSphere2DToOBB2D(HitPoint, 
        Src->GetWorldPosition(),
        Src->GetRadius(),
        Dest->GetBox()))
        return true;

    return false;
}

bool CCollision::CollisionLine2DToLine2D(
    FVector3D& HitPoint, CColliderLine2D* Src,
    CColliderLine2D* Dest)
{
    if (CollisionLine2DToLine2D(HitPoint, Src->GetLine(),
        Dest->GetLine()))
        return true;

    return false;
}

bool CCollision::CollisionLine2DToAABB2D(
    FVector3D& HitPoint, CColliderLine2D* Src, 
    CColliderAABB2D* Dest)
{
    if (CollisionLine2DToAABB2D(HitPoint, Src->GetLine(),
        Dest->GetBox()))
        return true;

    return false;
}

bool CCollision::CollisionLine2DToOBB2D(
    FVector3D& HitPoint, CColliderLine2D* Src, 
    CColliderOBB2D* Dest)
{
    if (CollisionLine2DToOBB2D(HitPoint, Src->GetLine(),
        Dest->GetBox()))
        return true;

    return false;
}

bool CCollision::CollisionLine2DToSphere2D(
    FVector3D& HitPoint, CColliderLine2D* Src,
    CColliderSphere2D* Dest)
{
    if (CollisionLine2DToSphere2D(HitPoint, Src->GetLine(),
        Dest->GetWorldPosition(),
        Dest->GetRadius()))
        return true;

    return false;
}

bool CCollision::CollisionAABB2DToAABB2D(FVector3D& HitPoint, 
    const FAABB2D& Src, const FAABB2D& Dest)
{
    if (Src.Min.x > Dest.Max.x)
        return false;

    else if (Dest.Min.x > Src.Max.x)
        return false;

    else if (Src.Min.y > Dest.Max.y)
        return false;

    else if (Dest.Min.y > Src.Max.y)
        return false;

    FVector3D   Min, Max;
    Min.x = Src.Min.x > Dest.Min.x ? Src.Min.x : Dest.Min.x;
    Min.y = Src.Min.y > Dest.Min.y ? Src.Min.y : Dest.Min.y;

    Max.x = Src.Max.x < Dest.Max.x ? Src.Max.x : Dest.Max.x;
    Max.y = Src.Max.y < Dest.Max.y ? Src.Max.y : Dest.Max.y;

    HitPoint.x = (Min.x + Max.x) * 0.5f;
    HitPoint.y = (Min.y + Max.y) * 0.5f;

    return true;
}

bool CCollision::CollisionSphere2DToSphere2D(
    FVector3D& HitPoint, const FVector3D& SrcCenter, 
    float SrcRadius, 
    const FVector3D& DestCenter, float DestRadius)
{
    float Dist = SrcCenter.Distance(DestCenter);

    if (Dist <= SrcRadius + DestRadius)
    {
        float   Gap = SrcRadius + DestRadius - Dist;
        Gap *= 0.5f;

        FVector3D   Dir = DestCenter - SrcCenter;
        Dir.Normalize();

        HitPoint = SrcCenter + Dir * (SrcRadius - Gap);

        return true;
    }

    return false;
}

bool CCollision::CollisionAABB2DToSphere2D(
    FVector3D& HitPoint, const FAABB2D& Src,
    const FVector3D& DestCenter, float DestRadius)
{
    // 원의 중점의 x값이 원본 사각형의 좌, 우 사이에 있거나
    // 원의 중점의 y값이 원본 사각형의 위, 아래 사이에
    // 있을 경우 사각형을 원의 반지름만큼 확장하여 원의
    // 중점이 확장된 사각형 안에 있는지 판단한다.
    if ((Src.Min.x <= DestCenter.x && DestCenter.x <= Src.Max.x) ||
        (Src.Min.y <= DestCenter.y && DestCenter.y <= Src.Max.y))
    {
        // 사각형을 원의 반지름만큼 확장한다.
        FAABB2D SrcExtent;
        SrcExtent.Min = Src.Min - DestRadius;
        SrcExtent.Max = Src.Max + DestRadius;

        // 확장된 사각형 안에 원의 중점이 들어왔는지
        // 판단한다.
        if (SrcExtent.Min.x > DestCenter.x)
            return false;

        else if (SrcExtent.Max.x < DestCenter.x)
            return false;

        else if (SrcExtent.Min.y > DestCenter.y)
            return false;

        else if (SrcExtent.Max.y < DestCenter.y)
            return false;

        FAABB2D SphereAABB;

        SphereAABB.Min.x = DestCenter.x - DestRadius;
        SphereAABB.Min.y = DestCenter.y - DestRadius;
        SphereAABB.Max.x = DestCenter.x + DestRadius;
        SphereAABB.Max.y = DestCenter.y + DestRadius;

        FVector3D   Min, Max;
        Min.x = Src.Min.x > SphereAABB.Min.x ? Src.Min.x : SphereAABB.Min.x;
        Min.y = Src.Min.y > SphereAABB.Min.y ? Src.Min.y : SphereAABB.Min.y;

        Max.x = Src.Max.x < SphereAABB.Max.x ? Src.Max.x : SphereAABB.Max.x;
        Max.y = Src.Max.y < SphereAABB.Max.y ? Src.Max.y : SphereAABB.Max.y;

        HitPoint.x = (Min.x + Max.x) * 0.5f;
        HitPoint.y = (Min.y + Max.y) * 0.5f;

        return true;
    }

    // 대각 영역일 경우 해당 방향의 사각형 꼭지점을 구해준다.
    FVector3D   Vertex;

    // 왼쪽 대각선 2 영역일 경우
    if (DestCenter.x < Src.Min.x)
    {
        Vertex.x = Src.Min.x;
    }

    // 오른쪽 대각선 2 영역일 경우
    else
    {
        Vertex.x = Src.Max.x;
    }

    // 아래쪽 영역일 경우
    if (DestCenter.y < Src.Min.y)
    {
        Vertex.y = Src.Min.y;
    }

    else
    {
        Vertex.y = Src.Max.y;
    }

    // 구해준 사각형의 꼭지점이 원 안에 들어가는지를 판단한다.
    float Dist = Vertex.Distance(DestCenter);

    if (Dist <= DestRadius)
    {
        HitPoint = Vertex;

        return true;
    }

    return false;
}

bool CCollision::CollisionOBB2DToOBB2D(
    FVector3D& HitPoint, 
    const FOBB2D& Src, const FOBB2D& Dest)
{
    FVector2D   CenterLine = Src.Center - Dest.Center;

    if (!ComputeAxisProjection(CenterLine,
        Src.Axis[EAxis::X], Src.HalfSize.x,
        Dest.Axis, Dest.HalfSize))
        return false;

    if (!ComputeAxisProjection(CenterLine,
        Src.Axis[EAxis::Y], Src.HalfSize.y,
        Dest.Axis, Dest.HalfSize))
        return false;

    if (!ComputeAxisProjection(CenterLine,
        Dest.Axis[EAxis::X], Dest.HalfSize.x,
        Src.Axis, Src.HalfSize))
        return false;

    if (!ComputeAxisProjection(CenterLine,
        Dest.Axis[EAxis::Y], Dest.HalfSize.y,
        Src.Axis, Src.HalfSize))
        return false;

    // 상자 2개를 AABB2D 정보로 만들어서 두 교집합의
    // 중점을 구한다.
    FAABB2D Hit1, Hit2;

    Hit1 = CreateAABB2D(Src);
    Hit2 = CreateAABB2D(Dest);

    FVector3D   Min, Max;
    Min.x = Hit1.Min.x > Hit2.Min.x ? Hit1.Min.x : Hit2.Min.x;
    Min.y = Hit1.Min.y > Hit2.Min.y ? Hit1.Min.y : Hit2.Min.y;

    Max.x = Hit1.Max.x < Hit2.Max.x ? Hit1.Max.x : Hit2.Max.x;
    Max.y = Hit1.Max.y < Hit2.Max.y ? Hit1.Max.y : Hit2.Max.y;

    HitPoint.x = (Min.x + Max.x) * 0.5f;
    HitPoint.y = (Min.y + Max.y) * 0.5f;

    return true;
}

bool CCollision::CollisionAABB2DToOBB2D(
    FVector3D& HitPoint, const FAABB2D& Src,
    const FOBB2D& Dest)
{
    FOBB2D  SrcOBB = CreateOBB2D(Src);

    return CollisionOBB2DToOBB2D(HitPoint, SrcOBB, Dest);
}

bool CCollision::CollisionSphere2DToOBB2D(
    FVector3D& HitPoint, const FVector3D& SrcCenter, 
    float SrcRadius, const FOBB2D& Dest)
{
    FVector2D   ConvertCenter;
    ConvertCenter.x = SrcCenter.x;
    ConvertCenter.y = SrcCenter.y;

    FVector2D   CenterLine = ConvertCenter - Dest.Center;

    FVector2D   Axis = CenterLine;
    Axis.Normalize();

    if (!ComputeAxisProjection(CenterLine,
        Axis, SrcRadius,
        Dest.Axis, Dest.HalfSize))
        return false;

    float CenterProjectionDist = 
        abs(CenterLine.Dot(Dest.Axis[EAxis::X]));

    if (CenterProjectionDist >
        SrcRadius + Dest.HalfSize.x)
        return false;

    CenterProjectionDist = 
        abs(CenterLine.Dot(Dest.Axis[EAxis::Y]));

    if (CenterProjectionDist >
        SrcRadius + Dest.HalfSize.y)
        return false;

    // 상자 2개를 AABB2D 정보로 만들어서 두 교집합의
    // 중점을 구한다.
    FAABB2D Hit1, Hit2;

    Hit1 = CreateAABB2D(ConvertCenter, SrcRadius);
    Hit2 = CreateAABB2D(Dest);

    FVector3D   Min, Max;
    Min.x = Hit1.Min.x > Hit2.Min.x ? Hit1.Min.x : Hit2.Min.x;
    Min.y = Hit1.Min.y > Hit2.Min.y ? Hit1.Min.y : Hit2.Min.y;

    Max.x = Hit1.Max.x < Hit2.Max.x ? Hit1.Max.x : Hit2.Max.x;
    Max.y = Hit1.Max.y < Hit2.Max.y ? Hit1.Max.y : Hit2.Max.y;

    HitPoint.x = (Min.x + Max.x) * 0.5f;
    HitPoint.y = (Min.y + Max.y) * 0.5f;

    return true;
}

bool CCollision::CollisionLine2DToLine2D(
    FVector3D& HitPoint, const FLine2D& Src, 
    const FLine2D& Dest)
{
    // Src 시작점 : A Src 도착점 : B
    // Dest 시작점 : C Dest 도착점 : D
    // 선분 A, B   선분 C, D
    // 선이 서로 교차하는지 판단해야 한다.
    // CCW(A, B, C) * CCW(A, B, D) < 0
    // CCW(C, D, A) * CCW(C, D, B) < 0
    FVector2D   A = Src.Start;
    FVector2D   B = Src.End;
    FVector2D   C = Dest.Start;
    FVector2D   D = Dest.End;

    int ccw1 = CCW2D(A, B, C);
    int ccw2 = CCW2D(A, B, D);
    int ccw3 = CCW2D(C, D, A);
    int ccw4 = CCW2D(C, D, B);

    if (ccw1 * ccw2 < 0 && ccw3 * ccw4 < 0)
    {
        // P = A * t(B - A)
        // Q = C + s(D - C)
        // A * t(B - A) = C + s(D - C)
        float Det = (A.x - B.x) * (C.y - D.y) - (A.y - B.y) * (C.x - D.x);

        HitPoint.x = ((A.x * B.y - A.y * B.x) * (C.x - D.x) -
            (A.x - B.x) * (C.x * D.y - C.y * D.x)) / Det;
        HitPoint.y = ((A.x * B.y - A.y * B.x) * (C.y - D.y) -
            (A.y - B.y) * (C.x * D.y - C.y * D.x)) / Det;
        /*float   x1 = B.x - A.x;
        float   y1 = B.y - A.y;
        float   SrcResult = y1 * A.x + x1 * A.y;

        float   x2 = D.x - C.x;
        float   y2 = D.y - C.y;
        float   DestResult = y2 * C.x + x2 * C.y;

        float   Determinant = y1 * x2 - x1 * y2;

        HitPoint.x = (x2 * SrcResult - y2 * DestResult) / Determinant;
        HitPoint.y = (y1 * DestResult - x1 * SrcResult) / Determinant;*/

        return true;
    }

    // 직선이 일직선 상에 있는 경우
    // 점 C가 직선 A, B 위에 있는지 판단한다.
    if (ccw1 == 0 && PointOnLine2D(A, B, C))
    {
        HitPoint.x = C.x;
        HitPoint.y = C.y;
        return true;
    }

    if (ccw2 == 0 && PointOnLine2D(A, B, D))
    {
        HitPoint.x = D.x;
        HitPoint.y = D.y;
        return true;
    }

    if (ccw3 == 0 && PointOnLine2D(C, D, A))
    {
        HitPoint.x = A.x;
        HitPoint.y = A.y;
        return true;
    }

    if (ccw4 == 0 && PointOnLine2D(C, D, B))
    {
        HitPoint.x = B.x;
        HitPoint.y = B.y;
        return true;
    }

    return false;
}

bool CCollision::CollisionLine2DToAABB2D(
    FVector3D& HitPoint, const FLine2D& Src,
    const FAABB2D& Dest)
{
    // 선을 구성하는 두 점중 하나라도 사각형 안에 들어오면
    // 충돌이다.
    if (CollisionPointToAABB2D(Src.Start, Dest))
    {
        HitPoint.x = Src.Start.x;
        HitPoint.y = Src.Start.y;

        return true;
    }

    else if (CollisionPointToAABB2D(Src.End, Dest))
    {
        HitPoint.x = Src.End.x;
        HitPoint.y = Src.End.y;

        return true;
    }

    // 사각형을 구성하는 4개의 변과 선이 충돌하는지 판단한다.
    FLine2D AABBLine[4];

    AABBLine[0].Start = Dest.Min;
    AABBLine[0].End.x = Dest.Min.x;
    AABBLine[0].End.y = Dest.Max.y;

    AABBLine[1].Start.x = Dest.Min.x;
    AABBLine[1].Start.y = Dest.Max.y;
    AABBLine[1].End = Dest.Max;

    AABBLine[2].Start = Dest.Max;
    AABBLine[2].End.x = Dest.Max.x;
    AABBLine[2].End.y = Dest.Min.y;

    AABBLine[3].Start.x = Dest.Max.x;
    AABBLine[3].Start.y = Dest.Min.y;
    AABBLine[3].End = Dest.Min;

    bool    Result = false;
    float   Dist = -1.f;

    FVector2D   HitResult;

    for (int i = 0; i < 4; ++i)
    {
        if (CollisionLine2DToLine2D(HitPoint, Src,
            AABBLine[i]))
        {
            Result = true;

            FVector2D   v;
            v.x = HitPoint.x;
            v.y = HitPoint.y;

            float   Dist1 = Src.Start.Distance(v);

            if (Dist == -1.f)
            {
                HitResult = v;
                Dist = Dist1;
            }

            else if (Dist1 < Dist)
            {
                HitResult = v;
                Dist = Dist1;
            }
        }
    }

    if (Result)
    {
        HitPoint.x = HitResult.x;
        HitPoint.y = HitResult.y;
    }

    return Result;
}

bool CCollision::CollisionLine2DToOBB2D(
    FVector3D& HitPoint, const FLine2D& Src, 
    const FOBB2D& Dest)
{
    // 선을 구성하는 두 점중 하나라도 사각형 안에 들어오면
    // 충돌이다.
    if (CollisionPointToOBB2D(Src.Start, Dest))
    {
        HitPoint.x = Src.Start.x;
        HitPoint.y = Src.Start.y;

        return true;
    }

    else if (CollisionPointToOBB2D(Src.End, Dest))
    {
        HitPoint.x = Src.End.x;
        HitPoint.y = Src.End.y;

        return true;
    }

    // 사각형을 구성하는 점 4개를 만들어준다.
    FVector2D   Pos[4];

    Pos[0] = Dest.Center - Dest.Axis[0] * Dest.HalfSize.x +
        Dest.Axis[1] * Dest.HalfSize.y;

    Pos[1] = Dest.Center - Dest.Axis[0] * Dest.HalfSize.x -
        Dest.Axis[1] * Dest.HalfSize.y;

    Pos[2] = Dest.Center + Dest.Axis[0] * Dest.HalfSize.x +
        Dest.Axis[1] * Dest.HalfSize.y;

    Pos[3] = Dest.Center + Dest.Axis[0] * Dest.HalfSize.x -
        Dest.Axis[1] * Dest.HalfSize.y;

    // 사각형을 구성하는 4개의 변과 선이 충돌하는지 판단한다.
    FLine2D OBBLine[4];

    OBBLine[0].Start = Pos[0];
    OBBLine[0].End = Pos[2];

    OBBLine[1].Start = Pos[2];
    OBBLine[1].End = Pos[3];

    OBBLine[2].Start = Pos[1];
    OBBLine[2].End = Pos[3];

    OBBLine[3].Start = Pos[0];
    OBBLine[3].End = Pos[1];

    bool    Result = false;
    float   Dist = -1.f;

    FVector2D   HitResult;

    for (int i = 0; i < 4; ++i)
    {
        if (CollisionLine2DToLine2D(HitPoint, Src,
            OBBLine[i]))
        {
            Result = true;

            FVector2D   v;
            v.x = HitPoint.x;
            v.y = HitPoint.y;

            float   Dist1 = Src.Start.Distance(v);

            if (Dist == -1.f)
            {
                HitResult = v;
                Dist = Dist1;
            }

            else if (Dist1 < Dist)
            {
                HitResult = v;
                Dist = Dist1;
            }
        }
    }

    if (Result)
    {
        HitPoint.x = HitResult.x;
        HitPoint.y = HitResult.y;
    }

    return Result;
}

bool CCollision::CollisionLine2DToSphere2D(
    FVector3D& HitPoint, const FLine2D& Src, 
    const FVector3D& Center, float Radius)
{
    // 임의의점 P가 있을 경우
    // 중점 : C 점 : P 반지름 : r
    // || P - C || - r = 0
    // 직선의방향 : D, 임의의 거리 : t
    // 직선 : S + D * t
    // 직선의 시작점에서 D 방향으로 t만큼 이동한 위치를
    // 구할 수 있다.
    // || S - C + Dt || - r = 0
    // 루트((S - C + Dt) * (S - C + Dt)) - r = 0
    // S - C : M
    // 루트((Dt + M) * (Dt + M)) - r = 0
    // (Dt + M) * (Dt + M) - r * r = 0
    // Dt^2 + 2MDt + M^2 - r^2 = 0
    // Ax^2 + Bx + C = 0
    // A는 방향벡터 D이기 때문에 제곱을 해도 크기인 1이
    // 나오게 된다.
    // (-B +- 루트(B^2 - 4AC)) / 2A
    // (-B +- 루트(B^2 - 4C)) / 2
    FVector2D   Dir = Src.End - Src.Start;

    // 선의 길이
    float   LineLength = Dir.Length();

    Dir.Normalize();

    FVector2D   Center2D;
    Center2D.x = Center.x;
    Center2D.y = Center.y;
    FVector2D   M = Src.Start - Center2D;
    float   b = 2.f * M.Dot(Dir);
    float   c = M.Dot(M) - Radius * Radius;

    float   Det = b * b - 4.f * c;

    if (Det < 0.f)
        return false;

    // 루트
    Det = sqrtf(Det);

    // (-B +- 루트(B^2 - 4C)) / 2
    float   t1, t2;
    t1 = (-b + Det) / 2.f;
    t2 = (-b - Det) / 2.f;

    if (t1 < 0.f && t2 < 0.f)
        return false;

    // 2개의 길이중 하나라도 선의 길이보다 짧아야 한다.
    // 단, 음수는 처리 X.
    bool    result = false;
    bool    Pair = false;

    if (t1 > 0.f && t1 < LineLength ||
        t2 > 0.f && t2 < LineLength)
        result = true;

    else
    {
        // 시작점과 도착점이 둘다 원 안에 있는지 판단한다.
        float   Length1 = Src.Start.Distance(Center2D);
        float   Length2 = Src.End.Distance(Center2D);

        if (Radius >= Length1 && Radius >= Length2)
        {
            Pair = true;
            result = true;
            HitPoint.x = Src.End.x;
            HitPoint.y = Src.End.y;
        }
    }

    if (result && !Pair)
    {
        float   HitDist = t1 < t2 ? t1 : t2;

        if (HitDist < 0.f)
            HitDist = t1 > t2 ? t1 : t2;

        HitPoint.x = Src.Start.x + Dir.x * HitDist;
        HitPoint.y = Src.Start.y + Dir.y * HitDist;
    }

    return result;
}

bool CCollision::ComputeAxisProjection(
    const FVector2D& CenterLine, 
    const FVector2D& SeparationAxis,
    float AxisHalfSize, const FVector2D* DestAxis,
    const FVector2D& DestHalfSize)
{
    // 센터를 연결하는 벡터를 분리축 후보에 투영하여
    // 구간의 길이를 구한다.
    // 단, 값 비교를 위해 마이너스 값은 필요가 없으므로
    // 절대값을 이용하여 구한다.
    float CenterProjectionDist = abs(CenterLine.Dot(SeparationAxis));

    // Src와 Dest를 분리축에 투영한 구간을 구한다.
    // Src의 X축에 투영하면 Src의 투영한 구간의 길이는
    // Src의 X절반 길이와 같다.
    float DestProjectionDist =
        abs(SeparationAxis.Dot(DestAxis[EAxis::X])) * DestHalfSize.x +
        abs(SeparationAxis.Dot(DestAxis[EAxis::Y])) * DestHalfSize.y;

    if (CenterProjectionDist >
        AxisHalfSize + DestProjectionDist)
        return false;

    return true;
}

FAABB2D CCollision::CreateAABB2D(const FOBB2D& Info)
{
    FAABB2D result;

    FVector2D   Pos[4];

    Pos[0] = Info.Center - Info.Axis[0] * Info.HalfSize.x +
        Info.Axis[1] * Info.HalfSize.y;

    Pos[1] = Info.Center - Info.Axis[0] * Info.HalfSize.x -
        Info.Axis[1] * Info.HalfSize.y;

    Pos[2] = Info.Center + Info.Axis[0] * Info.HalfSize.x +
        Info.Axis[1] * Info.HalfSize.y;

    Pos[3] = Info.Center + Info.Axis[0] * Info.HalfSize.x -
        Info.Axis[1] * Info.HalfSize.y;

    result.Min.x = Pos[0].x;
    result.Min.y = Pos[0].y;

    result.Max.x = Pos[0].x;
    result.Max.y = Pos[0].y;

    for (int i = 1; i < 4; ++i)
    {
        result.Min.x = result.Min.x > Pos[i].x ? Pos[i].x : result.Min.x;
        result.Min.y = result.Min.y > Pos[i].y ? Pos[i].y : result.Min.y;

        result.Max.x = result.Max.x < Pos[i].x ? Pos[i].x : result.Max.x;
        result.Max.y = result.Max.y < Pos[i].y ? Pos[i].y : result.Max.y;
    }

    return result;
}

FAABB2D CCollision::CreateAABB2D(const FVector2D& Center, 
    float Radius)
{
    FAABB2D result;
    
    result.Min = Center - Radius;
    result.Max = Center + Radius;

    return result;
}

FOBB2D CCollision::CreateOBB2D(const FAABB2D& Info)
{
    FOBB2D  result;

    result.Center = (Info.Min + Info.Max) * 0.5f;

    result.Axis[EAxis::X].x = 1.f;
    result.Axis[EAxis::X].y = 0.f;

    result.Axis[EAxis::Y].x = 0.f;
    result.Axis[EAxis::Y].y = 1.f;

    result.HalfSize = (Info.Max - Info.Min) * 0.5f;

    return result;
}

bool CCollision::CollisionPointToAABB2D(
    const FVector2D& Point, const FAABB2D& Info)
{
    if (Point.x < Info.Min.x)
        return false;

    else if (Point.y < Info.Min.y)
        return false;

    else if (Point.x > Info.Max.x)
        return false;

    else if (Point.y > Info.Max.y)
        return false;

    return true;
}

bool CCollision::CollisionPointToOBB2D(
    const FVector2D& Point, const FOBB2D& Info)
{
    FVector2D   CenterLine = Point - Info.Center;

    float   Dist = abs(CenterLine.Dot(Info.Axis[EAxis::X]));

    if (Dist > Info.HalfSize.x)
        return false;

    Dist = abs(CenterLine.Dot(Info.Axis[EAxis::Y]));

    if (Dist > Info.HalfSize.y)
        return false;

    return true;
}

bool CCollision::CollisionPointToSphere2D(
    const FVector2D& Point, const FVector2D& Center, 
    float Radius)
{
    float   Dist = Center.Distance(Point);

    return Dist <= Radius;
}

bool CCollision::CollisionPointToLine2D(
    const FVector2D& Point, const FLine2D& Info)
{
    // 점 p(x, y)가 있을 경우 직선의 방향 (A, B)
    // Ax + By + C = 0
    
    // 방향 내적 점 을 하면 코사인세타가 나온다.
    FVector2D   LineDir = Info.End - Info.Start;
    LineDir.Normalize();

    FVector2D   LineDir1 = Point - Info.Start;
    LineDir1.Normalize();

    float   Dot = LineDir.Dot(LineDir1);

    if (Dot < 1.f)
        return false;

    float Dist = Info.Start.Distance(Info.End);
    float Dist1 = Info.Start.Distance(Point);

    return Dist >= Dist1;
}

ECCWResult::Type CCollision::CCW2D(const FVector2D& P1, 
    const FVector2D& P2, const FVector2D& P3)
{
    // CCW(Counter Clockwise) 알고리즘
    // 점 3개가 이루는 방향을 계산하는 알고리즘.
    // 점 A, B, C 가 있을 경우 세 점이 이루는 방향을 계산
    // CCW(A, B, C) = (Bx - Ax) * (Cy - Ay) - (By - Ay) * (Cx - Ax)
    // CCW(A, C, B) = (Cx - Ax) * (By - Ay) - (Cy - Ay) * (Bx - Ax)
    // A : 1, 1 B : 4, 4 C : 3, 3
    // (3 - 1) * (4 - 1) - (3 - 1) * (4 - 1)
    // 2 * 3 - 2 * 3 = 0
    // A, B, C가 차례로 들어갔을 경우
    // 결과가 음수면 A, B, C는 시계방향이다.
    // 결과가 양수면 A, B, C는 반시계방향.
    // 결과가 0이면 세 점은 일직선상에 존재한다.
    float   Cross = (P2.x - P1.x) * (P3.y - P1.y) -
        (P2.y - P1.y) * (P3.x - P1.x);

    if (Cross < 0)
        return ECCWResult::CCW;

    else if (Cross > 0)
        return ECCWResult::CW;

    return ECCWResult::Line;
}

bool CCollision::PointOnLine2D(const FVector2D& LineStart, 
    const FVector2D& LineEnd, const FVector2D& Point)
{
    float MinX, MinY, MaxX, MaxY;

    if (LineStart.x < LineEnd.x)
    {
        MinX = LineStart.x;
        MaxX = LineEnd.x;
    }

    else
    {
        MinX = LineEnd.x;
        MaxX = LineStart.x;
    }

    if (LineStart.y < LineEnd.y)
    {
        MinY = LineStart.y;
        MaxY = LineEnd.y;
    }

    else
    {
        MinY = LineEnd.y;
        MaxY = LineStart.y;
    }

    if (MinX > Point.x)
        return false;

    else if (MaxX < Point.x)
        return false;

    else if (MinY > Point.y)
        return false;

    else if (MaxY < Point.y)
        return false;

    return true;
}
