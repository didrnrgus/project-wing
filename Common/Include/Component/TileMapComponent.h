#pragma once

#include "Component.h"
#include "Tile.h"

class CTileMapComponent :
	public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CTileMapComponent();
	CTileMapComponent(const CTileMapComponent& Com);
	CTileMapComponent(CTileMapComponent&& Com);
	virtual ~CTileMapComponent();

protected:
	std::vector<CTile*>	mTileList;
	ETileShape	mTileShape = ETileShape::Rect;
	FVector2D	mTileSize;
	int			mCountX = 0;
	int			mCountY = 0;
	bool		mTileOutLineRender = false;
	CSharedPtr<class CMesh>			mOutLineMesh;
	CSharedPtr<class CShader>		mOutLineShader;
	CSharedPtr<class CMesh>			mTileMesh;
	CSharedPtr<class CShader>		mTileShader;
	class CTransformCBuffer* mLineTransformCBuffer = nullptr;
	class CColliderCBuffer* mColorCBuffer = nullptr;
	class CTileMapCBuffer* mTileMapCBuffer = nullptr;
	std::vector<FAnimationFrame>	mTileFrameList;
	FVector2D						mTileTextureSize;
	TCHAR		mBackFileName[MAX_PATH] = {};
	TCHAR		mTileFileName[MAX_PATH] = {};

	int			mViewStartX;
	int			mViewStartY;
	int			mViewEndX;
	int			mViewEndY;

public:
	int GetTileFrameCount()	const
	{
		return (int)mTileFrameList.size();
	}

	const FVector2D& GetTileSize()	const
	{
		return mTileSize;
	}

	int GetTileCountX()	const
	{
		return mCountX;
	}

	int GetTileCountY()	const
	{
		return mCountY;
	}

public:
	void SetTileTextureSize(const FVector2D& Size)
	{
		mTileTextureSize = Size;
	}

	void SetTileTextureSize(unsigned int Width,
		unsigned int Height)
	{
		mTileTextureSize.x = (float)Width;
		mTileTextureSize.y = (float)Height;
	}

public:
	void SetTileOutLineRender(bool Render);
	void SetTileTexture(const std::string& Name);
	void SetTileTexture(const std::string& Name,
		const TCHAR* FileName);
	void SetTileTexture(class CTexture* Texture);
	void AddTileTextureFrame(const FVector2D& Start,
		const FVector2D& Size);
	void AddTileTextureFrame(float StartX, float StartY,
		float SizeX, float SizeY);

public:
	int GetTileIndexX(const FVector3D& Pos)	const;
	int GetTileIndexX(const FVector2D& Pos)	const;
	int GetTileIndexX(float x)	const;

	int GetTileIndexY(const FVector3D& Pos)	const;
	int GetTileIndexY(const FVector2D& Pos)	const;
	int GetTileIndexY(float y)	const;

	int GetTileIndex(const FVector3D& Pos)	const;
	int GetTileIndex(const FVector2D& Pos)	const;
	int GetTileIndex(float x, float y)	const;

	const CTile* GetTile(const FVector3D& Pos)	const;
	const CTile* GetTile(const FVector2D& Pos)	const;
	const CTile* GetTile(float x, float y)	const;

	ETileType ChangeTileType(ETileType Type, const FVector3D& Pos);
	ETileType ChangeTileType(ETileType Type, const FVector2D& Pos);
	ETileType ChangeTileType(ETileType Type, float x, float y);
	ETileType ChangeTileType(ETileType Type, int Index);

	void ChangeTileFrame(int Frame, const FVector3D& Pos);
	void ChangeTileFrame(int Frame, const FVector2D& Pos);
	void ChangeTileFrame(int Frame, float x, float y);
	void ChangeTileFrame(int Frame, int Index);

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTileMapComponent* Clone();
	virtual void EndFrame();

public:
	void RenderTile();
	void RenderTileOutLine();

public:
	void CreateTile(ETileShape Shape, int CountX, int CountY,
		const FVector2D& TileSize, int TileTextureFrame = -1);
	void Save(const char* FileName);
	void Load(const char* FileName);
};

