#pragma once

#include "Component.h"

class CInventoryComponent :
    public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CInventoryComponent();
	CInventoryComponent(const CInventoryComponent& Com);
	CInventoryComponent(CInventoryComponent&& Com);
	virtual ~CInventoryComponent();

protected:
	std::vector<FItemData*>	mItemList;
	std::function<void(const FItemData*, int)>	mChangeFunction;

public:
	void AddItem(FItemData* Item);
	void RemoveItem(int Index);

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CInventoryComponent* Clone();
	virtual void EndFrame();

public:
	template <typename T>
	void SetChangeFunction(T* Obj, 
		void(T::* Func)(const FItemData*, int))
	{
		mChangeFunction = std::bind(Func, Obj,
			std::placeholders::_1, std::placeholders::_2);
	}
};

