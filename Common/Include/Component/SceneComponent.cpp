#include "SceneComponent.h"
#include "Shader/TransformCBuffer.h"
#include "../Render/RenderManager.h"

CSceneComponent::CSceneComponent()
{
    mTransformCBuffer = new CTransformCBuffer;

    mTransformCBuffer->Init();
}

CSceneComponent::CSceneComponent(const CSceneComponent& Com) :
    CComponent(Com)
{
    mTransformCBuffer = Com.mTransformCBuffer->Clone();
}

CSceneComponent::CSceneComponent(CSceneComponent&& Com) :
    CComponent(Com)
{
    mTransformCBuffer = Com.mTransformCBuffer;
    Com.mTransformCBuffer = nullptr;
}

CSceneComponent::~CSceneComponent()
{
    SAFE_DELETE(mTransformCBuffer);

    size_t  Size = mChildList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        mChildList[i]->Destroy();
    }
}

void CSceneComponent::AddChild(CSceneComponent* Child)
{
    Child->mParent = this;

    mChildList.emplace_back(Child);

    Child->ComputeTransform();
}

bool CSceneComponent::Init()
{
    if (mRenderType == EComponentRender::Render)
        CRenderManager::GetInst()->AddRenderList(this);

    return true;
}

bool CSceneComponent::Init(const char* FileName)
{
    if (mRenderType == EComponentRender::Render)
        CRenderManager::GetInst()->AddRenderList(this);

    return true;
}

void CSceneComponent::PreUpdate(float DeltaTime)
{
    CComponent::PreUpdate(DeltaTime);

    std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    for (iter = mChildList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            // 지워야 할 값과 마지막 값을 바꾼다.
            std::swap(*iter, mChildList.back());

            mChildList.pop_back();
            iterEnd = mChildList.end();
            continue;
        }

        else if (!(*iter)->IsEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->PreUpdate(DeltaTime);
        ++iter;
    }
}

void CSceneComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);

    std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    for (iter = mChildList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            // 지워야 할 값과 마지막 값을 바꾼다.
            std::swap(*iter, mChildList.back());

            mChildList.pop_back();
            iterEnd = mChildList.end();
            continue;
        }

        else if (!(*iter)->IsEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->Update(DeltaTime);
        ++iter;
    }
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);

    std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    for (iter = mChildList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            // 지워야 할 값과 마지막 값을 바꾼다.
            std::swap(*iter, mChildList.back());

            mChildList.pop_back();
            iterEnd = mChildList.end();
            continue;
        }

        else if (!(*iter)->IsEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->PostUpdate(DeltaTime);
        ++iter;
    }
}

void CSceneComponent::Collision(float DeltaTime)
{
    CComponent::Collision(DeltaTime);

    std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    for (iter = mChildList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            // 지워야 할 값과 마지막 값을 바꾼다.
            std::swap(*iter, mChildList.back());

            mChildList.pop_back();
            iterEnd = mChildList.end();
            continue;
        }

        else if (!(*iter)->IsEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->Collision(DeltaTime);
        ++iter;
    }
}

void CSceneComponent::PreRender()
{
    CComponent::PreRender();

    mmatScale.Scaling(mWorldScale);
    mmatRot.Rotation(mWorldRot);
    mmatTranslate.Translation(mWorldPos);

    mmatWorld = mmatScale * mmatRot * mmatTranslate;

    //std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    //std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    //for (iter = mChildList.begin(); iter != iterEnd;)
    //{
    //    if (!(*iter)->IsActive())
    //    {
    //        // 지워야 할 값과 마지막 값을 바꾼다.
    //        std::swap(*iter, mChildList.back());

    //        mChildList.pop_back();
    //        iterEnd = mChildList.end();
    //        continue;
    //    }

    //    else if (!(*iter)->IsEnable())
    //    {
    //        ++iter;
    //        continue;
    //    }

    //    (*iter)->PreRender();
    //    ++iter;
    //}
}

