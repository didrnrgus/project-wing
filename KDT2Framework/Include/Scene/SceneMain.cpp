#include "SceneMain.h"
#include "../Object/PlayerObject.h"
#include "../Object/GunnerMonster.h"
#include "../Object/NearingMonster.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/SceneAssetManager.h"
#include "Asset/Material/Material.h"
#include "../UI/UserWidget/MainWidget.h"
#include "Scene/SceneUIManager.h"

CSceneMain::CSceneMain()
{
}

CSceneMain::~CSceneMain()
{
}

bool CSceneMain::InitAsset()
{
    mAssetManager->CreateMaterial("Monster1");

    CMaterial* Material =
        mAssetManager->FindMaterial("Monster1");

    Material->SetPixelShader("DefaultMaterialShader");
    Material->SetSamplerType(ETextureSamplerType::Linear);
    Material->AddTexture("Monster1", TEXT("Texture/block_wall.png"),
        0);

    mAssetManager->LoadSound("Hit", "Effect", false, "Sound/Fire1.wav");

    return true;
}

bool CSceneMain::InitObject()
{
    CPlayerObject* Player = CreateObj<CPlayerObject>("Player");


    CMonsterObject* Monster = CreateObj<CGunnerMonster>("Monster");
    Monster->SetWorldPos(900.f, 700.f);
    Monster = CreateObj<CNearingMonster>("Monster");
    Monster->SetTarget(Player);
    Monster->SetWorldPos(-400.f, 0.f);

    return true;
}

bool CSceneMain::InitWidget()
{
    CMainWidget* Widget = mUIManager->CreateWidget<CMainWidget>("Main");

    mUIManager->AddToViewport(Widget);

    return true;
}
