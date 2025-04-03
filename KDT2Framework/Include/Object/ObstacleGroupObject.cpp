#include "ObstacleGroupObject.h"
#include "Device.h"
#include "Component/SpriteComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ColliderOBB2D.h"
#include "Etc/DataStorageManager.h"
#include "Etc/NetworkManager.h"
#include "Interface/IPlayerStatController.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "UI/UserWidget/DebugWidget.h"

CObstacleGroupObject::CObstacleGroupObject()
{
}

CObstacleGroupObject::CObstacleGroupObject(const CObstacleGroupObject& Obj)
{
}

CObstacleGroupObject::CObstacleGroupObject(CObstacleGroupObject&& Obj)
{
}

CObstacleGroupObject::~CObstacleGroupObject()
{
	RemoveListener();
}

bool CObstacleGroupObject::Init()
{
	CSceneObject::Init();
	AddListener();

	auto mapInfo = CDataStorageManager::GetInst()->GetSelectedMapInfo();
	mMaxTriggerObstacleTime = mapInfo.ObstacleIntervalTime;

	FResolution RS = CDevice::GetInst()->GetResolution();
	mResolution = FVector2D(RS.Width, RS.Height);
	mCurTriggerObstacleTime = 0.0f;

	mRoot = CreateComponent<CSceneComponent>("mRoot");
	mRoot->SetWorldPos(FVector3D::One * 0.0f);
	mRoot->SetWorldScale(FVector3D::One);
	SetRootComponent(mRoot);

	for (int i = 0; i < mMaxObstacleCount; i++)
	{
		auto _image = CreateComponent<CSpriteComponent>("ObstacleRoot");
		mRoot->AddChild(_image);
		_image->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
		_image->SetPivot(0.5f, 0.5f);
		_image->SetColor(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
		_image->SetEnable(false);

		auto _body = CreateComponent<CColliderOBB2D>("CColliderOBB2D");
		_image->AddChild(_body);
		_body->SetCollisionProfile(PROFILE_MAP);
		_body->SetBoxSize(FVector2D::Zero);
		_body->SetEnable(false);

		mImages.push_back(_image);
		mBodies.push_back(_body);
	}
	mReadyIndex = 0;

	mDebugTextComp = CreateComponent<CWidgetComponent>("mDebugTextComp");
	mRoot->AddChild(mDebugTextComp);
	mDebugTextComp->SetRelativePos(FVector2D(-150.0f, 0.0f));
	mDebugWidget = mScene->GetUIManager()->CreateWidget<CDebugWidget>("mDebugWidget");
	mDebugTextComp->SetWidget(mDebugWidget);
#ifdef _DEBUG
	mDebugTextComp->SetEnable(true);
#else
	mDebugTextComp->SetEnable(false);
#endif // _DEBUG
	return true;
}

void CObstacleGroupObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	if (!IsGamePlayEnableByState())
		return;

	if (mTargetPlayerStat->GetIsStun())
		return;

	float boostMultiplyValue = mTargetPlayerStat->GetBoostValue();
	float moveValue = mTargetPlayerStat->GetSpeed() * DeltaTime * -1.0f * boostMultiplyValue;

	FVector3D moveValVector = FVector3D::Axis[EAxis::X] * moveValue;

	for (int i = 0; i < mMaxObstacleCount; i++)
	{
		auto pos = mImages[i]->GetWorldPosition();
		mImages[i]->SetWorldPos(pos + moveValVector);

		if (mImages[i]->GetWorldPosition().x < mResolution.x * 0.5f * -1.0f)
		{
			mImages[i]->SetEnable(false);
			mBodies[i]->SetEnable(false);
		}
	}

	if (!CNetworkManager::GetInst()->IsMultiplay())
	{
		mCurTriggerObstacleTime += DeltaTime;
		if (mCurTriggerObstacleTime >= mMaxTriggerObstacleTime)
		{
			mCurTriggerObstacleTime = 0.0f;
			SetTransformImfoAndStart(
				rand() % 50 + 100.0f	// scale
				, rand() % 360			// rotation
				, (rand() % (int)mResolution.y) - (mResolution.y * 0.5f));	// height
		}
	}
}

void CObstacleGroupObject::SetDebugText(const wchar_t* wstr)
{
	mDebugWidget->SetDebugText(wstr);
}

void CObstacleGroupObject::SetTransformImfoAndStart(float _scale, float _rot, float _height)
{
	auto _image = mImages[mReadyIndex];
	auto _body = mBodies[mReadyIndex];

	_image->SetWorldScale(FVector2D::One * _scale);
	_image->AddWorldRotationZ(_rot);

	auto scale = _image->GetWorldScale();
	_body->SetBoxSize(FVector2D::One * _scale * 0.7f);

	float _x = mResolution.x * 0.5f * 1.0f;
	_image->SetWorldPos(_x, _height, 0.0f);

	_image->SetEnable(true);
	_body->SetEnable(true);

	mReadyIndex++;
	if (mReadyIndex >= mMaxObstacleCount)
	{
		mReadyIndex = 0;
	}
}

void CObstacleGroupObject::AddListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->AddListener(this);
}

void CObstacleGroupObject::RemoveListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->RemoveListener(this);
}

void CObstacleGroupObject::ProcessMessage(const RecvMessage& msg)
{
	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_OBSTACLE:
	{
		const char* ptr = msg.body.data();
		float scale, rotation, height;
		memcpy(&scale, ptr, sizeof(float)); ptr += sizeof(float);
		memcpy(&rotation, ptr, sizeof(float)); ptr += sizeof(float);
		memcpy(&height, ptr, sizeof(float)); ptr += sizeof(float);

		//CLog::PrintLog(std::string("MSG_OBSTACLE ") + "scale: " + std::to_string(scale));
		//CLog::PrintLog(std::string("MSG_OBSTACLE ") + "rotation: " + std::to_string(rotation));
		//CLog::PrintLog(std::string("MSG_OBSTACLE ") + "height: " + std::to_string(height));

		SetTransformImfoAndStart(scale, rotation, height);
		break;
	}
	default:
		break;
	}
}