void CSceneComponent::Render()
{
    CComponent::Render();

    //std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    //std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    //for (iter = mChildList.begin(); iter != iterEnd;)
    //{
    //    if (!(*iter)->IsActive())
    //    {
    //        // 지워야 할 값과 마지막 값을 바꾼다.
    //        std::swap(*iter, mChildList.back());

    //        mChildList.pop_back();
    //        iterEnd = mChildList.end();
    //        continue;
    //    }

    //    else if (!(*iter)->IsEnable())
    //    {
    //        ++iter;
    //        continue;
    //    }

    //    (*iter)->Render();
    //    ++iter;
    //}
}

void CSceneComponent::PostRender()
{
    CComponent::PostRender();

    //std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    //std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    //for (iter = mChildList.begin(); iter != iterEnd;)
    //{
    //    if (!(*iter)->IsActive())
    //    {
    //        // 지워야 할 값과 마지막 값을 바꾼다.
    //        std::swap(*iter, mChildList.back());

    //        mChildList.pop_back();
    //        iterEnd = mChildList.end();
    //        continue;
    //    }

    //    else if (!(*iter)->IsEnable())
    //    {
    //        ++iter;
    //        continue;
    //    }

    //    (*iter)->PostRender();
    //    ++iter;
    //}
}

CSceneComponent* CSceneComponent::Clone()
{
    return new CSceneComponent(*this);
}

void CSceneComponent::EraseOwner()
{
    std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    for (iter = mChildList.begin(); iter != iterEnd; ++iter)
    {
        (*iter)->EraseOwner();
    }
}

void CSceneComponent::EndFrame()
{
    CComponent::EndFrame();

    std::vector<CSharedPtr<CSceneComponent>>::iterator  iter;
    std::vector<CSharedPtr<CSceneComponent>>::iterator  iterEnd = mChildList.end();

    for (iter = mChildList.begin(); iter != iterEnd; ++iter)
    {
        (*iter)->EndFrame();
    }
}

void CSceneComponent::SetRelativeScale(const FVector3D& Scale)
{
    mRelativeScale = Scale;

    if (mParent)
    {
        mWorldScale = mRelativeScale * mParent->mWorldScale;
    }

    else
    {
        mWorldScale = mRelativeScale;
    }

    size_t  Size = mChildList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        mChildList[i]->mWorldPos = mChildList[i]->mRelativePos * mWorldScale +
            mWorldPos;

        mChildList[i]->SetWorldScale(mChildList[i]->mRelativeScale * mWorldScale);
    }
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
    SetRelativeScale(FVector3D(x, y, z));
}

void CSceneComponent::SetRelativeScale(const FVector2D& Scale)
{
    SetRelativeScale(FVector3D(Scale.x, Scale.y, mRelativeScale.z));
}

void CSceneComponent::SetRelativeScale(float x, float y)
{
    SetRelativeScale(FVector3D(x, y, mRelativeScale.z));
}

void CSceneComponent::SetRelativeRotation(const FVector3D& Rot)
{
    mRelativeRot = Rot;

    if (mParent)
    {
        mWorldRot = mRelativeRot + mParent->mWorldRot;
    }

    else
    {
        mWorldRot = mRelativeRot;
    }



    FVector3D   Axis[EAxis::End] =
    {
        FVector3D(1.f, 0.f, 0.f),
        FVector3D(0.f, 1.f, 0.f),
        FVector3D(0.f, 0.f, 1.f)
    };

    FMatrix matRot;
    matRot.Rotation(mWorldRot);

    // 회전된 축을 구한다.
    mAxis[EAxis::X] = Axis[EAxis::X].TransformNormal(matRot);
    mAxis[EAxis::Y] = Axis[EAxis::Y].TransformNormal(matRot);
    mAxis[EAxis::Z] = Axis[EAxis::Z].TransformNormal(matRot);

    mAxis[EAxis::X].Normalize();
    mAxis[EAxis::Y].Normalize();
    mAxis[EAxis::Z].Normalize();

    size_t  Size = mChildList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        /*mChildList[i]->mWorldPos =
            mChildList[i]->mRelativePos.GetRotation(mWorldRot) + mWorldPos;*/
        FVector3D   ParentRot = GetWorldRotation();

        FMatrix matRot;
        matRot.Rotation(ParentRot);

        // 행렬의 41, 42, 43 에 부모의 위치를 넣어 부모의 위치를 중심으로
        // 회전하는 행렬을 만들어준다.
        memcpy(&matRot._41, &mWorldPos, sizeof(FVector3D));

        mChildList[i]->mWorldPos = mChildList[i]->mRelativePos.TransformCoord(matRot);

        mChildList[i]->SetWorldRotation(mChildList[i]->mRelativeRot + mWorldRot);
    }
}

