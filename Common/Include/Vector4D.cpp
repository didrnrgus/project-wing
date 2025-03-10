
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
