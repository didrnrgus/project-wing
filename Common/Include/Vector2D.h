#pragma once

#include <math.h>
#include "EngineMath.h"

struct FVector2D
{
	float	x = 0.f;
	float	y = 0.f;

#pragma region Construction

	FVector2D()
	{
	}

	FVector2D(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}

	FVector2D(const FVector2D& v) :
		x(v.x),
		y(v.y)
	{
	}

	FVector2D(FVector2D&& v)	:
		x(v.x),
		y(v.y)
	{
	}

#pragma endregion Construction

#pragma region Equal

	const FVector2D& operator = (const FVector2D& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	const FVector2D& operator = (float Value)
	{
		x = Value;
		y = Value;
		return *this;
	}

	const FVector2D& operator = (double Value)
	{
		x = (float)Value;
		y = (float)Value;
		return *this;
	}

	const FVector2D& operator = (int Value)
	{
		x = (float)Value;
		y = (float)Value;
		return *this;
	}

#pragma endregion Equal

#pragma region Plus
	// +
	FVector2D operator + (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	FVector2D operator + (float Value)	const
	{
		FVector2D	result;
		result.x = x + Value;
		result.y = y + Value;
		return result;
	}

	FVector2D operator + (double Value)	const
	{
		FVector2D	result;
		result.x = x + (float)Value;
		result.y = y + (float)Value;
		return result;
	}

	FVector2D operator + (int Value)	const
	{
		FVector2D	result;
		result.x = x + (float)Value;
		result.y = y + (float)Value;
		return result;
	}

	// +=
	const FVector2D& operator += (const FVector2D& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	const FVector2D& operator += (float Value)
	{
		x += Value;
		y += Value;
		return *this;
	}

	const FVector2D& operator += (double Value)
	{
		x += (float)Value;
		y += (float)Value;
		return *this;
	}

	const FVector2D& operator += (int Value)
	{
		x += (float)Value;
		y += (float)Value;
		return *this;
	}

	// ++
	const FVector2D& operator ++ ()
	{
		x += 1.f;
		y += 1.f;
		return *this;
	}

	const FVector2D& operator ++ (int)
	{
		x += 1.f;
		y += 1.f;
		return *this;
	}

#pragma endregion Plus

#pragma region Minus
	// -
	FVector2D operator - (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	FVector2D operator - (float Value)	const
	{
		FVector2D	result;
		result.x = x - Value;
		result.y = y - Value;
		return result;
	}

	FVector2D operator - (double Value)	const
	{
		FVector2D	result;
		result.x = x - (float)Value;
		result.y = y - (float)Value;
		return result;
	}

	FVector2D operator - (int Value)	const
	{
		FVector2D	result;
		result.x = x - (float)Value;
		result.y = y - (float)Value;
		return result;
	}

	// -=
	const FVector2D& operator -= (const FVector2D& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const FVector2D& operator -= (float Value)
	{
		x -= Value;
		y -= Value;
		return *this;
	}

	const FVector2D& operator -= (double Value)
	{
		x -= (float)Value;
		y -= (float)Value;
		return *this;
	}

	const FVector2D& operator -= (int Value)
	{
		x -= (float)Value;
		y -= (float)Value;
		return *this;
	}

	// --
	const FVector2D& operator -- ()
	{
		x -= 1.f;
		y -= 1.f;
		return *this;
	}

	const FVector2D& operator -- (int)
	{
		x -= 1.f;
		y -= 1.f;
		return *this;
	}

#pragma endregion Minus

#pragma region Multiply

	// *
	FVector2D operator * (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	FVector2D operator * (float Value)	const
	{
		FVector2D	result;
		result.x = x * Value;
		result.y = y * Value;
		return result;
	}

	FVector2D operator * (double Value)	const
	{
		FVector2D	result;
		result.x = x * (float)Value;
		result.y = y * (float)Value;
		return result;
	}

	FVector2D operator * (int Value)	const
	{
		FVector2D	result;
		result.x = x * (float)Value;
		result.y = y * (float)Value;
		return result;
	}

	// *=
	const FVector2D& operator *= (const FVector2D& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const FVector2D& operator *= (float Value)
	{
		x *= Value;
		y *= Value;
		return *this;
	}

	const FVector2D& operator *= (double Value)
	{
		x *= (float)Value;
		y *= (float)Value;
		return *this;
	}

	const FVector2D& operator *= (int Value)
	{
		x *= (float)Value;
		y *= (float)Value;
		return *this;
	}

#pragma endregion Multiply

#pragma region Divide

	// /
	FVector2D operator / (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	FVector2D operator / (float Value)	const
	{
		FVector2D	result;
		result.x = x / Value;
		result.y = y / Value;
		return result;
	}

	FVector2D operator / (double Value)	const
	{
		FVector2D	result;
		result.x = x / (float)Value;
		result.y = y / (float)Value;
		return result;
	}

	FVector2D operator / (int Value)	const
	{
		FVector2D	result;
		result.x = x / (float)Value;
		result.y = y / (float)Value;
		return result;
	}

	// /=
	const FVector2D& operator /= (const FVector2D& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const FVector2D& operator /= (float Value)
	{
		x /= Value;
		y /= Value;
		return *this;
	}

	const FVector2D& operator /= (double Value)
	{
		x /= (float)Value;
		y /= (float)Value;
		return *this;
	}

	const FVector2D& operator /= (int Value)
	{
		x /= (float)Value;
		y /= (float)Value;
		return *this;
	}

#pragma endregion Divide

#pragma region Function

	float Length()	const
	{
		// sqrtf : 루트값 구해주는 기능.
		return sqrtf(x * x + y * y);
	}

	void Normalize()
	{
		float	Size = Length();

		if (Size == 0.f)
			return;

		x /= Size;
		y /= Size;
	}


	static FVector2D Normalize(const FVector2D& v)
	{
		FVector2D	result;

		float	Size = v.Length();

		if (Size == 0.f)
			return result;

		result.x = v.x / Size;
		result.y = v.y / Size;
		return result;
	}

	float Dot(const FVector2D& v)	const
	{
		return x * v.x + y * v.y;
	}

	float Distance(const FVector2D& v)	const
	{
		FVector2D v1 = *this - v;

		return v1.Length();
	}

	float GetAngle(const FVector2D& v)	const
	{
		FVector2D	v1 = *this;
		FVector2D	v2 = v;

		v1.Normalize();
		v2.Normalize();

		float Angle = v1.Dot(v2);

		Angle = DirectX::XMConvertToDegrees(acosf(Angle));

		return Angle;
	}

	static float GetAngle(const FVector2D& v1, const FVector2D& v2)
	{
		return 0.f;
	}

#pragma endregion Function
};
