#include "GameManager.h"
#include "resource.h"
#include "Share/Timer.h"
#include "Device.h"
#include "Asset/AssetManager.h"
#include "Shader/ShaderManager.h"
#include "Asset/Mesh/MeshManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/Shader.h"
#include "Shader/TransformCBuffer.h"
#include "Scene/SceneManager.h"
#include "ProfileManager.h"
#include "Render/RenderManager.h"
#include "Render/RenderStateManager.h"
#include "Render/RenderState.h"
#include "Asset/Sound/SoundManager.h"
#include "UI/Widget.h"
#include "Etc/DataStorageManager.h"
#include "Etc/CURL.h"
#include "Etc/JsonController.h"
#include "Etc/NotionDBController.h"

#define ID_MENU_OPTION1  1000  // 옵션1 메뉴 ID
#define ID_MENU_CHECKBOX 1001  // 체크박스 ID
#define ID_MENU_OPTION2  1002  // 옵션2 메뉴 ID
#define ID_MENU_TEXTBOX  1003  // 입력 필드 띄우는 항목 ID
#define ID_EDITBOX       2001  // 팝업 내부의 입력 필드 ID
#define ID_OK_BUTTON     2002  // 팝업 내 OK 버튼 ID
#define ID_CANCEL_BUTTON 2003  // 팝업 내 Cancel 버튼 ID

HMENU hMainMenu, hOption2Menu = NULL, hOption1Menu = NULL;
BOOL option2Visible = FALSE;  // 옵션2 표시 여부

std::string inputText = "127.0.0.1";  // 입력된 텍스트를 저장할 변수

TCHAR   gRootPath[MAX_PATH];
char   gRootPathMultibyte[MAX_PATH];

DEFINITION_SINGLE(CGameManager)

bool CGameManager::mLoop = true;

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{
    CSceneManager::DestroyInst();

    CRenderManager::DestroyInst();

    CProfileManager::DestroyInst();

    CAssetManager::DestroyInst();

    CShaderManager::DestroyInst();

    CDevice::DestroyInst();

    ReleaseDC(mhWnd, mhDC);

    CDataStorageManager::DestroyInst();
    CCURL::DestroyInst();
    CJsonController::DestroyInst();
    CNotionDBController::DestroyInst();

    CLog::Destroy();
}

bool CGameManager::Init(HINSTANCE hInst)
{
    srand(GetTickCount());
    rand();

	mhInst = hInst;

	lstrcpy(mClassName, TEXT("KDT2Framework"));
	lstrcpy(mTitleName, TEXT("KDT2Framework"));

    RegisterWindowClass();

    if (!Create())
        return false;

    if (!CLog::Init())
        return false;


    // 인자로 들어간 윈도우에 출력할 수 있는 DC가 만들어진다.
    mhDC = GetDC(mhWnd);

    // 디바이스 초기화
    if (!CDevice::GetInst()->Init(mhWnd, 1280, 720, true))
        return false;

    // Shader 관리자 초기화
    if (!CShaderManager::GetInst()->Init())
        return false;

    // 애셋 관리자 초기화
    if (!CAssetManager::GetInst()->Init())
        return false;

    // Profile 관리자 초기화
    if (!CProfileManager::GetInst()->Init())
        return false;

    // Render 관리자 초기화
    if (!CRenderManager::GetInst()->Init())
        return false;

    // 타이머 초기화
    CTimer::Init();

    // 장면관리자 초기화
    if (!CSceneManager::GetInst()->Init())
        return false;

    FResolution RS = CDevice::GetInst()->GetResolution();

    CWidget::CreateUIProjection((float)RS.Width, (float)RS.Height,
        1000.f);

	return true;
}

int CGameManager::Run()
{
    MSG msg;

    while (mLoop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // 키보드 입력 메세지가 발생할 경우 동작한다.
            // WM_KEYDOWN, WM_KEYUP 등 메세지가 발생하면 문자일 경우 WM_CHAR 메세지를 하나더
            // 만들어주는 역할을 한다.
            TranslateMessage(&msg);

            // 메세지를 WndProc로 전달해준다.
            DispatchMessage(&msg);
        }

        // 윈도우 데드타임일 경우 동작한다.(메세지 큐에 메세지가 없는 경우)
        else
        {
            Logic();
        }
    }

	return (int)msg.wParam;
}

void CGameManager::Logic()
{
    float DeltaTime = CTimer::Update();

    Input(DeltaTime);

    if (Update(DeltaTime))
        return;

    Collision(DeltaTime);

    Render(DeltaTime);

    CAssetManager::GetInst()->GetSoundManager()->Update();
}

void CGameManager::Input(float DeltaTime)
{
    CSceneManager::GetInst()->Input(DeltaTime);
}

bool CGameManager::Update(float DeltaTime)
{
    //CLog::PrintLog("Update");

    static bool Push = false;

    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
    {
        Push = true;
    }

    else if (Push)
    {
        Push = false;
        CLog::SaveLog();
    }

    CRenderManager::GetInst()->Update(DeltaTime);

    return CSceneManager::GetInst()->Update(DeltaTime);
}

