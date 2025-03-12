#pragma once

#include "Vector4D.h"


#ifdef _DEBUG
	FVector4D FVector4D::Transparent = { 1.f, 1.f, 1.f, 0.1f };
#else
	FVector4D FVector4D::Transparent = { 1.f, 1.f, 1.f, 0.0f };
#endif // !_DEBUG

FVector4D FVector4D::Black = { 0.f, 0.f, 0.f, 1.f };
FVector4D FVector4D::White = { 1.f, 1.f, 1.f, 1.f };
FVector4D FVector4D::Red = { 1.f, 0.f, 0.f, 1.f };
FVector4D FVector4D::Blue = { 0.f, 0.f, 1.f, 1.f };
FVector4D FVector4D::Green = { 0.f, 1.f, 0.f, 1.f };
FVector4D FVector4D::Yellow = { 1.f, 1.f, 0.f, 1.f };
FVector4D FVector4D::Cyan = { 0.f, 1.f, 1.f, 1.f };
FVector4D FVector4D::Pink = { 1.f, 0.753f, 0.796f, 1.f };

FVector4D FVector4D::GetColorFromString(std::string colorName)
{
	if (strcmp(colorName.c_str(), "Black") == 0)
	{
		return FVector4D::Black;
	}
	else if (strcmp(colorName.c_str(), "White") == 0)
	{
		return FVector4D::White;
	}
	else if (strcmp(colorName.c_str(), "Red") == 0)
	{
		return FVector4D::Red;
	}
	else if (strcmp(colorName.c_str(), "Blue") == 0)
	{
		return FVector4D::Blue;
	}
	else if (strcmp(colorName.c_str(), "Green") == 0)
	{
		return FVector4D::Green;
	}
	else if (strcmp(colorName.c_str(), "Yellow") == 0)
	{
		return FVector4D::Yellow;
	}
	else if (strcmp(colorName.c_str(), "Cyan") == 0)
	{
		return FVector4D::Cyan;
	}
	else if (strcmp(colorName.c_str(), "Pink") == 0)
	{
		return FVector4D::Pink;
	}

	return FVector4D::White;
}