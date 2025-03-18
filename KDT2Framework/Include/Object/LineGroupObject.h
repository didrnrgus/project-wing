#pragma once
#include "Object/SceneObject.h"
#include "Etc/JsonContainer.h"
#include "Interface/IGamePlayController.h"

namespace ELinePosType
{
	enum Type
	{
		Top = 0,
		Bottom
	};
}

class CLineGroupObject : public CSceneObject, public IGamePlayController
{
	friend class CScene;

protected:
	CLineGroupObject();
	CLineGroupObject(const CLineGroupObject& Obj);
	CLineGroupObject(CLineGroupObject&& Obj);
	virtual ~CLineGroupObject();

protected:
	CSharedPtr<class CSceneComponent> mRoot;
	class IPlayerStatController* mTargetPlayerStat;

	// mLines & mColliderLines 는 쌍을 이뤄야 함.
	std::list<FLine2D> mTopLine2DInfos;
	std::list<CSharedPtr<class CSpriteComponent>> mTopLines;
	std::list<CSharedPtr<class CColliderLine2D>> mTopColliderLines;
	std::list<FLine2D> mBottomLine2DInfos;
	std::list<CSharedPtr<class CSpriteComponent>> mBottomLines;
	std::list<CSharedPtr<class CColliderLine2D>> mBottomColliderLines;

	FVector2D mToAddPos;
	const int mMaxLineCount = 7; // 보여질 라인
	const float mSnapXValue = 300.0f; // 데이터는 Y값만 있을거라서 -> 라인의 x축 투영길이.
	float mDifficultyRate;
	int mCurLineNodeIndex;
	int mLineNodesCount;
	int mLineNodesCycleCount;

	float mMovedValue;

public:
	virtual bool Init() override;
	virtual void PreUpdate(float DeltaTime) override;
	virtual void Update(float DeltaTime) override;

	virtual void SetGamePlayState(EGamePlayState::Type type) override;
public:
	// 씬에서 호출 -> 로드된 데이터를 받는 용도.
	void InitLines();
	void AddLine(ELinePosType::Type type, int lineNodeIndex);
	void AddLineSetting(ELinePosType::Type type, int lineNodeIndex
		, class CSpriteComponent* spriteComponent, class CColliderLine2D* colliderLine3D);
	void ArrangeLines();
	void MoveLines(float DeltaTime);
	template<typename T>
	void MoveLine(std::list<CSharedPtr<T>>& list, FVector3D moveVal);

public:
	void SetTargetStat(class IPlayerStatController* targetPlayerStat) { mTargetPlayerStat = targetPlayerStat; }
};

