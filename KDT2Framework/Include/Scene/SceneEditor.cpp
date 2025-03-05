#include "SceneEditor.h"
#include "../Object/TileMapObj.h"
#include "../Object/EditorPlayer.h"
#include "Scene/Input.h"

CSceneEditor::CSceneEditor()
{
}

CSceneEditor::~CSceneEditor()
{
}

bool CSceneEditor::InitAsset()
{
    return true;
}

bool CSceneEditor::InitObject()
{
    CEditorPlayer* EditorPlayer = CreateObj<CEditorPlayer>("EditorPlayer");

    mTileMapObj = CreateObj<CTileMapObj>("TileMap");


    mInput->AddBindKey("EditorMode", '1');

    mInput->AddBindFunction<CSceneEditor>("EditorMode",
        EInputType::Down, this, &CSceneEditor::EditorMode);

    mInput->AddBindKey("TileType", '2');

    mInput->AddBindFunction<CSceneEditor>("TileType",
        EInputType::Down, this, &CSceneEditor::TileTypeKey);

    mInput->AddBindKey("TileFrame", '3');

    mInput->AddBindFunction<CSceneEditor>("TileFrame",
        EInputType::Down, this, &CSceneEditor::TileFrameKey);

    return true;
}

bool CSceneEditor::InitWidget()
{
    return true;
}

void CSceneEditor::EditorMode(float DeltaTime)
{
    mEditorMode = (EEditorMode)((int)mEditorMode + 1);

    if (mEditorMode == EEditorMode::End)
        mEditorMode = EEditorMode::TileType;

    mTileMapObj->SetEditorMode(mEditorMode);
}

void CSceneEditor::TileTypeKey(float DeltaTime)
{
    mTileMapObj->AddTileType();
}

void CSceneEditor::TileFrameKey(float DeltaTime)
{
    mTileMapObj->AddTileFrame();
}
