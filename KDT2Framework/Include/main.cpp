
#include "GameManager.h"
#include "Share/SharedPtr.h"
#include "Share/Object.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    //int Size = sizeof(Test);

    /*CObject* Obj = nullptr;

    CSharedPtr  ptr = Obj;

    CObject* Obj1 = ptr;*/

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(874);

    if (!CGameManager::GetInst()->Init(hInstance))
    {
        CGameManager::DestroyInst();
        return 0;
    }

    int Ret = CGameManager::GetInst()->Run();

    CGameManager::DestroyInst();

    return Ret;
}