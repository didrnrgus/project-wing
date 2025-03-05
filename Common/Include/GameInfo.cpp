
#include "GameInfo.h"

int Clamp(int Value, int Min, int Max)
{
	if (Value < Min)
		Value = Min;

	else if (Value > Max)
		Value = Max;

	return Value;
}

float Clamp(float Value, float Min, float Max)
{
	if (Value < Min)
		Value = Min;

	else if (Value > Max)
		Value = Max;

	return Value;
}
