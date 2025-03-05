#include "InventoryComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/Texture.h"
#include "Asset/Texture/TextureManager.h"
#include "../Scene/SceneUIManager.h"
#include "../UI/UserWidget/Inventory.h"

CInventoryComponent::CInventoryComponent()
{
}

CInventoryComponent::CInventoryComponent(
    const CInventoryComponent& Com) :
    CComponent(Com)
{
}

CInventoryComponent::CInventoryComponent(CInventoryComponent&& Com) :
    CComponent(Com)
{
}

CInventoryComponent::~CInventoryComponent()
{
    for (int i = 0; i < 25; ++i)
    {
        SAFE_DELETE(mItemList[i]);
    }
}

void CInventoryComponent::AddItem(FItemData* Item)
{
    for (int i = 0; i < 25; ++i)
    {
        if (!mItemList[i])
        {
            mItemList[i] = Item;

            if (mChangeFunction)
                mChangeFunction(Item, i);
            return;
        }
    }
}

void CInventoryComponent::RemoveItem(int Index)
{
    if (!mItemList[Index])
        return;

    SAFE_DELETE(mItemList[Index]);

    if (mChangeFunction)
        mChangeFunction(nullptr, Index);
}

bool CInventoryComponent::Init()
{
    if (!CComponent::Init())
        return false;

    CInventory* Inventory =
        dynamic_cast<CInventory*>(mScene->GetUIManager()->FindWindowWidget("Inventory"));

    if (Inventory)
    {
        SetChangeFunction<CInventory>(Inventory,
            &CInventory::InventoryChange);
    }

    mItemList.resize(25, nullptr);

    std::string NameArray[2] =
    {
        "IconSword",
        "IconShield"
    };

    for (int i = 0; i < 10; ++i)
    {
        int RandIndex = rand() % 2;

        FItemData* Data = new FItemData;

        Data->Name = NameArray[RandIndex];

        if (RandIndex == 0)
            Data->Type = EItemType::Weapon;

        else if (RandIndex == 1)
            Data->Type = EItemType::Armor;

        if (mScene)
            Data->Icon = mScene->GetAssetManager()->FindTexture(NameArray[RandIndex]);

        else
            Data->Icon = CAssetManager::GetInst()->GetTextureManager()->FindTexture(NameArray[RandIndex]);

        AddItem(Data);
    }

    return true;
}

bool CInventoryComponent::Init(const char* FileName)
{
    if (!CComponent::Init(FileName))
        return false;

    return true;
}

void CInventoryComponent::PreUpdate(float DeltaTime)
{
    CComponent::PreUpdate(DeltaTime);
}

void CInventoryComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);
}

void CInventoryComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);
}

CInventoryComponent* CInventoryComponent::Clone()
{
    return new CInventoryComponent(*this);
}

void CInventoryComponent::EndFrame()
{
    CComponent::EndFrame();
}

