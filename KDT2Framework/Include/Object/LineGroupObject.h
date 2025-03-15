#pragma once
#include "Object/SceneObject.h"
#include "Etc/JsonContainer.h"

namespace ELinePosType
{
	enum Type
	{
		Top = 0,
		Bottom
	};
}

class CLineGroupObject : public CSceneObject
{
	friend class CScene;

protected:
	CLineGroupObject();
	CLineGroupObject(const CLineGroupObject& Obj);
	CLineGroupObject(CLineGroupObject&& Obj);
	virtual ~CLineGroupObject();

protected:
	CSharedPtr<class CSceneComponent> mRoot;

	// mLines & mColliderLines 는 쌍을 이뤄야 함.
	std::list<FLine2D> mTopLine2DInfos;
	std::list<CSharedPtr<class CSpriteComponent>> mTopLines;
	std::list<CSharedPtr<class CColliderLine2D>> mTopColliderLines;
	std::list<FLine2D> mBottomLine2DInfos;
	std::list<CSharedPtr<class CSpriteComponent>> mBottomLines;
	std::list<CSharedPtr<class CColliderLine2D>> mBottomColliderLines;

	FVector2D mToAddPos;
	const int maxLineCount = 7; // 보여질 라인
	const float mSnapXValue = 300.0f; // 데이터는 Y값만 있을거라서 -> 라인의 x축 투영길이.
	float mDifficultyRate;
	int mCurLineNodeIndex;
	int mLineNodesCycleCount;
	bool mIsStart;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

public:
	// 씬에서 호출 -> 로드된 데이터를 받는 용도.
	void InitLines();
	void AddLine(ELinePosType::Type type, int lineNodeIndex);
	void RemoveLine();
	void MoveLines(float DeltaTime);
	void ArrangeLines();
	void PauseMove(float DeltaTime);

public:
	void SetStart(bool isStart);
};