void CGameManager::Collision(float DeltaTime)
{
    CSceneManager::GetInst()->Collision(DeltaTime);
}

void CGameManager::Render(float DeltaTime)
{
    CDevice::GetInst()->ClearBackBuffer(mClearColor);
    CDevice::GetInst()->ClearDepthStencil(1.f, 0);
    CDevice::GetInst()->SetTarget();

    //CRenderState* AlphaBlend = CRenderManager::GetInst()->GetStateManager()->FindState("AlphaBlend");

    //AlphaBlend->SetState();

    // 출력이 아닌 Component들의 Render 관련 함수를
    // 호출하기 위한 용도.
    CSceneManager::GetInst()->Render();

    CRenderManager::GetInst()->Render();

    CSceneManager::GetInst()->EndFrame();

    //AlphaBlend->ResetState();

    CDevice::GetInst()->Render();
}

void CGameManager::RegisterWindowClass()
{
    WNDCLASSEXW wcex;

    // 윈도우클래스 구조체의 크기를 나타낸다. 반드시 지정되어야 한다.
    wcex.cbSize = sizeof(WNDCLASSEX);

    // 화면에 출력가능한 영역을 클라이언트 영역이라고 한다.
    // 클라이언트 영역의 크기(가로, 세로)가 변경될 시 전부 다시 그려주도록 한다.
    wcex.style = CS_HREDRAW | CS_VREDRAW;

    // 메세지큐에서 얻어온 메세지를 인자로 넣어서 호출해줄 함수의 주소를 넘겨준다.
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    // 운영체제에서 부여해준 HINSTANCE를 전달한다.
    wcex.hInstance = mhInst;

    // 실행파일 아이콘을 지정한다.
    wcex.hIcon = LoadIcon(mhInst, MAKEINTRESOURCE(IDI_ICON1));

    // 윈도우 창에서의 커서 모양을 나타낸다.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

    // 클라이언트 영역의 색상을 지정한다.
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // 윈도우 메뉴를 지정한다.
    // 0을 대입하면 메뉴를 없앤다.
    wcex.lpszMenuName = 0;

    // 등록할 윈도우클래스의 이름을 지정한다.
    wcex.lpszClassName = mClassName;

    // 윈도우창 좌측 상단의 작은 아이콘을 지정한다.
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    // 위에서 설정한 윈도우클래스를 등록한다.
    RegisterClassExW(&wcex);
}

bool CGameManager::Create()
{
    // CreateWindow : 윈도우 창을 생성해주는 함수이다.
    // WinAPI에서 함수명뒤에 W가 붙으면 유니코드, A가 붙으면 멀티바이트이다.
    // 1번인자 : 윈도우 클래스 이름을 지정한다.
    // 2번인자 : 윈도우 타이틀바에 출력할 이름을 지정한다.
    // 3번인자 : 윈도우 창의 모양을 결정한다.
    // 4번인자 : 화면에서 윈도우가 시작할 X지점을 지정한다.
    // 5번인자 : 화면에서 윈도우가 시작할 Y지점을 지정한다.
    // 6번인자 : 윈도우 창의 가로 크기를 지정한다.
    // 7번인자 : 윈도우 창의 세로 크기를 지정한다.
    // 8번인자 : 부모윈도우가 있을 경우 부모윈도우의 핸들을 지정한다.
    // 9번인자 : 메뉴 핸들을 전달한다.
    // 10번인자 : 윈도우 인스턴스를 전달한다. WinMain에서 전달은 값으로 전달해야 한다.
    // 11번인자 : 창 생성 데이터를 지정한다. WM_CREATE는 윈도우 생성시 발생하는 메세지인데
    // 이 메세지가 발생하면 WndProc 함수의 lParam에 이 값이 전달된다.
    // 이렇게 윈도우를 생성하면 윈도우 핸들을 만들어준다.
    // 잘못된 생성일 경우 0을 반환한다.
    mhWnd = CreateWindowW(mClassName, mTitleName, WS_OVERLAPPEDWINDOW,
        100, 100, 1280, 720, nullptr, nullptr, mhInst, nullptr);

    if (!mhWnd)
    {
        return false;
    }

    // 윈도우 클라이언트 영역의 크기를 원하는 크기로 지정한다.
    // 위에서 지정한 윈도우 크기는 타이틀바 등의 크기가 모두 합쳐진 크기로 지정된다.
    RECT    WindowRC = { 0, 0, 1280, 720 };

    // 클라이언트 영역이 1280, 720이 되기 위해 필요한 윈도우 전체 크기를 얻어온다.
    // (ThickFrame, Menu, TitleBar 등이 포함된 전체크기)
    AdjustWindowRect(&WindowRC, WS_OVERLAPPEDWINDOW, FALSE);

    SetWindowPos(mhWnd, HWND_TOPMOST, 100, 100, WindowRC.right - WindowRC.left,
        WindowRC.bottom - WindowRC.top, SWP_NOMOVE | SWP_NOZORDER);

    // 위에서 윈도우 창을 만들었다면 ShowWindow 함수를 이용해서 창을 보여줄지 숨길지를
    // 결정한다.
    ShowWindow(mhWnd, SW_SHOW);

    // 클라이언트 영역을 강제로 다시 그리게 요청해주는 함수이다.
    // 처음 생성시나 특정상황에 창을 새로고침 해야 할 경우 사용한다.
    UpdateWindow(mhWnd);

    return true;
}

