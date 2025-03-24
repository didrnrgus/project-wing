#pragma once
#include "GameInfo.h"

class CProcessManager
{
private:
	PROCESS_INFORMATION pi{};
	bool running = false;

public:
	bool LaunchProcess(const std::wstring& exePath);
	void WaitForExit();
	void Terminate();

	DECLARE_SINGLE(CProcessManager);
};

