#include "LineGroupObject.h"
#include "Object/SceneObject.h"
#include "Component/ColliderLine2D.h"
#include "Component/SpriteComponent.h"
#include "Device.h"
#include "Scene/Input.h"
#include "Scene/Scene.h"
#include "Etc/DataStorageManager.h"

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
	mToAddPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.5f);
	mCurLineNodeIndex = 0;
	mDifficultyRate = CDataStorageManager::GetInst()->GetSelectedMapInfo().DifficultyRate;
	mIsStart = false;

	mRoot = CreateComponent<CSceneComponent>("Root");
	SetRootComponent(mRoot);

#ifdef _DEBUG
	mScene->GetInput()->AddBindKey("PauseMove", 'P');
	mScene->GetInput()->AddBindFunction<CLineGroupObject>("PauseMove",
		EInputType::Down, this, &CLineGroupObject::PauseMove);
#endif // _DEBUG

	InitLines();

	return true;
}

void CLineGroupObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	if (!mIsStart)
		return;

	MoveLines(DeltaTime);
}

void CLineGroupObject::InitLines()
{
	for (int i = 0; i < maxLineCount; i++, mCurLineNodeIndex++)
	{
		AddLine(ELinePosType::Top, mCurLineNodeIndex);
		AddLine(ELinePosType::Bottom, mCurLineNodeIndex);
	}
}

void CLineGroupObject::AddLine(ELinePosType::Type type, int lineNodeIndex)
{
	auto lineNode = CDataStorageManager::GetInst()->GetLineNodeInSelectedMap(lineNodeIndex);
	auto lineNodeNext = CDataStorageManager::GetInst()->GetLineNodeInSelectedMap(lineNodeIndex + 1);

	FLine2D lineInfo;
	if (type == ELinePosType::Top)
	{
		// top
		lineInfo.Start = FVector2D(mSnapXValue * lineNodeIndex, lineNode.TopYPos) - mToAddPos;
		lineInfo.End = FVector2D(mSnapXValue * (lineNodeIndex + 1), lineNodeNext.TopYPos) - mToAddPos;
		mTopLine2DInfos.push_back(lineInfo);
	}
	else
	{
		// bottom
		lineInfo.Start = FVector2D(mSnapXValue * lineNodeIndex, lineNode.BottomYPos) - mToAddPos;
		lineInfo.End = FVector2D(mSnapXValue * (lineNodeIndex + 1), lineNodeNext.BottomYPos) - mToAddPos;
		mBottomLine2DInfos.push_back(lineInfo);
	}

	CSharedPtr<CSpriteComponent> tempSpriteComp = CreateComponent<CSpriteComponent>();
	mRoot->AddChild(tempSpriteComp);
	tempSpriteComp->SetTexture("basic", TEXT("Texture/basic.png"));
	tempSpriteComp->SetPivot(FVector2D::Axis[EAxis::X] * 0.5f);
	tempSpriteComp->SetWorldPos(lineInfo.Start);
	tempSpriteComp->SetWorldRotationZ(lineInfo.GetRotationAngle());
	tempSpriteComp->SetWorldScale(FVector2D(20.0f, lineInfo.GetLength()));
	tempSpriteComp->SetColor(FVector4D::Green);
#ifdef _DEBUG
	tempSpriteComp->SetOpacity(0.5f);
#endif // _DEBUG

	CSharedPtr<CColliderLine2D> tempLine2DComp = CreateComponent<CColliderLine2D>();
	tempSpriteComp->AddChild(tempLine2DComp);
	tempLine2DComp->SetCollisionProfile("Map");
	tempLine2DComp->SetRelativePos(FVector2D::Zero);
	tempLine2DComp->SetWorldRotationZ(lineInfo.GetRotationAngle());
	tempLine2DComp->SetLineDistance(lineInfo.GetLength());

	if (type == ELinePosType::Top)
	{
		mTopLines.push_back(tempSpriteComp);
		mTopColliderLines.push_back(tempLine2DComp);
	}
	else
	{
		mBottomLines.push_back(tempSpriteComp);
		mBottomColliderLines.push_back(tempLine2DComp);
	}
}

void CLineGroupObject::RemoveLine()
{
	// MoveLines() 으로부터 들어와야 함.
	// 하나를 지웠다면, AddLine() 해서 하나 추가하자, 
	// -> 끝났다면, 난이도 비율 올리자 -> 좀더 좁혀지도록.

	mTopLine2DInfos.pop_front();
	mTopLines.pop_front();
	mTopColliderLines.pop_front();
	mBottomLine2DInfos.pop_front();
	mBottomLines.pop_front();
	mBottomColliderLines.pop_front();

	mCurLineNodeIndex++;
	AddLine(ELinePosType::Top, mCurLineNodeIndex);
	AddLine(ELinePosType::Bottom, mCurLineNodeIndex);
}

void CLineGroupObject::MoveLines(float DeltaTime)
{
	// 라인이 이동하고, begin 의 start 에서
	// -> line2DInfo 의 End.x 가 음수로 가면 pop_front
	// pop_front 가 일어났다면, push_back을 해준다
	// -> 데이터기반으로 push하든 수치 알고리즘으로 하든 알아서,
	FVector3D tempPos = mRoot->GetWorldPosition();
	tempPos += FVector3D::Axis[EAxis::X] * -300 * DeltaTime;

	if (tempPos.x < -mSnapXValue)
	{
		tempPos = FVector3D::Zero;
		CLog::PrintLog("CLineGroupObject::MoveLines if (tempPos.x < -mSnapXValue)");
		CLog::PrintLog("CLineGroupObject::MoveLines tempPos.x: " + std::to_string(tempPos.x));

		// 앞에꺼 하나씩 빼.
		RemoveLine();
	}

	mRoot->SetWorldPos(tempPos);
}

void CLineGroupObject::ArrangeLines()
{
	// mRoot가 움직였으니 자식 라인들도 움직여야 함.
}

void CLineGroupObject::PauseMove(float DeltaTime)
{
	CLog::PrintLog("CLineGroupObject::PauseMove");
	mIsStart = !mIsStart;
}


void CLineGroupObject::SetStart(bool isStart)
{
	this->mIsStart = isStart;
}

