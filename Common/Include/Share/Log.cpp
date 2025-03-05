#include "Log.h"

std::vector<std::string> CLog::mvecLog;
HANDLE CLog::hConsole;

bool CLog::Init()
{
#ifdef _DEBUG
	// 콘솔창을 열어준다.
	AllocConsole();

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#else

#endif // _DEBUG

	mvecLog.reserve(10000);


	return true;
}

void CLog::Destroy()
{
#ifdef _DEBUG
	// 콘솔창을 열어준다.
	FreeConsole();
#else

#endif // _DEBUG
}

void CLog::PrintLog(const std::string& Text, 
	ELogPrintType PrintType)
{
	SYSTEMTIME	sysTime;
	
	// UTC 표준시간
	//GetSystemTime(&sysTime);

	// 지역 시간
	GetLocalTime(&sysTime);

	char	TimeText[128] = {};

	sprintf_s(TimeText, "[%d.%d.%d %d:%d:%d:%d]",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
		sysTime.wMilliseconds);

	std::string	OutputText = TimeText;

	OutputText += " : ";
	OutputText += Text;
	OutputText += "\n";

	mvecLog.emplace_back(OutputText);

	switch (PrintType)
	{
	case ELogPrintType::Console:
	{
		DWORD	Number = 0;
		WriteConsoleA(hConsole, OutputText.c_str(),
			(DWORD)OutputText.length(), &Number, nullptr);
	}
		break;
	case ELogPrintType::VisualOutput:
		OutputDebugStringA(OutputText.c_str());
		break;
	case ELogPrintType::All:
	{
		DWORD	Number = 0;
		WriteConsoleA(hConsole, OutputText.c_str(),
			(DWORD)OutputText.length(), &Number, nullptr);
		OutputDebugStringA(OutputText.c_str());
	}
		break;
	}
}

void CLog::SaveLog()
{
	SYSTEMTIME	sysTime;

	// 지역 시간
	GetLocalTime(&sysTime);

	char	TimeText[128] = {};

	sprintf_s(TimeText, "../Bin/Log/%d_%d_%d_%d_%d_%d.txt",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	FILE* File = nullptr;

	fopen_s(&File, TimeText, "wt");

	if (!File)
		return;

	size_t	Size = mvecLog.size();

	for (size_t i = 0; i < Size; ++i)
	{
		fputs(mvecLog[i].c_str(), File);
	}

	fclose(File);
}
