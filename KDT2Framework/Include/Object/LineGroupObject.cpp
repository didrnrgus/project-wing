﻿#include "LineGroupObject.h"
#include "Object/SceneObject.h"
#include "Component/ColliderLine2D.h"
#include "Component/SpriteComponent.h"
#include "Device.h"
#include "Scene/Input.h"
#include "Scene/Scene.h"
#include "Etc/DataStorageManager.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"

CLineGroupObject::CLineGroupObject()
{
}

CLineGroupObject::CLineGroupObject(const CLineGroupObject& Obj)
{
}

CLineGroupObject::CLineGroupObject(CLineGroupObject&& Obj)
{
}

CLineGroupObject::~CLineGroupObject()
{
}

bool CLineGroupObject::Init()
{
	CSceneObject::Init();
	FResolution RS = CDevice::GetInst()->GetResolution();
	mResolution = FVector2D(RS.Width, RS.Height);
	mToAddPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.5f);
	mCurLineNodeIndex = 0;
	mLineNodesCount = CDataStorageManager::GetInst()->GetLineNodeCountInSelectedMap();
	mLineNodesCycleCount = 0;
	mDifficultyRate = CDataStorageManager::GetInst()->GetSelectedMapInfo().DifficultyRate;
	mMovedValue = 0.0f;
	mIsTitle = false;

	mRoot = CreateComponent<CSceneComponent>("Root");
	SetRootComponent(mRoot);

	InitLines();

	return true;
}

void CLineGroupObject::PreUpdate(float DeltaTime)
{
	CSceneObject::PreUpdate(DeltaTime);

	if (!IsGamePlayEnableByState())
		return;

	if (mTargetPlayerStat->GetIsStun())
		return;

	MoveLines(DeltaTime);
}

void CLineGroupObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
}

void CLineGroupObject::SetGamePlayState(EGamePlayState::Type type)
{
	IGamePlayStateController::SetGamePlayState(type);

	CLog::PrintLog("CLineGroupObject::SetGamePlayState type: " + std::to_string(type));
}

void CLineGroupObject::InitLines()
{
	for (int i = 0; i < mMaxLineCount; i++, mCurLineNodeIndex++)
	{
		if (i > 0)
		{
			if (mCurLineNodeIndex % mLineNodesCount == 0)
			{
				mLineNodesCycleCount++;
			}
		}

		AddLine(ELinePosType::Top, mCurLineNodeIndex);
		AddLine(ELinePosType::Bottom, mCurLineNodeIndex);
	}
}

void CLineGroupObject::AddLine(ELinePosType::Type type, int lineNodeIndex)
{
	auto mapInfo = CDataStorageManager::GetInst()->GetSelectedMapInfo();

	CSharedPtr<CSpriteComponent> tempSpriteComp = CreateComponent<CSpriteComponent>();
	mRoot->AddChild(tempSpriteComp);
	tempSpriteComp->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	tempSpriteComp->SetPivot(FVector2D::Axis[EAxis::X] * 0.5f);

	tempSpriteComp->SetColor(FVector4D::GetColorFromString(mapInfo.ColorName));
#ifdef _DEBUG
	tempSpriteComp->SetOpacity(0.5f);
#endif // _DEBUG

	CSharedPtr<CColliderLine2D> tempLine2DComp = CreateComponent<CColliderLine2D>();
	tempSpriteComp->AddChild(tempLine2DComp);
	tempLine2DComp->SetCollisionProfile(PROFILE_MAP);
	tempLine2DComp->SetRelativePos(FVector2D::Zero);

	CSharedPtr<CColliderLine2D> tempLine2DCompForEmptySpace = CreateComponent<CColliderLine2D>();
	tempSpriteComp->AddChild(tempLine2DCompForEmptySpace);
	tempLine2DCompForEmptySpace->SetCollisionProfile(PROFILE_MAP);
	tempLine2DCompForEmptySpace->SetRelativePos(FVector2D::Zero);

	AddLineSetting(type, lineNodeIndex, tempSpriteComp, tempLine2DComp, tempLine2DCompForEmptySpace);
}

