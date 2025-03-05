#include "ColliderOBB2D.h"
#include "../Collision.h"
#include "Asset/Mesh/Mesh.h"
#include "Asset/AssetManager.h"
#include "Asset/Mesh/MeshManager.h"
#include "ColliderSphere2D.h"
#include "ColliderAABB2D.h"
#include "ColliderLine2D.h"

CColliderOBB2D::CColliderOBB2D()
{
    mColliderType = EColliderType::Colider2D;
    mColliderShape = EColliderShape::OBB2D;

    mBoxSize.x = 50.f;
    mBoxSize.y = 50.f;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D& Com) :
    CColliderBase(Com)
{
    mColliderType = EColliderType::Colider2D;
    mColliderShape = EColliderShape::OBB2D;

    mBoxSize = Com.mBoxSize;
}

CColliderOBB2D::CColliderOBB2D(CColliderOBB2D&& Com) :
    CColliderBase(Com)
{
    mColliderType = EColliderType::Colider2D;
    mColliderShape = EColliderShape::OBB2D;

    mBoxSize = Com.mBoxSize;
}

CColliderOBB2D::~CColliderOBB2D()
{
}

bool CColliderOBB2D::Init()
{
    if (!CColliderBase::Init())
        return false;

#ifdef _DEBUG

    mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("FrameCenterRect");
    mEnableRotation = true;

#endif // _DEBUG


    return true;
}

bool CColliderOBB2D::Init(const char* FileName)
{
    if (!CColliderBase::Init(FileName))
        return false;

#ifdef _DEBUG

    mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("FrameCenterRect");
    mEnableRotation = true;

#endif // _DEBUG

    return true;
}

void CColliderOBB2D::PreUpdate(float DeltaTime)
{
    CColliderBase::PreUpdate(DeltaTime);
}

void CColliderOBB2D::Update(float DeltaTime)
{
    CColliderBase::Update(DeltaTime);

    mBoxInfo.Center.x = mWorldPos.x;
    mBoxInfo.Center.y = mWorldPos.y;

    mBoxInfo.Axis[0].x = mAxis[EAxis::X].x;
    mBoxInfo.Axis[0].y = mAxis[EAxis::X].y;

    mBoxInfo.Axis[1].x = mAxis[EAxis::Y].x;
    mBoxInfo.Axis[1].y = mAxis[EAxis::Y].y;

    mBoxInfo.HalfSize = mBoxSize * 0.5f;

    // 사각혀을 구성하는 4개의 꼭지점을 구한다.
    FVector2D   Pos[4];

    Pos[0] = mBoxInfo.Center - mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x +
        mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;

    Pos[1] = mBoxInfo.Center - mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x -
        mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;

    Pos[2] = mBoxInfo.Center + mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x +
        mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;

    Pos[3] = mBoxInfo.Center + mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x -
        mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;

    // 4개의 꼭지점에서 x, y 값을 가장 작은값과 큰값을
    // 얻어와 Min, Max 정보를 구성한다.

    mMin.x = Pos[0].x;
    mMin.y = Pos[0].y;

    mMax.x = Pos[0].x;
    mMax.y = Pos[0].y;

    for (int i = 1; i < 4; ++i)
    {
        mMin.x = mMin.x > Pos[i].x ? Pos[i].x : mMin.x;
        mMin.y = mMin.y > Pos[i].y ? Pos[i].y : mMin.y;

        mMax.x = mMax.x < Pos[i].x ? Pos[i].x : mMax.x;
        mMax.y = mMax.y < Pos[i].y ? Pos[i].y : mMax.y;
    }

    SetWorldScale(mBoxSize);
}

void CColliderOBB2D::PostUpdate(float DeltaTime)
{
    CColliderBase::PostUpdate(DeltaTime);
}

void CColliderOBB2D::Collision(float DeltaTime)
{
    CColliderBase::Collision(DeltaTime);
}

void CColliderOBB2D::PreRender()
{
    CColliderBase::PreRender();
}

void CColliderOBB2D::Render()
{
    CColliderBase::Render();
}

void CColliderOBB2D::PostRender()
{
    CColliderBase::PostRender();
}

CColliderOBB2D* CColliderOBB2D::Clone()
{
    return new CColliderOBB2D(*this);
}

bool CColliderOBB2D::Collision(FVector3D& HitPoint,
    CColliderBase* Dest)
{
    if (Dest->GetColliderType() == EColliderType::Colider3D)
        return false;

    switch (Dest->GetColliderShape())
    {
    case EColliderShape::AABB2D:
        return CCollision::CollisionAABB2DToOBB2D(HitPoint,
            (CColliderAABB2D*)Dest, this);
    case EColliderShape::Sphere2D:
        return CCollision::CollisionSphere2DToOBB2D(
            HitPoint, (CColliderSphere2D*)Dest, this);
    case EColliderShape::OBB2D:
        return CCollision::CollisionOBB2DToOBB2D(HitPoint,
            this, (CColliderOBB2D*)Dest);
    case EColliderShape::Line2D:
        return CCollision::CollisionLine2DToOBB2D(HitPoint,
            (CColliderLine2D*)Dest, this);
    }

    return false;
}


