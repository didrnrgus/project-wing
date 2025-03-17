#include "PlayerGraphicObject.h"
#include "Device.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Etc/DataStorageManager.h"
#include "Scene/SceneManager.h"

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
	FResolution RS = CDevice::GetInst()->GetResolution();

	if (CSceneManager::GetInst()->GetCurrentSceneType() == EGameScene::LobbySingle
		|| CSceneManager::GetInst()->GetCurrentSceneType() == EGameScene::LobbyMulti)
	{
		mRootInitPos = FVector3D(370.f, 242.f, 0.f);
	}
	else
	{
		mRootInitPos = FVector3D(RS.Width * 0.25f * -1.0f, 0.0f, 0.f);
	}
	mMapDifficultySinAngle = 0.0f;

	mRoot = CreateComponent<CSpriteComponent>("PlayerRoot");
	mRoot->SetPivot(0.5f, 0.5f);
	mRoot->SetWorldPos(mRootInitPos);
	auto defaultPlayerStat = CDataStorageManager::GetInst()->GetCharacterState(0);
	FVector3D tempRootSize = FVector3D(defaultPlayerStat.SizeX, defaultPlayerStat.SizeY, 1.f);
	mRoot->SetWorldScale(tempRootSize);
	mRoot->SetColor(FVector4D::GetColorFromString(defaultPlayerStat.ColorName));
	SetRootComponent(mRoot);

	mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();
	for (int i = 0; i < CDataStorageManager::GetInst()->GetCharacterCount(); i++)
	{
		mAnimation->AddSequence(
			CDataStorageManager::GetInst()->GetCharacterState(i).ImageSequenceName
			, 0.3f, 1.f, true, false);
	}

	mAnimation->ChangeAnimation(CDataStorageManager::GetInst()->GetCharacterState(0).ImageSequenceName);

	return true;
}

void CPlayerGraphicObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	// 맵 선택 이미지 웨이브.
	if (CSceneManager::GetInst()->GetCurrentSceneType() == EGameScene::LobbySingle
		|| CSceneManager::GetInst()->GetCurrentSceneType() == EGameScene::LobbyMulti)
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

bool CPlayerGraphicObject::SetChangeGraphic(int index)
{
	// 텍스처를 아예 바꾼다면 mAnimation 에서 처리 해야 함.

	// 현재는 컬러로만.
	auto playerStat = CDataStorageManager::GetInst()->GetCharacterState(index);
	mRoot->SetColor(FVector4D::GetColorFromString(playerStat.ColorName));

	FVector3D tempRootSize = FVector3D(playerStat.SizeX, playerStat.SizeY, 1.f);
	mRoot->SetWorldScale(tempRootSize);
	mAnimation->ChangeAnimation(playerStat.ImageSequenceName);

#ifdef _DEBUG
	auto sceneType = CSceneManager::GetInst()->GetCurrentSceneType();
	if (sceneType == EGameScene::InGameSingle
		|| sceneType == EGameScene::InGameMulti)
	{
		mRoot->SetOpacity(0.5f);
	}
#endif // _DEBUG

	return false;
}
