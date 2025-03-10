#include "PlayerGraphicObject.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"

CPlayerGraphicObject::CPlayerGraphicObject()
{
}

CPlayerGraphicObject::CPlayerGraphicObject(const CPlayerGraphicObject& Obj)
{
}

CPlayerGraphicObject::CPlayerGraphicObject(CPlayerGraphicObject&& Obj)
{
}

CPlayerGraphicObject::~CPlayerGraphicObject()
{
}

bool CPlayerGraphicObject::Init()
{
	CSceneObject::Init();

	mPlayerColors.push_back(FVector4D::White);
	mPlayerColors.push_back(FVector4D::Red);
	mPlayerColors.push_back(FVector4D::Green);
	mPlayerColors.push_back(FVector4D::Cyan);
	mPlayerColors.push_back(FVector4D::Yellow);

	mRoot = CreateComponent<CSpriteComponent>("Player");
	mCamera = CreateComponent<CCameraComponent>();
	mCamera->SetProjectionType(ECameraProjectionType::Ortho);
	mCamera->SetWorldPos(FVector3D::Zero);

	mRoot->SetPivot(0.5f, 0.5f);
	mRoot->SetWorldPos(370.f, 242.f, 0.f);
	FVector3D tempRootSize = FVector3D(128.f, 128.f, 1.f);
	mRoot->SetWorldScale(tempRootSize * 1.2f);
	mRoot->SetColor(mPlayerColors[0]);
	SetRootComponent(mRoot);


	mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();
	mAnimation->AddSequence("PlayerIdle", 1.0f, 1.f, true, false);
	mAnimation->ChangeAnimation("PlayerIdle");

	return true;
}

void CPlayerGraphicObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
}

int CPlayerGraphicObject::GetGraphicCount()
{
	return mPlayerColors.size();
}

bool CPlayerGraphicObject::SetChangeGraphic(int index)
{
	// 텍스처를 아예 바꾼다면 mAnimation 에서 처리 해야 함.

	// 현재는 컬러로만.
	mRoot->SetColor(mPlayerColors[index]);

	return false;
}
