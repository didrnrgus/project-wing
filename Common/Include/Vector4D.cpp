#pragma once

#include "Vector4D.h"
#include "Etc/DataStorageManager.h"

FVector4D FVector4D::Transparent = { 0.f, 0.f, 0.f, 0.0f };
FVector4D FVector4D::Black = { 0.f, 0.f, 0.f, 1.f };
FVector4D FVector4D::White = { 1.f, 1.f, 1.f, 1.f };
FVector4D FVector4D::Green = { 0.f, 1.f, 0.f, 1.f };
FVector4D FVector4D::Gray = { 0.5f, 0.5f, 0.5f, 1.f };

FVector4D FVector4D::GetColorFromString(std::string _colorName)
{
	auto _info = CDataStorageManager::GetInst()->GetColorInfoFromName(_colorName);

	FVector4D _color(_info.R, _info.G, _info.B, _info.A);
	return _color;
}