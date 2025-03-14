#pragma once
#include "Object/SceneObject.h"
#include "Etc/JsonContainer.h"

//namespace EPositionType
//{
//	enum Type
//	{
//		Top = 0,
//		Bottom
//	};
//}

class CLineGroupObject : public CSceneObject
{
	friend class CScene;

protected:
	CLineGroupObject();
	CLineGroupObject(const CLineGroupObject& Obj);
	CLineGroupObject(CLineGroupObject&& Obj);
	virtual ~CLineGroupObject();

protected:
	// 미리 몽땅 들고있을까? -> SetData();
	std::list<FLine2D> mLine2DInfos;

	// mLines & mColliderLines 는 쌍을 이뤄야 함.
	std::list<CSharedPtr<class CSpriteComponent>> mLines;
	std::list<CSharedPtr<class CColliderLine2D>> mColliderLines;

	FVector2D mToAddPos;
	int maxLineCount;
	int curLineNodeIndex;
	float difficultyRate;
	float snapXValue; // 데이터는 Y값만 있을거라서 -> 라인의 x축 투영길이.
	bool isStart;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

public:
	// 씬에서 호출 -> 로드된 데이터를 받는 용도.
	void InitLines();
	void AddLine(FLine2D lineInfo);
	void RemoveLine();
	void MoveLines(float DeltaTime);
	void PauseMove(float DeltaTime);
	
	void MakeJson();

public:
	void SetStart(bool isStart);
};

