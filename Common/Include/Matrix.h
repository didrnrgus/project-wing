#pragma once

#include "Vector4D.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include <string>

__declspec(align(16)) union FMatrix
{
	DirectX::XMMATRIX	m;

	struct
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};

	FVector4D	v[4];

	FMatrix()
	{
		// XMMatrixIdentity : 항등행렬을 만들어주는 함수.
		m = DirectX::XMMatrixIdentity();
	}

	FMatrix(const FMatrix& _m)
	{
		m = _m.m;
	}

	FMatrix(FMatrix&& _m)
	{
		m = _m.m;
	}

	FMatrix(const DirectX::XMMATRIX& _m)
	{
		m = _m;
	}

	FMatrix(const FVector4D _v[4])
	{
		memcpy(v, _v, sizeof(FVector4D) * 4);
	}

	FVector4D& operator [] (int Index)
	{
		assert(0 <= Index && Index <= 3);

		return v[Index];
	}

	const FMatrix& operator = (const FMatrix& _m)
	{
		m = _m.m;
		return *this;
	}

	const FMatrix& operator = (const DirectX::XMMATRIX& _m)
	{
		m = _m;
		return *this;
	}

	const FMatrix& operator = (const FVector4D _v[4])
	{
		memcpy(v, _v, sizeof(FVector4D) * 4);
		return *this;
	}

	FMatrix operator * (const FMatrix& _m)	const
	{
		return FMatrix(m * _m.m);
	}

	FMatrix operator * (const DirectX::XMMATRIX& _m)	const
	{
		return FMatrix(m * _m);
	}

	void Identity()
	{
		m = DirectX::XMMatrixIdentity();
	}

	void Transpose()
	{
		m = DirectX::XMMatrixTranspose(m);
	}

	void Inverse()
	{
		DirectX::XMVECTOR	det = DirectX::XMMatrixDeterminant(m);
		m = DirectX::XMMatrixInverse(&det, m);
	}

	void Scaling(const FVector3D& _v)
	{
		m = DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
	}

	void Scaling(float x, float y, float z)
	{
		m = DirectX::XMMatrixScaling(x, y, z);
	}

	void Scaling(const FVector2D& _v)
	{
		m = DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
	}

	void Scaling(float x, float y)
	{
		m = DirectX::XMMatrixScaling(x, y, 1.f);
	}

	void Rotation(const FVector3D& _v)
	{
		float x = DirectX::XMConvertToRadians(_v.x);
		float y = DirectX::XMConvertToRadians(_v.y);
		float z = DirectX::XMConvertToRadians(_v.z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		DirectX::XMVECTOR Quat =
			DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		m = DirectX::XMMatrixRotationQuaternion(Quat);

		//DirectX::XMMatrixRotationX
	}

	void Rotation(float _x, float _y, float _z)
	{
		float x = DirectX::XMConvertToRadians(_x);
		float y = DirectX::XMConvertToRadians(_y);
		float z = DirectX::XMConvertToRadians(_z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		DirectX::XMVECTOR Quat =
			DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		m = DirectX::XMMatrixRotationQuaternion(Quat);
	}

	void RotationX(float _x)
	{
		float x = DirectX::XMConvertToRadians(_x);

		m = DirectX::XMMatrixRotationX(x);
	}

	void RotationY(float _y)
	{
		float y = DirectX::XMConvertToRadians(_y);

		m = DirectX::XMMatrixRotationY(y);
	}

	void RotationZ(float _z)
	{
		float z = DirectX::XMConvertToRadians(_z);

		m = DirectX::XMMatrixRotationZ(z);
	}

	void RotationAxis(const FVector3D& Axis, float _Angle)
	{
		float Angle = DirectX::XMConvertToRadians(_Angle);

		DirectX::XMVECTOR	_Axis =
			DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&Axis);

		m = DirectX::XMMatrixRotationAxis(_Axis, Angle);
	}

	void Translation(const FVector3D& _v)
	{
		m = DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
	}

	void Translation(float x, float y, float z)
	{
		m = DirectX::XMMatrixTranslation(x, y, z);
	}

	void Translation(const FVector2D& _v)
	{
		m = DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
	}

	void Translation(float x, float y)
	{
		m = DirectX::XMMatrixTranslation(x, y, 0.f);
	}

	static FMatrix StaticIdentity()
	{
		return DirectX::XMMatrixIdentity();
	}

	static FMatrix StaticTranspose(const FMatrix& _m)
	{
		return DirectX::XMMatrixTranspose(_m.m);
	}

	static FMatrix StaticInverse(const FMatrix& _m)
	{
		DirectX::XMVECTOR	det = DirectX::XMMatrixDeterminant(_m.m);
		return DirectX::XMMatrixInverse(&det, _m.m);
	}

	static FMatrix StaticScaling(const FVector3D& _v)
	{
		return DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
	}

	static FMatrix StaticScaling(float x, float y, float z)
	{
		return DirectX::XMMatrixScaling(x, y, z);
	}

	static FMatrix StaticScaling(const FVector2D& _v)
	{
		return DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
	}

	static FMatrix StaticScaling(float x, float y)
	{
		return DirectX::XMMatrixScaling(x, y, 1.f);
	}

	static FMatrix StaticRotation(const FVector3D& _v)
	{
		float x = DirectX::XMConvertToRadians(_v.x);
		float y = DirectX::XMConvertToRadians(_v.y);
		float z = DirectX::XMConvertToRadians(_v.z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		DirectX::XMVECTOR Quat =
			DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		return DirectX::XMMatrixRotationQuaternion(Quat);

		//DirectX::XMMatrixRotationX
	}

	static FMatrix StaticRotation(float _x, float _y, float _z)
	{
		float x = DirectX::XMConvertToRadians(_x);
		float y = DirectX::XMConvertToRadians(_y);
		float z = DirectX::XMConvertToRadians(_z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		DirectX::XMVECTOR Quat =
			DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		return DirectX::XMMatrixRotationQuaternion(Quat);
	}

	static FMatrix StaticRotationX(float _x)
	{
		float x = DirectX::XMConvertToRadians(_x);

		return DirectX::XMMatrixRotationX(x);
	}

	static FMatrix StaticRotationY(float _y)
	{
		float y = DirectX::XMConvertToRadians(_y);

		return DirectX::XMMatrixRotationY(y);
	}

	static FMatrix StaticRotationZ(float _z)
	{
		float z = DirectX::XMConvertToRadians(_z);

		return DirectX::XMMatrixRotationZ(z);
	}

	static FMatrix StaticRotationAxis(const FVector3D& Axis, float _Angle)
	{
		float Angle = DirectX::XMConvertToRadians(_Angle);

		DirectX::XMVECTOR	_Axis =
			DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&Axis);

		return DirectX::XMMatrixRotationAxis(_Axis, Angle);
	}

	static FMatrix StaticTranslation(const FVector3D& _v)
	{
		return DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
	}

	static FMatrix StaticTranslation(float x, float y, float z)
	{
		return DirectX::XMMatrixTranslation(x, y, z);
	}

	static FMatrix StaticTranslation(const FVector2D& _v)
	{
		return DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
	}

	static FMatrix StaticTranslation(float x, float y)
	{
		return DirectX::XMMatrixTranslation(x, y, 0.f);
	}
};




/*
4x4 * 4x4 

1x4       4x4
1 2 3 4 * 1 2 3 4 = 1 + 10 + 27 + 12 = 50  = 50, 30, 40, 78
		  5 6 7 8   2 + 12 + 0 + 16 = 30
		  9 0 1 2   3 + 14 + 3 + 20 = 40
		  3 4 5 6   4 + 16 + 6 + 24 = 50

1 2 3 4	  1 2 3 4   50, 30, 40, 78
5 6 7 8 * 5 6 7 8 = 122, 
9 0 1 2	  9 0 1 2
3 4 5 6	  3 4 5 6

1 0 0 0	  1 2 3 4   1 2 3 4
0 1 0 0 * 5 6 7 8 = 5 6 7 8
0 0 1 0	  9 0 1 2
0 0 0 1	  3 4 5 6

1 2 3 4   1 0 0 0   1 2 3 4
5 6 7 8 * 0 1 0 0 = 
9 0 1 2	  0 0 1 0
3 4 5 6	  0 0 0 1

A 행렬에 A행렬의 역행렬을 곱하면 항등(단위)행렬이 나온다.

전치행렬 : 행과 열을 바꿔준다.
1 2 3 4
5 6 7 8
9 0 1 2
3 4 5 6

1 5 9 3
2 6 0 4
3 7 1 5
4 8 2 6

월드행렬 구성요소
크기, 자전, 위치(이동), 공전, 부모
월드행렬 = 크기행렬 * 자전행렬 * 이동행렬 * 공전행렬 *
부모행렬



x 0 0 0
0 y 0 0
0 0 z 0
0 0 0 1

1, 1, 0, 1 * 100 0   0   0 = 100, 100, 0
			 0   100 0   0
			 0   0   100 0
			 0   0   0   1

-1, 1, 0, 1 * 100 0   0   0 = -100, 100, 0
 			  0   100 0   0 
			  0   0   100 0
			  0   0   0   1

cos -sin
sin cos

X축 회전
1 0   0    0
0 cos -sin 0
0 sin cos  0
0 0   0    1

Y축 회전
cos  0 sin 0
0    1 0   0
-sin 0 cos 0
0    0 0   1

Z축 회전
cos -sin 0 0
sin cos  0 0
0   0    1 0
0   0    0 1

최종 회전행렬 = X축회전행렬 * Y축회전행렬 * Z축회전행렬

100, 100, 0, 1 * 1 0   0    0 = 100, 0
				 0 cos -sin 0
				 0 sin cos  0
				 0 0   0    1

이동행렬
1 0 0 0
0 1 0 0
0 0 1 0
x y z 1

1, 1, 0, 1 * 1   0   0   0 = 201, 201, 0
			 0   1   0   0
			 0   0   1   0
			 200 200 0   1


PI = 180도를 의미한다.
라디안 * 180 / PI
각도 * PI / 180



*/