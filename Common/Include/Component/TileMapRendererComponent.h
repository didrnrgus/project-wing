#pragma once

#include "SceneComponent.h"

class CTileMapRendererComponent :
    public CSceneComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CTileMapRendererComponent();
	CTileMapRendererComponent(const CTileMapRendererComponent& Com);
	CTileMapRendererComponent(CTileMapRendererComponent&& Com);
	virtual ~CTileMapRendererComponent();

protected:
	CSharedPtr<class CMesh>			mMesh;
	CSharedPtr<class CShader>		mShader;
	CSharedPtr<class CTexture>		mBackTexture;
	CSharedPtr<class CTexture>		mTileTexture;
	class CTileMapComponent* mTileMap = nullptr;

public:
	CTexture* GetBackTexture()	const
	{
		return mBackTexture;
	}

	CTexture* GetTileTexture()	const
	{
		return mTileTexture;
	}

public:
	void SetTileMapComponent(CTileMapComponent* TileMap);

	void SetShader(const std::string& Name);
	void SetShader(class CShader* Shader);
	void SetBackTexture(const std::string& Name);
	void SetBackTexture(const std::string& Name,
		const TCHAR* FileName);
	void SetBackTexture(class CTexture* Texture);
	void SetTileTexture(const std::string& Name);
	void SetTileTexture(const std::string& Name,
		const TCHAR* FileName);
	void SetTileTexture(class CTexture* Texture);

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTileMapRendererComponent* Clone();
};