void CLineGroupObject::AddLineSetting(ELinePosType::Type type, int lineNodeIndex
	, CSpriteComponent* spriteComponent
	, CColliderLine2D* colliderLine2D
	, CColliderLine2D* colliderLine2DForEmptySpace)
{
	auto lineNode = CDataStorageManager::GetInst()->GetLineNodeInSelectedMap(lineNodeIndex);
	auto lineNodeNext = CDataStorageManager::GetInst()->GetLineNodeInSelectedMap(lineNodeIndex + 1);

	lineNodeIndex = lineNodeIndex >= mMaxLineCount - 1 ? mMaxLineCount - 1 : lineNodeIndex;
	float startX = mSnapXValue * lineNodeIndex;
	float endX = mSnapXValue * (lineNodeIndex + 1);
	float startY, endY;
	FLine2D lineInfo;
	if (type == ELinePosType::Top)
	{
		startY = lineNode.TopYPos - mDifficultyRate * mLineNodesCycleCount;
		endY = lineNodeNext.TopYPos - mDifficultyRate * mLineNodesCycleCount;
		lineInfo.Start = FVector2D(startX, startY) - mToAddPos;
		lineInfo.End = FVector2D(endX, endY) - mToAddPos;
		mTopLine2DInfos.push_back(lineInfo);
	}
	else
	{
		startY = lineNode.BottomYPos + mDifficultyRate * mLineNodesCycleCount;
		endY = lineNodeNext.BottomYPos + mDifficultyRate * mLineNodesCycleCount;
		lineInfo.Start = FVector2D(startX, startY) - mToAddPos;
		lineInfo.End = FVector2D(endX, endY) - mToAddPos;
		mBottomLine2DInfos.push_back(lineInfo);
	}

	spriteComponent->SetWorldPos(lineInfo.Start);
	spriteComponent->SetWorldRotationZ(lineInfo.GetRotationAngle());
	spriteComponent->SetWorldScale(FVector2D(20.0f, lineInfo.GetLength()));
	colliderLine2D->SetWorldRotationZ(lineInfo.GetRotationAngle());
	colliderLine2D->SetLineDistance(lineInfo.GetLength());
	colliderLine2DForEmptySpace->SetLineDistance(mResolution.y * 0.5f);

	if (type == ELinePosType::Top)
	{
		colliderLine2DForEmptySpace->SetWorldRotationZ(0.0f);
		mTopLines.push_back(spriteComponent);
		mTopColliderLines.push_back(colliderLine2D);
		mTopColliderLinesForEmptySpace.push_back(colliderLine2DForEmptySpace);
	}
	else
	{
		colliderLine2DForEmptySpace->SetWorldRotationZ(180.0f);
		mBottomLines.push_back(spriteComponent);
		mBottomColliderLines.push_back(colliderLine2D);
		mBottomColliderLinesForEmptySpace.push_back(colliderLine2DForEmptySpace);
	}
}

void CLineGroupObject::ArrangeLines()
{
	//CLog::PrintLog("CLineGroupObject::ArrangeLines");

	(*mTopLines.begin()).Get()->Destroy();
	(*mTopColliderLines.begin()).Get()->Destroy();
	(*mTopColliderLinesForEmptySpace.begin()).Get()->Destroy();
	(*mBottomLines.begin()).Get()->Destroy();
	(*mBottomColliderLines.begin()).Get()->Destroy();
	(*mBottomColliderLinesForEmptySpace.begin()).Get()->Destroy();

	mTopLine2DInfos.pop_front();
	mTopLines.pop_front();
	mTopColliderLines.pop_front();
	mTopColliderLinesForEmptySpace.pop_front();
	mBottomLine2DInfos.pop_front();
	mBottomLines.pop_front();
	mBottomColliderLines.pop_front();
	mBottomColliderLinesForEmptySpace.pop_front();

	mCurLineNodeIndex++;
	if (mCurLineNodeIndex % mLineNodesCount == 0)
	{
		if (!mIsTitle)
			mLineNodesCycleCount++;
	}

	AddLine(ELinePosType::Top, mCurLineNodeIndex);
	AddLine(ELinePosType::Bottom, mCurLineNodeIndex);
}

void CLineGroupObject::MoveLines(float DeltaTime)
{
	float boostMultiplyValue = mTargetPlayerStat->GetBoostValue();
	float moveValue = mTargetPlayerStat->GetSpeed() * DeltaTime * -1.0f * boostMultiplyValue;

	mMovedValue += moveValue;

	if (mMovedValue < -mSnapXValue)
	{
		//CLog::PrintLog("CLineGroupObject::MoveLines if (mMovedValue < -mSnapXValue)");
		//CLog::PrintLog("CLineGroupObject::MoveLines mMovedValue: " + std::to_string(mMovedValue));

		// 앞에꺼 하나씩 빼서 뒤에 붙인다.
		ArrangeLines();
		mMovedValue = 0.0f;
	}
	else
	{
		FVector3D moveValVector = FVector3D::Axis[EAxis::X] * moveValue;

		MoveLine(mTopLines, moveValVector);
		MoveLine(mTopColliderLines, moveValVector);
		MoveLine(mBottomLines, moveValVector);
		MoveLine(mBottomColliderLines, moveValVector);
	}

}

template<typename T>
void CLineGroupObject::MoveLine(std::list<CSharedPtr<T>>& list
	, FVector3D moveVal)
{
	auto it = list.begin();
	auto check = (*it).Get();

	if (!check)
	{
		CLog::PrintLog("CLineGroupObject::MoveLine check error");
		return;
	}

	CSceneComponent* result = dynamic_cast<CSceneComponent*>(check);

	if (!result)
	{
		CLog::PrintLog("CLineGroupObject::MoveLine result error");
		return;
	}

	auto end = list.end();

	for (; it != end; ++it)
	{
		CSceneComponent* comp = dynamic_cast<CSceneComponent*>((*it).Get());
		auto pos = comp->GetWorldPosition();
		comp->SetWorldPos(pos + moveVal);
	}
}
