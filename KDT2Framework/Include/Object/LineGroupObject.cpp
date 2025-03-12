#include "LineGroupObject.h"
#include "Object/SceneObject.h"
#include "Component/ColliderLine2D.h"
#include "Component/SpriteComponent.h"
#include "Device.h"
#include "Scene/Input.h"
#include "Scene/Scene.h"

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
	FResolution rs = CDevice::GetInst()->GetResolution();
	mToAddPos = FVector2D(rs.Width * 0.5f, rs.Height * 0.5f);
	maxLineCount = 6;
	curLineNodeIndex = 0;
	difficultyRate = 1.0f;
	snapXValue = 300.0f;
	isStart = false;

#ifdef _DEBUG
	mScene->GetInput()->AddBindKey("PauseMove", 'P');
	mScene->GetInput()->AddBindFunction<CLineGroupObject>("PauseMove",
		EInputType::Down, this, &CLineGroupObject::PauseMove);
	MakeJson(); // test
#endif // _DEBUG
	
	

	return true;
}

void CLineGroupObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	if (!isStart)
		return;

	MoveLines(DeltaTime);
}

void CLineGroupObject::InitLines()
{
	// 데이터는 매니저에서 꺼내 쓰자.
	for (int i = 0; i < maxLineCount; i++)
	{
		FLine2D lineInfo;
		lineInfo.Start = FVector2D(0, 0) - mToAddPos;
		lineInfo.End = FVector2D(snapXValue, 350.0f) - mToAddPos;
		mLine2DInfos.push_back(lineInfo);
		AddLine(lineInfo);
	}
}

void CLineGroupObject::AddLine(const FLine2D lineInfo)
{
	CSharedPtr<CSpriteComponent> tempSpriteComp = CreateComponent<CSpriteComponent>();
	SetRootComponent(tempSpriteComp);
	tempSpriteComp->SetTexture("basic", TEXT("Texture/basic.png"));
	tempSpriteComp->SetPivot(FVector2D::Axis[EAxis::X] * 0.5f);
	tempSpriteComp->SetWorldPos(lineInfo.Start);
	tempSpriteComp->SetWorldRotationZ(lineInfo.GetRotationAngle());
	tempSpriteComp->SetWorldScale(FVector2D(20.0f, lineInfo.GetLength()));
	tempSpriteComp->SetColor(FVector4D::Green);
#ifdef _DEBUG
	tempSpriteComp->SetOpacity(0.5f);
#endif // _DEBUG
	mLines.push_back(tempSpriteComp);

	CSharedPtr<CColliderLine2D> tempLine2DComp = CreateComponent<CColliderLine2D>();
	tempSpriteComp->AddChild(tempLine2DComp);
	tempLine2DComp->SetCollisionProfile("Map");
	tempLine2DComp->SetRelativePos(FVector2D::Zero);
	tempLine2DComp->SetWorldRotationZ(lineInfo.GetRotationAngle());
	tempLine2DComp->SetLineDistance(lineInfo.GetLength());
	mColliderLines.push_back(tempLine2DComp);
}

void CLineGroupObject::RemoveLine()
{
	// MoveLines() 으로부터 들어와야 함.
	// 하나를 지웠다면, AddLine() 해서 하나 추가하자, 
	// -> 끝났다면, 난이도 비율 올리자 -> 좀더 좁혀지도록.
}

void CLineGroupObject::MoveLines(float DeltaTime)
{
	// 라인이 이동하고, begin 의 start 에서
	// -> line2DInfo 의 End.x 가 음수로 가면 pop_front
	// pop_front 가 일어났다면, push_back을 해준다
	// -> 데이터기반으로 push하든 수치 알고리즘으로 하든 알아서,
}

void CLineGroupObject::PauseMove(float DeltaTime)
{
	CLog::PrintLog("CLineGroupObject::PauseMove");
	isStart = !isStart;
}

// 구조체를 JSON으로 변환 (직렬화)
void to_json(nlohmann::json& j, const FLineNode& lineNode)
{
	j = nlohmann::json{
		{"topYPos", lineNode.TopYPos}
		, {"bottomYPos", lineNode.BottomYPos}
		, {"itemType", lineNode.ItemType}
		, {"obstacleType", lineNode.ObstacleType}
	};
}

// JSON을 구조체로 변환 (역직렬화)
void from_json(const nlohmann::json& j, FLineNode& lineNode)
{
	j.at("topYPos").get_to(lineNode.TopYPos);
	j.at("bottomYPos").get_to(lineNode.BottomYPos);
	j.at("itemType").get_to(lineNode.ItemType);
	j.at("obstacleType").get_to(lineNode.ObstacleType);
}

void CLineGroupObject::MakeJson()
{
    nlohmann::json jsonData;
    std::vector<FLineNode> nodes;
    
    nodes.push_back(FLineNode(720.0f, 0.0f, -1, -1));
    nodes.push_back(FLineNode(700.0f, 20.0f, -1, -1));
    nodes.push_back(FLineNode(690.0f, 20.0f, -1, -1));
    nodes.push_back(FLineNode(700.0f, 50.0f, -1, -1));
    nodes.push_back(FLineNode(630.0f, 130.0f, -1, -1));

    jsonData["stageNum"] = 1;
    jsonData["lineNodeList"] = nodes;

    std::string strJson = jsonData.dump(4);
    
}

void CLineGroupObject::SetStart(bool isStart)
{
	this->isStart = isStart;
}

