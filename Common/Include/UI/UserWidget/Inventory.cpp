#include "Inventory.h"
#include "../Common/Button.h"
#include "../Common/TextBlock.h"
#include "../Common/TitleBar.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "Asset/Texture/Texture.h"

CInventory::CInventory()
{
}

CInventory::~CInventory()
{
}

bool CInventory::Init()
{
    CWindowWidget::Init();

    mScene->GetAssetManager()->LoadTexture("IconSword",
        TEXT("Texture/Icon_Sword.PNG"));

    mScene->GetAssetManager()->LoadTexture("IconShield",
        TEXT("Texture/Icon_Shield.PNG"));

    SetSize(400.f, 420.f);

    CSharedPtr<CTitleBar>    TitleBar =
        mScene->GetUIManager()->CreateWidget<CTitleBar>("TitleBar");

    TitleBar->SetSize(360.f, 20.f);
    TitleBar->SetPos(0.f, 400.f);
    TitleBar->SetTexture("MPBar", TEXT("Texture/MPBar.png"));
    TitleBar->SetText(TEXT("인벤토리"));

    TitleBar->SetUpdateWidget(this);

    AddWidget(TitleBar);

    CSharedPtr<CButton>    Button = mScene->GetUIManager()->CreateWidget<CButton>("TestButton");

    Button->SetPos(360.f, 400.f);
    Button->SetSize(40.f, 20.f);
    //Button->SetTint(EButtonState::Normal, 1.f, 0.f, 0.f);
    Button->SetTexture(EButtonState::Normal, "StartButton",
        TEXT("Texture/End.png"));
    Button->SetTexture(EButtonState::Hovered, "StartButton",
        TEXT("Texture/End.png"));
    Button->SetTexture(EButtonState::Click, "StartButton",
        TEXT("Texture/End.png"));

    Button->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
    Button->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
    Button->SetTint(EButtonState::Click, 0.6f, 0.6f, 0.6f);

    /*Button->SetSound(EButtonEventState::Hovered, "ButtonHovered",
        "Sound/TeemoSmile.mp3");
    Button->SetSound(EButtonEventState::Click, "ButtonClick",
        "Sound/TeemoStartClicck.mp3");*/

    Button->SetEventCallback<CInventory>(EButtonEventState::Click,
        this, &CInventory::CloseButton);

    AddWidget(Button);

    mSlotList.resize(25);

    std::string NameArray[2] =
    {
        "IconSword",
        "IconShield"
    };

    // 80, 80
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            std::string SlotName = "Slot" +
                std::to_string(i * 5 + j);

            CSlot* Slot =
                mScene->GetUIManager()->CreateWidget<CSlot>(SlotName);

            Slot->SetSize(80.f, 80.f);
            Slot->SetPos(j * 80.f, (4 - i) * 80.f);

            Slot->SetIndex(i * 5 + j);

            mSlotList[i * 5 + j] = Slot;

            AddWidget(Slot);
        }
    }

    /*for (int i = 0; i < 20; ++i)
    {
        int RandIndex = rand() % 2;

        mSlotList[i]->SetTextureIcon(NameArray[RandIndex]);
    }*/

    return true;
}

void CInventory::Update(float DeltaTime)
{
    CWindowWidget::Update(DeltaTime);
}

void CInventory::Render()
{
    CWindowWidget::Render();
}

void CInventory::CloseButton()
{
    SetEnable(false);
}

void CInventory::InventoryChange(const FItemData* Item, int Index)
{
    if (!Item)
        mSlotList[Index]->SetTextureIcon(nullptr);

    else
        mSlotList[Index]->SetTextureIcon(Item->Icon);
}
