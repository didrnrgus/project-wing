#pragma once

#include "../GameInfo.h"

enum class ELogPrintType
{
	Console,
	VisualOutput,
	All
};

class CLog
{
private:
	static std::vector<std::string>	mvecLog;
	static HANDLE	hConsole;

public:
	static bool Init();
	static void Destroy();
	static void PrintLog(const std::string& Text,
		ELogPrintType PrintType = ELogPrintType::All);
	static void SaveLog();
};

