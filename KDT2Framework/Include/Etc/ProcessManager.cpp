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

	// 1. Job Object 생성
	HANDLE hJob = CreateJobObject(NULL, NULL);

	// 2. 종료 시 자식도 죽도록 설정
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobInfo = { 0 };
	jobInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jobInfo, sizeof(jobInfo));

	STARTUPINFO si = {};
	si.cb = sizeof(si);

	// CreateProcess는 문자열을 수정할 수 있어야 하므로 복사 필요
	wchar_t cmdLine[MAX_PATH];
	wcscpy_s(cmdLine, exePath.c_str());

	// 3. 자식 프로세스 생성
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
	auto createResult = CreateProcess(
		NULL,       // 실행할 파일 이름 (명령어 포함이면 NULL)
		cmdLine,    // 커맨드라인 문자열
		NULL,       // 프로세스 보안 속성
		NULL,       // 스레드 보안 속성
		TRUE,       // 핸들 상속 여부 (필요하다면 TRUE)
#ifdef _DEBUG
		CREATE_NEW_CONSOLE, // 새로운 콘솔 창 열기
#else
		CREATE_NO_WINDOW, // 콘솔 없이 실행
#endif // _DEBUG
		NULL,       // 환경 변수
		NULL,       // 현재 디렉토리
		&si,        // STARTUPINFO
		&pi         // PROCESS_INFORMATION
	);

	if (createResult)
	{
		running = true;
		CLog::PrintLog("CProcessManager::LaunchProcess Process started!");
	}
	else
	{
		CLog::PrintLog("CProcessManager::LaunchProcess Failed to start process. Error code: ");
		CLog::PrintLog(std::to_string(GetLastError()));
		return false;
	}

	// 4. 자식 프로세스를 Job Object에 연결
	AssignProcessToJobObject(hJob, pi.hProcess);

	return true;
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
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
