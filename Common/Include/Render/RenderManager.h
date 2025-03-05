#pragma once

#include "../GameInfo.h"

struct FRenderLayer
{
	std::list<CSharedPtr<class CSceneComponent>>	RenderList;
	int			ZOrder = 0;
};

class CRenderManager
{
private:
	class CRenderStateManager* mStateManager = nullptr;
	std::map<int, FRenderLayer*>			mLayerList;
	std::unordered_map<std::string, int>	mLayerNameList;
	ERenderSortType	mRenderSortType = ERenderSortType::Y;
	ID3D11SamplerState* mSampler = nullptr;
	class CRenderState* mAlphaBlend = nullptr;
	class CRenderState* mDepthDisable = nullptr;
	CSharedPtr<class CWidget>	mMouseWidget;

public:
	class CRenderStateManager* GetStateManager()
	{
		return mStateManager;
	}

	void SetSortType(ERenderSortType Type)
	{
		mRenderSortType = Type;
	}

	void AddRenderList(class CSceneComponent* Component);
	void ClearRenderList();
	void SetMouseWidget(class CWidget* Widget);

public:
	bool Init();
	void Update(float DeltaTime);
	void Render();

public:
	bool CreateRenderLayer(const std::string& Name,
		int ZOrder);

private:
	FRenderLayer* FindLayer(const std::string& Name);

private:
	static bool SortY(const CSharedPtr<class CSceneComponent>& Src,
		const CSharedPtr<class CSceneComponent>& Dest);

	static bool SortAlpha(const CSharedPtr<class CSceneComponent>& Src,
		const CSharedPtr<class CSceneComponent>& Dest);

	DECLARE_SINGLE(CRenderManager)
};

