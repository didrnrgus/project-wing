#pragma once

#include "GameInfo.h"

class CGameManager
{
private:
	static bool	mLoop;
	HINSTANCE	mhInst = 0;
	HWND		mhWnd = 0;
	HDC			mhDC = 0;
	TCHAR		mClassName[256] = {};
	TCHAR		mTitleName[256] = {};

	float		mClearColor[4] = {};

public:
	HINSTANCE GetWindowInstance() const { return mhInst; }
	HWND GetWindowHandle() const { return mhWnd; }
	void ExitGame() { DestroyWindow(mhWnd); }

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Input(float DeltaTime);
	bool Update(float DeltaTime);
	void Collision(float DeltaTime);
	void Render(float DeltaTime);

private:
	void RegisterWindowClass();
	bool Create();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void OpenIPInputPopup(HWND hWnd);
	static void OpenNicknameInputPopup(HWND hWnd);
	void UpdateTitle();
	static LRESULT CALLBACK PopupProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void UpdateMenu(HWND hWnd);

	DECLARE_SINGLE(CGameManager)
};

