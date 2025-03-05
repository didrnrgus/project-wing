#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <crtdbg.h>
#include <string>
#include <functional>
#include <iostream>
#include <algorithm>

#include <d3d11.h>
#include <d3dcompiler.h>

//#include <dwrite.h> // Window7 이상 지원
//#include <dwrite_1.h>	// 위의 기능에 Window8 기능 추가 
//#include <dwrite_2.h> // 위의 기능에 Window8.1 기능 추가
#include <dwrite_3.h>	// 위의 기능에 Window10 기능 추가
#include <d2d1.h>

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"

#include "Share/SharedPtr.h"

// extern 은 선언된 전역변수를 다른곳에서 사용할 수 있게
// 해주는 기능이다.
extern TCHAR   gRootPath[MAX_PATH];
extern char   gRootPathMultibyte[MAX_PATH];

// .lib 를 링크 걸어주는 기능이다.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

#define	DECLARE_SINGLE(Type)	\
private:\
	Type();\
	~Type();\
private:\
	static Type*	mInst;\
public:\
	static Type* GetInst()\
	{\
		if(!mInst)\
			mInst = new Type;\
		return mInst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(mInst);\
	}

#define	DEFINITION_SINGLE(Type)	Type* Type::mInst = nullptr;

int Clamp(int Value, int Min, int Max);
float Clamp(float Value, float Min, float Max);


namespace EShaderBufferType
{
	enum Type
	{
		Vertex = 0x1,
		Pixel = 0x2,
		Hull = 0x4,
		Domain = 0x8,
		Geometry = 0x10,
		Compute = 0x20,
		Graphic = Vertex | Pixel | Hull | Domain | Geometry,
		All = Graphic | Compute
	};
}


struct FResolution
{
	unsigned int	Width = 0;
	unsigned int	Height = 0;
};

struct FVertexBuffer
{
	ID3D11Buffer* Buffer = nullptr;
	int			Size = 0;
	int			Count = 0;
	void* Data = nullptr;

	FVertexBuffer()
	{
	}

	~FVertexBuffer()
	{
		SAFE_RELEASE(Buffer);
		SAFE_DELETE_ARRAY(Data);
	}
};

struct FIndexBuffer
{
	// 데이터를 저장하기 위한 버퍼
	ID3D11Buffer* Buffer = nullptr;
	// 데이터 1개의 크기
	int		Size = 0;
	// 데이터 개수
	int		Count = 0;
	// 데이터 포맷
	DXGI_FORMAT	Fmt = DXGI_FORMAT_UNKNOWN;
	void* Data = nullptr;

	FIndexBuffer()
	{
	}

	~FIndexBuffer()
	{
		SAFE_RELEASE(Buffer);
		SAFE_DELETE_ARRAY(Data);
	}
};

struct FVertexColor
{
	FVector3D	Pos;
	FVector4D	Color;

	FVertexColor()
	{
	}

	FVertexColor(const FVector3D& _Pos, const FVector4D& _Color)	:
		Pos(_Pos),
		Color(_Color)
	{
	}

	FVertexColor(float x, float y, float z, float r, float g, float b, float a)	:
		Pos(x, y, z),
		Color(r, g, b, a)
	{
	}
};

struct FVertexTexture
{
	FVector3D	Pos;
	FVector2D	UV;

	FVertexTexture()
	{
	}

	FVertexTexture(float x, float y, float z, float u,
		float v) :
		Pos(x, y, z),
		UV(u, v)
	{
	}
};

enum class EColliderType : unsigned char
{
	Colider2D,
	Colider3D
};

enum class EColliderShape : unsigned char
{
	AABB2D,
	Sphere2D,
	OBB2D,
	Line2D
};

struct FAABB2D
{
	FVector2D	Min;
	FVector2D	Max;
};

struct FOBB2D
{
	FVector2D	Center;
	FVector2D	Axis[2];
	FVector2D	HalfSize;
};

struct FLine2D
{
	FVector2D	Start;
	FVector2D	End;
};

namespace ECollisionChannel
{
	enum Type : unsigned char
	{
		Default,
		Player,
		Monster,
		PlayerAttack,
		MonsterAttack,
		MonsterDetect,
		End
	};
}

namespace ECollisionInteraction
{
	enum Type : unsigned char
	{
		Ignore,			// 충돌 무시
		Collision,		// 충돌
		End
	};
}

/*
플레이어 프로파일을 만들고 채널을 Player로 지정했다.
이때 다른 채널을 사용하는 프로파일과 충돌해야 할지 여부를
판단하기 위해서 충돌상호작용이 다른 채널과 어떻게 되어 있는지를
저장해야 한다.

1번프로파일 정보
Channel : Player
Enable : true
Iteraction[Default] = Collision;
Iteraction[Player] = Collision;
Iteraction[Monster] = Collision;
Iteraction[PlayerAttack] = Ignore;
Iteraction[MonsterAttack] = Ignore;

2번프로파일 정보
Channel : MonsterAttack
Enable : true
Iteraction[Default] = Collision;
Iteraction[Player] = Collision;
Iteraction[Monster] = Ignore;
Iteraction[PlayerAttack] = Ignore;
Iteraction[MonsterAttack] = Ignore;
*/
struct FCollisionProfile
{
	std::string		Name;
	// 현재 프로파일이 사용하는 충돌채널
	ECollisionChannel::Type	Channel = ECollisionChannel::Default;
	bool			Enable = true;
	ECollisionInteraction::Type	Interaction[ECollisionChannel::End];
};

enum class EAssetType
{
	Mesh,
	Texture,
	Material,
	Animation2D,
	Sound
};

namespace ERenderStateType
{
	enum Type
	{
		Blend,
		Rasterizer,
		DepthStencil
	};
}

enum class EComponentRender
{
	None,
	Render
};

enum class ERenderSortType
{
	None,
	Y,
	Alpha
};

enum class EAnimationTextureType
{
	SpriteSheet,
	Frame
};

struct FAnimationFrame
{
	FVector2D   Start;
	FVector2D   Size;
};

enum class EItemType : unsigned char
{
	Weapon,
	Armor,
	Consume,
	Other
};

struct FItemData
{
	std::string		Name;
	EItemType		Type;
	CSharedPtr<class CTexture>	Icon;
};

enum class ETileShape
{
	Rect,
	Isometric,
	End
};

enum class ETileType
{
	None = -2,
	MouseOver,
	Normal,
	UnableToMove,
	End
};

enum class EEditorMode
{
	TileType,
	TileImage,
	End
};