void CSceneComponent::SetRelativeRotation(float x, float y, float z)
{
    SetRelativeRotation(FVector3D(x, y, z));
}

void CSceneComponent::SetRelativeRotation(const FVector2D& Rot)
{
    SetRelativeRotation(FVector3D(Rot.x, Rot.y, mRelativeRot.z));
}

void CSceneComponent::SetRelativeRotation(float x, float y)
{
    SetRelativeRotation(FVector3D(x, y, mRelativeRot.z));
}

void CSceneComponent::SetRelativeRotationX(float x)
{
    SetRelativeRotation(FVector3D(x, mRelativeRot.y, mRelativeRot.z));
}

void CSceneComponent::SetRelativeRotationY(float y)
{
    SetRelativeRotation(FVector3D(mRelativeRot.x, y, mRelativeRot.z));
}

void CSceneComponent::SetRelativeRotationZ(float z)
{
    SetRelativeRotation(FVector3D(mRelativeRot.x, mRelativeRot.y, z));
}

void CSceneComponent::SetRelativeRotationAxis(float Angle, const FVector3D& Axis)
{
}

void CSceneComponent::SetRelativePos(const FVector3D& Pos)
{
    mRelativePos = Pos;

    if (mParent)
    {
        FVector3D   ParentRot = mParent->GetWorldRotation();

        FMatrix matRot;
        matRot.Rotation(ParentRot);

        // 행렬의 41, 42, 43 에 부모의 위치를 넣어 부모의 위치를 중심으로
        // 회전하는 행렬을 만들어준다.
        memcpy(&matRot._41, &mParent->mWorldPos, sizeof(FVector3D));

        mWorldPos = mRelativePos.TransformCoord(matRot);
    }

    else
    {
        mWorldPos = mRelativePos;
    }

    size_t  Size = mChildList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        mChildList[i]->SetWorldPos(mChildList[i]->mRelativePos + mWorldPos);
    }
}

void CSceneComponent::SetRelativePos(float x, float y, float z)
{
    SetRelativePos(FVector3D(x, y, z));
}

void CSceneComponent::SetRelativePos(const FVector2D& Pos)
{
    SetRelativePos(FVector3D(Pos.x, Pos.y, mRelativePos.z));
}

void CSceneComponent::SetRelativePos(float x, float y)
{
    SetRelativePos(FVector3D(x, y, mRelativePos.z));
}

void CSceneComponent::SetWorldScale(const FVector3D& Scale)
{
    mWorldScale = Scale;

    if (mParent)
    {
        mRelativeScale = mWorldScale / mParent->mWorldScale;
    }

    else
    {
        mRelativeScale = mWorldScale;
    }

    size_t  Size = mChildList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        mChildList[i]->SetWorldScale(mChildList[i]->mRelativeScale * mWorldScale);

        mChildList[i]->SetWorldPos(mChildList[i]->mRelativePos * mWorldScale + mWorldPos);
    }
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
    SetWorldScale(FVector3D(x, y, z));
}

void CSceneComponent::SetWorldScale(const FVector2D& Scale)
{
    SetWorldScale(FVector3D(Scale.x, Scale.y, mWorldScale.z));
}

void CSceneComponent::SetWorldScale(float x, float y)
{
    SetWorldScale(FVector3D(x, y, mWorldScale.z));
}