LRESULT CALLBACK CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        hMainMenu = CreateMenu();
        hOption1Menu = CreatePopupMenu();
        AppendMenu(hOption1Menu, MF_STRING, ID_MENU_CHECKBOX, TEXT("남의 방 들어가기"));
        AppendMenu(hMainMenu, MF_POPUP, (UINT_PTR)hOption1Menu, TEXT("멀티플레이"));
        SetMenu(hWnd, hMainMenu);
    } break;

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == ID_MENU_CHECKBOX)
        {
            option2Visible = !option2Visible;
            UpdateMenu(hWnd);

            if (!option2Visible)
            {
                inputText = "127.0.0.1";
            }
        }
        else if (LOWORD(wParam) == ID_MENU_TEXTBOX)
        {
            OpenPopup(hWnd);  // 팝업 열기
        }
    } break;

    case WM_DESTROY:
        mLoop = false;
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CGameManager::OpenPopup(HWND hWnd)
{
    POINT pt;
    GetCursorPos(&pt);  // 마우스 포인터의 현재 위치를 얻어옴

    // 팝업 창 생성 (마우스 위치에서 열리도록)
    HWND hPopup = CreateWindowEx(
        WS_EX_TOPMOST,  // 항상 위에 나타나도록 설정
        TEXT("STATIC"), TEXT(""), WS_POPUP | WS_VISIBLE | WS_BORDER,
        pt.x, pt.y, 300, 150,  // 마우스 포인터 위치 (pt.x, pt.y)에서 창을 열기
        hWnd, NULL, GetModuleHandle(NULL), NULL);

    // 입력 필드 추가
    HWND hEditBox = CreateWindowExA(0, "EDIT", inputText.c_str(),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        50, 30, 200, 20, hPopup, (HMENU)ID_EDITBOX, GetModuleHandle(NULL), NULL);

    // 확인 버튼 추가
    CreateWindowExA(0, "BUTTON", "확인",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        50, 60, 50, 30, hPopup, (HMENU)ID_OK_BUTTON, GetModuleHandle(NULL), NULL);

    // 취소 버튼 추가
    CreateWindowExA(0, "BUTTON", "취소",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        150, 60, 50, 30, hPopup, (HMENU)ID_CANCEL_BUTTON, GetModuleHandle(NULL), NULL);

    // 팝업 창을 닫는 기본 메시지 처리
    SetWindowLongPtr(hPopup, GWLP_WNDPROC, (LONG_PTR)PopupProc);
}

LRESULT CALLBACK CGameManager::PopupProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_OK_BUTTON)  // 확인 버튼
        {
            HWND hEditBox = GetDlgItem(hWnd, ID_EDITBOX);
            char buffer[256];
            GetWindowTextA(hEditBox, buffer, sizeof(buffer));
            inputText = std::string(buffer); // 입력된 값 저장
            DestroyWindow(hWnd);  // 팝업 닫기
            return TRUE;
        }
        else if (LOWORD(wParam) == ID_CANCEL_BUTTON)  // 취소 버튼
        {
            DestroyWindow(hWnd);  // 팝업 닫기
            return TRUE;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);  // 팝업 닫기
        return TRUE;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void CGameManager::UpdateMenu(HWND hWnd)
{
    ModifyMenu(hOption1Menu, ID_MENU_CHECKBOX, MF_BYCOMMAND | MF_STRING,
        ID_MENU_CHECKBOX, option2Visible ? TEXT("멀티에서 내가 방장") : TEXT("남의 방 들어가기"));

    if (option2Visible)
    {
        if (!hOption2Menu)
        {
            hOption2Menu = CreatePopupMenu();
            AppendMenu(hOption2Menu, MF_STRING, ID_MENU_TEXTBOX, TEXT("방장 IP주소 입력하기"));
            AppendMenu(hMainMenu, MF_POPUP, (UINT_PTR)hOption2Menu, TEXT("방장 IP주소 입력하셨나요?"));
        }
    }
    else
    {
        if (hOption2Menu)
        {
            // 옵션2 메뉴가 존재하면 제거
            RemoveMenu(hMainMenu, (UINT_PTR)hOption2Menu, MF_POPUP);
            DestroyMenu(hOption2Menu);
            hOption2Menu = NULL;
        }
    }
    DrawMenuBar(hWnd);
}
