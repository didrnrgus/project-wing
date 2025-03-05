#include "ColliderBase.h"
#include "Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "ProfileManager.h"
#include "Shader/ColliderCBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/Shader.h"
#include "Shader/ShaderManager.h"
#include "Scene/CameraManager.h"
#include "Asset/Mesh/Mesh.h"

CColliderBase::CColliderBase()
{
#ifdef _DEBUG

    mRenderType = EComponentRender::Render;
    mRenderLayerName = "Object";

#endif // _DEBUG
    // 리해싱이 자주 일어나지 않도록 미리 공간을 확보한다.
    mCollisionObjects.reserve(100);
    mCollisionObjects.max_load_factor(2.f);
    //mCollisionObjects.rehash(200);
}

CColliderBase::CColliderBase(const CColliderBase& Com)  :
    CSceneComponent(Com)
{
}

CColliderBase::CColliderBase(CColliderBase&& Com) :
    CSceneComponent(Com)
{
}

CColliderBase::~CColliderBase()
{
    auto    iter = mCollisionObjects.begin();
    auto    iterEnd = mCollisionObjects.end();

    for (; iter != iterEnd; ++iter)
    {
        iter->first->CallCollisionEnd(this);
    }

#ifdef _DEBUG

    SAFE_DELETE(mCBuffer);

#endif // _DEBUG
}

void CColliderBase::SetCollisionProfile(const std::string& Name)
{
    mProfile = CProfileManager::GetInst()->FindProfile(Name);
}

void CColliderBase::CallCollisionBegin(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    // 인자로 들어온 물체는 이제 막 충돌되기 시작한 물체
    // 이므로 충돌목록에 추가한다.
    AddCollisionObject(Dest);

    // 충돌상태 ON
    mCollision = true;

    // 충돌 시작 시 호출해야 할 함수가 있을 경우 호출해준다.
    if (mCollisionBeginFunc)
        mCollisionBeginFunc(HitPoint, Dest);
}

void CColliderBase::CallCollisionEnd(CColliderBase* Dest)
{
    // 인자로 들어온 물체는 충돌되다가 떨어지는 물체이므로
    // 제거한다.
    EraseCollisionObject(Dest);

    // 충돌이 떨어지면 이 함수가 무조건 호출되기 때문에
    // 더이상 충돌되는 물체가 없다면 충돌상태를 false로
    // 만들어준다.
    if (mCollisionObjects.empty())
        mCollision = false;

    if (mCollisionEndFunc)
        mCollisionEndFunc(Dest);
}

bool CColliderBase::CheckCollisionObject(
    CColliderBase* Collider)
{
    auto    iter = mCollisionObjects.find(Collider);

    if (iter == mCollisionObjects.end())
        return false;

    return true;
}

void CColliderBase::AddCollisionObject(
    CColliderBase* Collider)
{
    // unordered_map은 랜덤액세스를 지원한다.
    // 해당 키값이 없을 경우 노드를 하나 새로 만들어주고
    // 있을 경우 해당 노드를 가져온다.
    mCollisionObjects[Collider] = true;
}

void CColliderBase::EraseCollisionObject(
    CColliderBase* Collider)
{
    mCollisionObjects.erase(Collider);
}

bool CColliderBase::Init()
{
    if (!CSceneComponent::Init())
        return false;

    SetCollisionProfile("Default");

#ifdef _DEBUG

    mShader = CShaderManager::GetInst()->FindShader("FrameMeshShader");

    mCBuffer = new CColliderCBuffer;

    mCBuffer->Init();

#endif // _DEBUG


    mScene->GetCollision()->AddCollider(this);

    return true;
}

bool CColliderBase::Init(const char* FileName)
{
    if (!CSceneComponent::Init(FileName))
        return false;

    SetCollisionProfile("Default");

#ifdef _DEBUG

    mShader = CShaderManager::GetInst()->FindShader("FrameMeshShader");

    mCBuffer = new CColliderCBuffer;

    mCBuffer->Init();

    mTransformCBuffer = new CTransformCBuffer;

    mTransformCBuffer->Init();

#endif // _DEBUG

    mScene->GetCollision()->AddCollider(this);

    return true;
}

void CColliderBase::PreUpdate(float DeltaTime)
{
    CSceneComponent::PreUpdate(DeltaTime);
}

void CColliderBase::Update(float DeltaTime)
{
    CSceneComponent::Update(DeltaTime);
}

void CColliderBase::PostUpdate(float DeltaTime)
{
    CSceneComponent::PostUpdate(DeltaTime);
}

void CColliderBase::Collision(float DeltaTime)
{
    CSceneComponent::Collision(DeltaTime);
}

void CColliderBase::PreRender()
{
    CSceneComponent::PreRender();
}

void CColliderBase::Render()
{
    CSceneComponent::Render();

#ifdef _DEBUG

    FMatrix  matScale, matRot, matTranslate, matWorld;
        
    matScale.Scaling(mWorldScale);

    if (mEnableRotation)
        matRot.Rotation(mWorldRot);

    matTranslate.Translation(mWorldPos);

    matWorld = matScale * matRot * matTranslate;

    mTransformCBuffer->SetWorldMatrix(matWorld);
    mTransformCBuffer->SetViewMatrix(mScene->GetCameraManager()->GetViewMatrix());
    mTransformCBuffer->SetProjMatrix(mScene->GetCameraManager()->GetProjMatrix());

    mTransformCBuffer->UpdateBuffer();

    if (mCollision)
        mCBuffer->SetColor(1.f, 0.f, 0.f, 1.f);

    else
        mCBuffer->SetColor(0.f, 1.f, 0.f, 1.f);

    mCBuffer->UpdateBuffer();

    mShader->SetShader();

    mMesh->Render();

#endif // _DEBUG
}

void CColliderBase::PostRender()
{
    CSceneComponent::PostRender();
}

void CColliderBase::EraseOwner()
{
    CSceneComponent::EraseOwner();

    // 이 함수는 오너 오브젝트가 지워질때 호출되는 함수인데
    // End함수가 오너 오브젝트의 함수로 지정되어 있을 경우
    // End함수를 제거한다.
    if (mCollisionEndFunc && mEndObj == GetOwner())
    {
        mCollisionEndFunc = nullptr;
    }
}

CColliderBase* CColliderBase::Clone()
{
    return nullptr;
}
