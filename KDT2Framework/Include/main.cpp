
#include "GameManager.h"
#include "Share/SharedPtr.h"
#include "Share/Object.h"
#include <exception>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	int Ret = 0;

	try
	{
		std::cout << "프로그램 실행 중..." << std::endl;

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		if (!CGameManager::GetInst()->Init(hInstance))
		{
			CGameManager::DestroyInst();
			return 0;
		}

		Ret = CGameManager::GetInst()->Run();

		CGameManager::DestroyInst();

		//throw std::runtime_error("테스트 에러 발생");  // 강제 예외 (테스트용)
	}
	catch (const std::exception& e) {
		std::cerr << "예외 발생: " << e.what() << std::endl;
		//system("pause");  // 창이 자동 종료되지 않도록 함
	}

	return Ret;
}