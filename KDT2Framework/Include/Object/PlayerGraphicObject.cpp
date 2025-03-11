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
	
	mRootInitPos = FVector3D(370.f, 242.f, 0.f);
	mMapDifficultySinAngle = 0.0f;

	mRoot = CreateComponent<CSpriteComponent>("Player");
	mRoot->SetPivot(0.5f, 0.5f);
	mRoot->SetWorldPos(mRootInitPos);
	FVector3D tempRootSize = FVector3D(200.0f, 135.0f, 1.f);
	mRoot->SetWorldScale(tempRootSize * 1.0f);
	mRoot->SetColor(mPlayerColors[0]);
	SetRootComponent(mRoot);


	mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();
	mAnimation->AddSequence("PlayerIdle", 0.3f, 1.f, true, false);
	mAnimation->ChangeAnimation("PlayerIdle");

	return true;
}

void CPlayerGraphicObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	// �� ���� �̹��� ���̺�.
	{
		mMapDifficultySinAngle += DeltaTime * 180.0f;

		if (mMapDifficultySinAngle >= 360.0f)
		{
			mMapDifficultySinAngle = 0.0f;
		}

		float tempVal = cos(DirectX::XMConvertToRadians(mMapDifficultySinAngle));
		mRoot->SetWorldPos(mRootInitPos + FVector3D::Axis[EAxis::Y] * tempVal * 10);
	}
}

int CPlayerGraphicObject::GetGraphicCount()
{
	return mPlayerColors.size();
}

bool CPlayerGraphicObject::SetChangeGraphic(int index)
{
	// �ؽ�ó�� �ƿ� �ٲ۴ٸ� mAnimation ���� ó�� �ؾ� ��.

	// ����� �÷��θ�.
	mRoot->SetColor(mPlayerColors[index]);

	return false;
}
