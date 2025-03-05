#include "SceneCollision.h"
#include "../Component/ColliderBase.h"
#include "CollisionQuadTree.h"
#include "Scene/SceneUIManager.h"
#include "Scene/Input.h"

CSceneCollision::CSceneCollision()
{
}

CSceneCollision::~CSceneCollision()
{
    SAFE_DELETE(mQuadTree);
}

void CSceneCollision::AddCollider(CColliderBase* Collider)
{
    if (Collider->GetColliderType() == EColliderType::Colider2D)
        mColliderList2D.emplace_back(Collider);

    else
        mColliderList3D.emplace_back(Collider);
}

bool CSceneCollision::Init()
{
    mQuadTree = new CCollisionQuadTree;

    mQuadTree->mScene = mScene;

    mQuadTree->Init();

    return true;
}

void CSceneCollision::Update(float DeltaTime, CSceneUIManager* UIManager,
    CInput* Input)
{
    if (mInterval > 0.f)
    {
        mIntervalTime += DeltaTime;

        if (mIntervalTime < mInterval)
            return;

        mIntervalTime -= mInterval;
    }

    mQuadTree->Update(DeltaTime);

    size_t Size = mColliderList2D.size();

    for (size_t i = 0; i < Size;)
    {
        if (!mColliderList2D[i]->IsActive())
        {
            if (i < Size - 1)
            {
                /*CSharedPtr<CColliderBase> Temp = mColliderList2D[i];
                mColliderList2D[i] = mColliderList2D[Size - 1];
                mColliderList2D[Size - 1] = Temp;*/
                mColliderList2D[i] = mColliderList2D[Size - 1];
            }
            mColliderList2D.pop_back();
            --Size;
            continue;
        }

        else if (!mColliderList2D[i]->IsEnable())
        {
            ++i;
            continue;
        }

        // 쿼드트리에 추가한다.
        mQuadTree->AddCollider(mColliderList2D[i]);

        ++i;
    }

    // 마우스와 UI의 충돌을 진행한다.
    FVector2D   MousePos = Input->GetMousePos();

    bool Result = UIManager->CollisionMouse(DeltaTime, MousePos);




    FVector2D   MouseWorldPos = Input->GetMouseWorldPos2D();

    // 모든 충돌체가 쿼드트리의 노드에 추가되었기 때문에
    // 충돌처리를 진행한다.
    mQuadTree->Collision(DeltaTime);
}

void CSceneCollision::Render()
{
    mQuadTree->Render();
}

void CSceneCollision::ReturnNodePool()
{
    mQuadTree->ReturnNodePool();
}