void CSceneComponent::SetWorldRotation(const FVector3D& Rot)
{
    mWorldRot = Rot;

    if (mParent)
    {
        mRelativeRot = mWorldRot - mParent->mWorldRot;
    }

    else
    {
        mRelativeRot = mWorldRot;
    }

    FVector3D   Axis[EAxis::End] =
    {
        FVector3D(1.f, 0.f, 0.f),
        FVector3D(0.f, 1.f, 0.f),
        FVector3D(0.f, 0.f, 1.f)
    };

    FMatrix matRot;
    matRot.Rotation(mWorldRot);

    // 회전된 축을 구한다.
    mAxis[EAxis::X] = Axis[EAxis::X].TransformNormal(matRot);
    mAxis[EAxis::Y] = Axis[EAxis::Y].TransformNormal(matRot);
    mAxis[EAxis::Z] = Axis[EAxis::Z].TransformNormal(matRot);

    mAxis[EAxis::X].Normalize();
    mAxis[EAxis::Y].Normalize();
    mAxis[EAxis::Z].Normalize();

    size_t  Size = mChildList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        mChildList[i]->SetWorldRotation(mChildList[i]->mRelativeRot + mWorldRot);

        FVector3D   ParentRot = GetWorldRotation();

        FMatrix matRot;
        matRot.Rotation(ParentRot);

        // 행렬의 41, 42, 43 에 부모의 위치를 넣어 부모의 위치를 중심으로
        // 회전하는 행렬을 만들어준다.
        memcpy(&matRot._41, &mWorldPos, sizeof(FVector3D));

        mChildList[i]->mWorldPos = mChildList[i]->mRelativePos.TransformCoord(matRot);
        /*mChildList[i]->SetWorldPos(
            mChildList[i]->mRelativePos.GetRotation(mWorldRot) + mWorldPos);*/
    }
}

void CSceneComponent::SetWorldRotation(float x, float y, float z)
{
    SetWorldRotation(FVector3D(x, y, z));
}

void CSceneComponent::SetWorldRotation(const FVector2D& Rot)
{
    SetWorldRotation(FVector3D(Rot.x, Rot.y, mWorldRot.z));
}

void CSceneComponent::SetWorldRotation(float x, float y)
{
    SetWorldRotation(FVector3D(x, y, mWorldRot.z));
}

void CSceneComponent::SetWorldRotationX(float x)
{
    SetWorldRotation(FVector3D(x, mWorldRot.y, mWorldRot.z));
}

void CSceneComponent::SetWorldRotationY(float y)
{
    SetWorldRotation(FVector3D(mWorldRot.x, y, mWorldRot.z));
}

void CSceneComponent::SetWorldRotationZ(float z)
{
    SetWorldRotation(FVector3D(mWorldRot.x, mWorldRot.y, z));
}

void CSceneComponent::SetWorldRotationAxis(float Angle, const FVector3D& Axis)
{
}

void CSceneComponent::SetWorldPos(const FVector3D& Pos)
{
    mWorldPos = Pos;

    if (mParent)
    {
        FVector3D   ParentRot = mParent->GetWorldRotation();

        FMatrix matRot;
        matRot.Rotation(ParentRot);

        // 행렬의 41, 42, 43 에 부모의 위치를 넣어 부모의 위치를 중심으로
        // 회전하는 행렬을 만들어준다.
        memcpy(&matRot._41, &mParent->mWorldPos, sizeof(FVector3D));

        mWorldPos = mRelativePos.TransformCoord(matRot);

        /*FVector3D RelativePos = mWorldPos - mParent->mWorldPos;
        mRelativePos = RelativePos.GetRotation(mParent->mWorldRot * -1.f);*/
    }

    else
    {
        mRelativePos = mWorldPos;
    }

    size_t  Size = mChildList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        mChildList[i]->SetWorldPos(mChildList[i]->mRelativePos + mWorldPos);
    }
}

void CSceneComponent::SetWorldPos(float x, float y, float z)
{
    SetWorldPos(FVector3D(x, y, z));
}

void CSceneComponent::SetWorldPos(const FVector2D& Pos)
{
    SetWorldPos(FVector3D(Pos.x, Pos.y, mWorldPos.z));
}

void CSceneComponent::SetWorldPos(float x, float y)
{
    SetWorldPos(FVector3D(x, y, mWorldPos.z));
}

void CSceneComponent::AddRelativeScale(const FVector3D& Scale)
{
    mRelativeScale += Scale;

    SetRelativeScale(mRelativeScale);
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
    AddRelativeScale(FVector3D(x, y, z));
}

void CSceneComponent::AddRelativeScale(const FVector2D& Scale)
{
    AddRelativeScale(FVector3D(Scale.x, Scale.y, 0.f));
}

