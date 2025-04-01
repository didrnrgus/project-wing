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
	mResolution = FVector2D(RS.Width, RS.Height);

	if (CSceneManager::GetInst()->GetCurrentSceneType() == EGameScene::Lobby)
	{
		FVector2D tempPos = FVector2D(mResolution.x * 0.8f, mResolution.y * 0.75f) - mResolution * 0.5f;
		mRootInitPos = FVector3D(tempPos.x, tempPos.y, 0.0f);
	}
	else
	{
		mRootInitPos = FVector3D(RS.Width * 0.15f * -1.0f, 0.0f, 0.f);
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

	mDeadSign = CreateComponent<CSpriteComponent>(TEXTURE_NAME_DEAD_SIGN);
	mRoot->AddChild(mDeadSign);
	mDeadSign->SetTexture(TEXTURE_NAME_DEAD_SIGN, TEXTURE_PATH_DEAD_SIGN);
	mDeadSign->SetPivot(FVector2D::One * 0.5f);
	mDeadSign->SetColor(FVector4D::Red * 0.8f);
	mDeadSign->SetWorldScale(FVector2D(1.1f, 1.0f) * 128.0f * 1.5f);
	//CRenderManager::GetInst()->MoveRenderElement(mDeadSign, true);
	mDeadSign->SetEnable(false);

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
	if (CSceneManager::GetInst()->GetCurrentSceneType() == EGameScene::Lobby)
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

void CPlayerGraphicObject::SetMovePlayer(FVector3D _worldPos)
{
	mRoot->SetWorldPos(_worldPos);
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
	if (sceneType == EGameScene::InGame)
	{
		mRoot->SetOpacity(0.5f);
	}
#endif // _DEBUG

	return false;
}
