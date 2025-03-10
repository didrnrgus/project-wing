#include "Vector2D.h"

FVector2D FVector2D::Zero;
FVector2D FVector2D::One = { 1.f, 1.f};
FVector2D FVector2D::Axis[EAxis::End] =
{
	{1.f, 0.f},
	{0.f, 1.f}
};