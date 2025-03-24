#pragma once
#include "ProcessManager.h"

DEFINITION_SINGLE(CProcessManager);

CProcessManager::CProcessManager()
{

}

CProcessManager::~CProcessManager()
{
	Terminate();
}

bool CProcessManager::LaunchProcess(const std::wstring& exePath)
{
	if (running)
	{
		Terminate();
	}

	STARTUPINFO si{};
	si.cb = sizeof(si);

	// CreateProcess는 문자열을 수정할 수 있어야 하므로 복사 필요
	wchar_t cmdLine[MAX_PATH];
	wcscpy_s(cmdLine, exePath.c_str());

	/*
	CreateProcessW(
    _In_opt_ LPCWSTR lpApplicationName,
    _Inout_opt_ LPWSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCWSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOW lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
    );
	*/
	auto createResult = CreateProcess(NULL
		, cmdLine
		, NULL
		, NULL
		, TRUE
		, 0
		, NULL
		, NULL
		, &si
		, &pi);

	if (createResult)
	{
		running = true;
		CLog::PrintLog("CProcessManager::LaunchProcess Process started!");
		return true;
	}
	else
	{
		CLog::PrintLog("CProcessManager::LaunchProcess Failed to start process. Error code: ");
		CLog::PrintLog(std::to_string(GetLastError()));
		return false;
	}
}

void CProcessManager::WaitForExit()
{
	if (running)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CLog::PrintLog("CProcessManager::WaitForExit Process exited.");
		running = false;
	}
}

void CProcessManager::Terminate()
{
	// 안전한 종료
	if (running)
	{
		TerminateProcess(pi.hProcess, 0);
		CLog::PrintLog("CProcessManager::Terminate Process terminated manually.");
		running = false;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
