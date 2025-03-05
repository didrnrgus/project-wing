#pragma once

#include "../Share/Object.h"
#include "../Object/SceneObject.h"
#include "UIInfo.h"

class CWidget abstract	:
	public CObject
{
	friend class CSceneUIManager;

protected:
	CWidget();
	virtual ~CWidget();

protected:
	static FMatrix	mUIProj;

public:
	static void CreateUIProjection(float Width, float Height, float ViewDist)
	{
		mUIProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f,
			Width, 0.f, Height,
			0.f, ViewDist);
	}

protected:
	class CScene* mScene = nullptr;
	class CWidget* mParent = nullptr;
	CSharedPtr<CSceneObject>	mOwnerObject;
	CSharedPtr<class CShader>	mShader;
	CSharedPtr<class CMesh>		mMesh;
	class CUICBuffer* mUICBuffer = nullptr;
	class CTransformCBuffer* mTransformCBuffer = nullptr;
	std::string	mName;
	FVector2D	mPos;		// 상대적인 위치
	FVector2D	mRenderPos;	// 최종 출력 위치
	FVector2D	mSize;
	FVector2D	mPivot;
	float		mRotation = 0.f;

	int			mZOrder = 0;
	bool		mMouseOn = false;

public:
	void SetOwnerObject(class CSceneObject* Object)
	{
		mOwnerObject = Object;
	}

	void SetParent(CWidget* Widget)
	{
		mParent = Widget;
	}

public:
	const FVector2D& GetPos()	const
	{
		return mPos;
	}

	const FVector2D& GetRenderPos()	const
	{
		return mRenderPos;
	}

	const FVector2D& GetSize()	const
	{
		return mSize;
	}

	const FVector2D& GetPivot()	const
	{
		return mPivot;
	}

	float GetRotation()	const
	{
		return mRotation;
	}

	int GetZOrder()	const
	{
		return mZOrder;
	}

public:
	virtual void SetPos(const FVector2D& Pos)
	{
		mPos = Pos;
	}

	virtual void SetPos(float x, float y)
	{
		mPos.x = x;
		mPos.y = y;
	}

	void AddPos(const FVector2D& Pos)
	{
		mPos += Pos;
	}

	void AddPos(float x, float y)
	{
		mPos.x += x;
		mPos.y += y;
	}

	void SetRenderPos(const FVector2D& Pos)
	{
		mRenderPos = mPos + Pos;
	}

	virtual void SetSize(const FVector2D& Size)
	{
		mSize = Size;
	}

	virtual void SetSize(float x, float y)
	{
		mSize.x = x;
		mSize.y = y;
	}

	void SetPivot(const FVector2D& Pivot)
	{
		mPivot = Pivot;
	}

	void SetPivot(float x, float y)
	{
		mPivot.x = x;
		mPivot.y = y;
	}

	void SetRotation(float Rotation)
	{
		if (Rotation > 360.f)
		{
			// 정수 부분만 남긴다.
			int	Angle1 = (int)Rotation;
			float Angle2 = Rotation - Angle1;

			Angle1 %= 360;

			mRotation = Angle1 + Angle2;
		}

		else if (Rotation < 0.f)
		{
			Rotation *= -1.f;

			// 정수 부분만 남긴다.
			int	Angle1 = (int)Rotation;
			float Angle2 = Rotation - Angle1;

			Angle1 %= 360;

			mRotation = 360.f - (Angle1 + Angle2);
		}

		else
			mRotation = Rotation;

	}

	void SetZOrder(int ZOrder)
	{
		mZOrder = ZOrder;
	}

public:
	void SetShader(const std::string& Name);
	void SetShader(class CShader* Shader);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
	virtual bool CollisionMouse(CWidget** Result, const FVector2D& MousePos);
	virtual void EndFrame();
	virtual void MouseHovered();
	virtual void MouseUnHovered();
};