void CSceneComponent::AddRelativeScale(float x, float y)
{
    AddRelativeScale(FVector3D(x, y, 0.f));
}

void CSceneComponent::AddRelativeRotation(const FVector3D& Rot)
{
    mRelativeRot += Rot;

    SetRelativeRotation(mRelativeRot);
}

void CSceneComponent::AddRelativeRotation(float x, float y, float z)
{
    AddRelativeRotation(FVector3D(x, y, z));
}

void CSceneComponent::AddRelativeRotation(const FVector2D& Rot)
{
    AddRelativeRotation(FVector3D(Rot.x, Rot.y, 0.f));
}

void CSceneComponent::AddRelativeRotation(float x, float y)
{
    AddRelativeRotation(FVector3D(x, y, 0.f));
}

void CSceneComponent::AddRelativeRotationX(float x)
{
    AddRelativeRotation(FVector3D(x, 0.f, 0.f));
}

void CSceneComponent::AddRelativeRotationY(float y)
{
    AddRelativeRotation(FVector3D(0.f, y, 0.f));
}

void CSceneComponent::AddRelativeRotationZ(float z)
{
    AddRelativeRotation(FVector3D(0.f, 0.f, z));
}

void CSceneComponent::AddRelativePos(const FVector3D& Pos)
{
    mRelativePos += Pos;

    SetRelativePos(mRelativePos);
}

void CSceneComponent::AddRelativePos(float x, float y, float z)
{
    AddRelativePos(FVector3D(x, y, z));
}

void CSceneComponent::AddRelativePos(const FVector2D& Pos)
{
    AddRelativePos(FVector3D(Pos.x, Pos.y, 0.f));
}

void CSceneComponent::AddRelativePos(float x, float y)
{
    AddRelativePos(FVector3D(x, y, 0.f));
}

void CSceneComponent::AddWorldScale(const FVector3D& Scale)
{
    mWorldScale += Scale;

    SetWorldScale(mWorldScale);
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
    AddWorldScale(FVector3D(x, y, z));
}

void CSceneComponent::AddWorldScale(const FVector2D& Scale)
{
    AddWorldScale(FVector3D(Scale.x, Scale.y, 0.f));
}

void CSceneComponent::AddWorldScale(float x, float y)
{
    AddWorldScale(FVector3D(x, y, 0.f));
}

void CSceneComponent::AddWorldRotation(const FVector3D& Rot)
{
    mWorldRot += Rot;

    SetWorldRotation(mWorldRot);
}

void CSceneComponent::AddWorldRotation(float x, float y, float z)
{
    AddWorldRotation(FVector3D(x, y, z));
}

void CSceneComponent::AddWorldRotation(const FVector2D& Rot)
{
    AddWorldRotation(FVector3D(Rot.x, Rot.y, 0.f));
}

void CSceneComponent::AddWorldRotation(float x, float y)
{
    AddWorldRotation(FVector3D(x, y, 0.f));
}

void CSceneComponent::AddWorldRotationX(float x)
{
    AddWorldRotation(FVector3D(x, 0.f, 0.f));
}

void CSceneComponent::AddWorldRotationY(float y)
{
    AddWorldRotation(FVector3D(0.f, y, 0.f));
}

void CSceneComponent::AddWorldRotationZ(float z)
{
    AddWorldRotation(FVector3D(0.f, 0.f, z));
}

void CSceneComponent::AddWorldPos(const FVector3D& Pos)
{
    mWorldPos += Pos;

    SetWorldPos(mWorldPos);
}

void CSceneComponent::AddWorldPos(float x, float y, float z)
{
    AddWorldPos(FVector3D(x, y, z));
}

void CSceneComponent::AddWorldPos(const FVector2D& Pos)
{
    AddWorldPos(FVector3D(Pos.x, Pos.y, 0.f));
}

void CSceneComponent::AddWorldPos(float x, float y)
{
    AddWorldPos(FVector3D(x, y, 0.f));
}

void CSceneComponent::ComputeTransform()
{
    SetWorldScale(mRelativeScale * mParent->mWorldScale);
    SetWorldRotation(mRelativeRot + mParent->mWorldRot);
    SetWorldPos(mRelativePos + mParent->mWorldPos);
}
