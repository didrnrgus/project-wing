
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

std::wstring FormatWithCommaManual(int value)
{
	std::wstring number = std::to_wstring(value);
	std::wstring result;
	int len = static_cast<int>(number.length());

	for (int i = 0; i < len; ++i) {
		result += number[i];
		int remaining = len - i - 1;
		if (remaining > 0 && remaining % 3 == 0) {
			result += L',';
		}
	}

	return result;
}

std::wstring ToUpperWString(const wchar_t* _str)
{
	std::wstring str(_str);
	std::transform(str.begin(), str.end(), str.begin()
		, [](wchar_t ch)
		{
			return std::towupper(ch);
		});

	return str;
}

std::wstring ToUpperWString(std::string _str)
{
	std::wstring str(_str.begin(), _str.end());
	std::transform(str.begin(), str.end(), str.begin()
		, [](wchar_t ch)
		{
			return std::towupper(ch);
		});

	return str;
}
