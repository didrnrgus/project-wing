#pragma once

#include "WindowWidget.h"
#include "Slot.h"

class CInventory :
    public CWindowWidget
{
	friend class CSceneUIManager;

protected:
	CInventory();
	virtual ~CInventory();

protected:
	std::vector<CSharedPtr<CSlot>>	mSlotList;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

private:
	void CloseButton();

public:
	void InventoryChange(const FItemData* Item, int Index);
};

