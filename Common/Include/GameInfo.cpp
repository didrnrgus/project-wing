
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

void ConvertWStringAndCheckTextOverLength(const std::string& _inString, std::wstring& _outString, const float _maxScore)
{
	float score = 0;
	int cutIndex = 0;

	auto CharScore = [](char c) -> int
		{
			// 얇은 문자
			if (c == '1' || c == 'l' || c == 'I' || c == 'i')
				return 1.0f;
			// 넓은 문자 (대문자 알파벳)
			if (c >= 'A' && c <= 'Z')
				return 2.3f;
			// 기본 중간 너비 문자
			return 2.0f;
		};

	for (int i = 0; i < _inString.size(); ++i)
	{
		score += CharScore(_inString[i]);
		if (score > _maxScore) break;
		cutIndex = i + 1;
	}

	bool isOver = (score > _maxScore);

	if (isOver)
	{
		std::string cutStr = _inString.substr(0, cutIndex);
		_outString = std::wstring(cutStr.begin(), cutStr.end()) + L"...";
	}
	else
	{
		_outString = std::wstring(_inString.begin(), _inString.end());
	}

	return;
}