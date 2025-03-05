#pragma once

#include "Scene/Scene.h"

class CSceneEditor :
    public CScene
{
	friend class CSceneManager;

private:
	CSceneEditor();
	virtual ~CSceneEditor();

private:
	CSharedPtr<class CTileMapObj>	mTileMapObj;
	EEditorMode	mEditorMode = EEditorMode::TileType;

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();

private:
	void EditorMode(float DeltaTime);
	void TileTypeKey(float DeltaTime);
	void TileFrameKey(float DeltaTime);
};

