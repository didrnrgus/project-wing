#include "Input.h"
#include "../GameManager.h"
#include "../Device.h"
#include "Scene.h"
#include "CameraManager.h"

CInput::CInput()
{
}

CInput::~CInput()
{
	auto	iter = mKeyStateMap.begin();
	auto	iterEnd = mKeyStateMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	auto	iter1 = mBindKeyMap.begin();
	auto	iter1End = mBindKeyMap.end();

	for (; iter1 != iter1End; ++iter1)
	{
		SAFE_DELETE(iter1->second);
	}

    if (mkeyboard)
    {
        mkeyboard->Unacquire();
        SAFE_RELEASE(mkeyboard);
    }

    if (mMouse)
    {
        mMouse->Unacquire();
        SAFE_RELEASE(mMouse);
    }

    SAFE_RELEASE(mInput);
}

bool CInput::InitInput()
{
    // 키보드 장치 생성
    if (FAILED(mInput->CreateDevice(GUID_SysKeyboard, &mkeyboard, nullptr)))
        return false;

    // 키보드 데이터 형식 설정
    if (FAILED(mkeyboard->SetDataFormat(&c_dfDIKeyboard)))
        return false;

    // DISCL_FOREGROUND : 활성화 시에만 키를 입력받게 한다.
    // DISCL_BACKGROUND : 백그라운드에서도 사용할 수 있게 설정한다.
    // 활성화 상태가 아니어도 키 입력이 가능하다.
    // DISCL_EXCLUSIVE : 입력 디바이스를 독점모드로 설정한다.
    // DISCL_NONEXCLUSIVE : 입력 디바이스를 비독점모드로 설정한다.

    if (CDevice::GetInst()->GetWindowMode())
    {
        if (FAILED(mkeyboard->SetCooperativeLevel(mhWnd,
            DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
            return false;
    }

    else
    {
        if (FAILED(mkeyboard->SetCooperativeLevel(mhWnd,
            DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
            return false;
    }

    // 키보드 장치를 획득한다.
    if (FAILED(mkeyboard->Acquire()))
        return false;


    // 마우스 장치 생성
    if (FAILED(mInput->CreateDevice(GUID_SysMouse, &mMouse, nullptr)))
        return false;

    // 마우스 데이터 형식 설정
    if (FAILED(mMouse->SetDataFormat(&c_dfDIMouse)))
        return false;

    if (CDevice::GetInst()->GetWindowMode())
    {
        if (FAILED(mMouse->SetCooperativeLevel(mhWnd,
            DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
            return false;
    }

    else
    {
        if (FAILED(mMouse->SetCooperativeLevel(mhWnd,
            DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
            return false;
    }

    // 마우스 장치를 획득한다.
    if (FAILED(mMouse->Acquire()))
        return false;

    return true;
}

bool CInput::UpdateKeyboard()
{
    if (!mkeyboard)
        return false;

    HRESULT result = mkeyboard->GetDeviceState(256, (LPVOID)&mKeyState);

    if (FAILED(result))
    {
        if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
            mkeyboard->Acquire();
    }

    return true;
}

bool CInput::UpdateMouse()
{
    if (!mMouse)
        return false;

    // MouseState는 x, y에는 마우스 좌표, z에는 휠값이 들어온다.
    HRESULT result = mMouse->GetDeviceState(sizeof(mMouseState),
        (LPVOID)&mMouseState);

    if (FAILED(result))
    {
        if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
            mMouse->Acquire();
    }

    return true;
}

FKeyState* CInput::FindKeyState(unsigned char Key)
{
    auto    iter = mKeyStateMap.find(Key);

    if (iter == mKeyStateMap.end())
        return nullptr;

    return iter->second;
}

FBindKey* CInput::FindBindKey(const std::string& Name)
{
    auto    iter = mBindKeyMap.find(Name);

    if (iter == mBindKeyMap.end())
        return nullptr;

    return iter->second;
}

void CInput::AddBindKey(const std::string& Name, unsigned char Key)
{
    // 중복된 이름이 있는지 판단한다.
    if (FindBindKey(Name))
        return;

    FBindKey* NewKey = new FBindKey;

    FKeyState* State = FindKeyState(Key);

	// 키가 등록된게 없다면 생성해서 등록해준다.
	if (!State)
	{
		State = new FKeyState;

		State->Key = ConvertKey(Key);

		mKeyStateMap.insert(std::make_pair(Key, State));
	}

	NewKey->Key = State;

	mBindKeyMap.insert(std::make_pair(Name, NewKey));
}

void CInput::ChangeKeyCtrl(const std::string& Name, bool Ctrl)
{
	FBindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Ctrl = Ctrl;
}

void CInput::ChangeKeyAlt(const std::string& Name, bool Alt)
{
	FBindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Alt = Alt;
}

void CInput::ChangeKeyShift(const std::string& Name, bool Shift)
{
	FBindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Shift = Shift;
}

bool CInput::Init()
{
    mhInst = CGameManager::GetInst()->GetWindowInstance();
    mhWnd = CGameManager::GetInst()->GetWindowHandle();

    if (FAILED(DirectInput8Create(mhInst, DIRECTINPUT_VERSION,
        IID_IDirectInput8, (void**)&mInput, nullptr)))
        mInputType = EInputSystem_Type::Window;

    if (mInputType == EInputSystem_Type::DInput)
    {
        if (!InitInput())
            return false;
    }

    return true;
}

void CInput::Update(float DeltaTime)
{
    if (mInputType == EInputSystem_Type::DInput)
    {
        UpdateKeyboard();
        UpdateMouse();
    }

	UpdateMousePos(DeltaTime);

    UpdateInput(DeltaTime);

	UpdateBind(DeltaTime);
}

void CInput::UpdateMousePos(float DeltaTime)
{
	// 윈도우 창에서의 마우스 위치를 구한다.
	// GetCursorPos 함수는 스크린 좌표를 구해준다.
	// 스크린 좌표는 모니터상에서의 좌표를 구해준다.
	POINT	MousePT;
	GetCursorPos(&MousePT);

	// 스크린 좌표를 클라이언트 좌표로 변환한다.
	ScreenToClient(mhWnd, &MousePT);

	// 디바이스로부터 다이렉트 해상도와 윈도우 창 크기의 비율을 얻어온다.
	FVector2D	Ratio = CDevice::GetInst()->GetResolutionRatio();
	FResolution	ViewportRS = CDevice::GetInst()->GetResolution();

	FVector2D	MousePos;

	// 윈도우 창에서의 마우스 위치를 해상도 비율을 곱하여 DirectX Viewport
	// 상에서의 위치를 구한다.
	MousePos.x = MousePT.x * Ratio.x;
	MousePos.y = MousePT.y * Ratio.y;

	// 윈도우는 Y좌표가 아래로 Y+ 방향이 되고
	// DirectX에서는 Y좌표가 위로 Y+ 방향이 된다.
	// 그러므로 뷰포트 해상도를 이용하여 Y좌표를 반전시킨다.
	MousePos.y = ViewportRS.Height - MousePos.y;

	if (mMouseCompute)
	{

		mMouseMove = MousePos - mMousePos;
	}

	else
	{
		mMouseCompute = true;
	}

	mMousePos = MousePos;

	FVector3D WorldPos = mScene->GetCameraManager()->GetCameraWorldPos();

	mMouseWorldPos2D.x = WorldPos.x + mMousePos.x - ViewportRS.Width * 0.5f;
	mMouseWorldPos2D.y = WorldPos.y + mMousePos.y - ViewportRS.Height * 0.5f;
}

void CInput::UpdateInput(float DeltaTime)
{
    switch (mInputType)
    {
    case EInputSystem_Type::DInput:
		if (mKeyState[DIK_LCONTROL] & 0x80)
		{
			if (!mCtrl[EInputType::Down] && !mCtrl[EInputType::Hold])
			{
				mCtrl[EInputType::Down] = true;
				mCtrl[EInputType::Hold] = true;
			}

			else
				mCtrl[EInputType::Down] = false;
		}

		else if (mCtrl[EInputType::Hold])
		{
			mCtrl[EInputType::Down] = false;
			mCtrl[EInputType::Hold] = false;
			mCtrl[EInputType::Up] = true;
		}

		else if (mCtrl[EInputType::Up])
		{
			mCtrl[EInputType::Up] = false;
		}

		if (mKeyState[DIK_LALT] & 0x80)
		{
			if (!mAlt[EInputType::Down] && !mAlt[EInputType::Hold])
			{
				mAlt[EInputType::Down] = true;
				mAlt[EInputType::Hold] = true;
			}

			else
				mAlt[EInputType::Down] = false;
		}

		else if (mAlt[EInputType::Hold])
		{
			mAlt[EInputType::Down] = false;
			mAlt[EInputType::Hold] = false;
			mAlt[EInputType::Up] = true;
		}

		else if (mAlt[EInputType::Up])
		{
			mAlt[EInputType::Up] = false;
		}

		if (mKeyState[DIK_LSHIFT] & 0x80)
		{
			if (!mShift[EInputType::Down] && !mShift[EInputType::Hold])
			{
				mShift[EInputType::Down] = true;
				mShift[EInputType::Hold] = true;
			}

			else
				mShift[EInputType::Down] = false;
		}

		else if (mShift[EInputType::Hold])
		{
			mShift[EInputType::Down] = false;
			mShift[EInputType::Hold] = false;
			mShift[EInputType::Up] = true;
		}

		else if (mShift[EInputType::Up])
		{
			mShift[EInputType::Up] = false;
		}

        for (int i = 0; i < EMouseButtonType::End; ++i)
        {
            if (mMouseState.rgbButtons[i] & 0x80)
            {
                // Down이 false인 경우는 처음 누르는 경우이다.
                if (!mMouseDown[i] && !mMouseHold[i])
                {
                    mMouseDown[i] = true;
                    mMouseHold[i] = true;

					/*CLog::PrintLog("x: " + std::to_string(mMousePos.x) + ", y: " + std::to_string(mMousePos.y));
					CLog::PrintLog("world_x: " + std::to_string(mMouseWorldPos2D.x) + ", world_y: " + std::to_string(mMouseWorldPos2D.y));*/
                }

                // 이전 프레임에도 눌렀고 지금도 누르고 있다는 것이다.
                else
                    mMouseDown[i] = false;
            }

            // 키는 누르지 않았지만 Hold가 true인 경우라면 누르고 있다가
            // 떼는 경우이다.
            else if (mMouseHold[i])
            {
                mMouseDown[i] = false;
                mMouseHold[i] = false;
                mMouseUp[i] = true;
            }

            else if (mMouseUp[i])
                mMouseUp[i] = false;
        }
        break;
    case EInputSystem_Type::Window:
        break;
    }

	auto	iter = mKeyStateMap.begin();
	auto	iterEnd = mKeyStateMap.end();

	for (; iter != iterEnd; ++iter)
	{
		bool	KeyPush = false;

		switch (iter->second->Key)
		{
		case DIK_MOUSELBUTTON:
			if (mMouseState.rgbButtons[EMouseButtonType::LButton] & 0x80)
				KeyPush = true;
			break;
		case DIK_MOUSERBUTTON:
			if (mMouseState.rgbButtons[EMouseButtonType::RButton] & 0x80)
				KeyPush = true;
			break;
		case DIK_MOUSEWHEEL:
			break;
		default:
			if (mKeyState[iter->second->Key] & 0x80)
				KeyPush = true;
			break;
		}

		// 현재 해당 키를 누르고 있을 경우
		if (KeyPush)
		{
			if (!iter->second->Down && !iter->second->Hold)
			{
				iter->second->Down = true;
				iter->second->Hold = true;
			}

			else
				iter->second->Down = false;
		}

		// 현재 해당 키를 안누르고 있을 경우
		// Hold가 true라면 이전프레임에 이 키를 누르고 있다가
		// 이제 막 키를 해제했다는 것이다.
		else if (iter->second->Hold)
		{
			iter->second->Down = false;
			iter->second->Hold = false;
			iter->second->Up = true;
		}

		else if (iter->second->Up)
			iter->second->Up = false;
	}
}

void CInput::UpdateBind(float DeltaTime)
{
	auto	iter = mBindKeyMap.begin();
	auto	iterEnd = mBindKeyMap.end();

	for (; iter != iterEnd; ++iter)
	{
		// Down이 충족될 경우 등록된 함수를 호출한다.
		if (iter->second->Key->Down &&
			iter->second->Ctrl == mCtrl[EInputType::Down] &&
			iter->second->Alt == mAlt[EInputType::Down] &&
			iter->second->Shift == mShift[EInputType::Down])
		{
			size_t	Size = iter->second->FunctionList[EInputType::Down].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->FunctionList[EInputType::Down][i].Func(DeltaTime);
			}
		}

		// Hold가 충족될 경우 등록된 함수를 호출한다.
		if (iter->second->Key->Hold &&
			iter->second->Ctrl == mCtrl[EInputType::Hold] &&
			iter->second->Alt == mAlt[EInputType::Hold] &&
			iter->second->Shift == mShift[EInputType::Hold])
		{
			iter->second->KeyHold = true;
			size_t	Size = iter->second->FunctionList[EInputType::Hold].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->FunctionList[EInputType::Hold][i].Func(DeltaTime);
			}
		}

		bool	Verification = false;

		if (iter->second->Ctrl)
		{
			if (mCtrl[EInputType::Up])
				Verification = true;
		}

		if (iter->second->Alt)
		{
			if (mAlt[EInputType::Up])
				Verification = true;
		}

		if (iter->second->Shift)
		{
			if (mShift[EInputType::Up])
				Verification = true;
		}

		// Up이 충족될 경우 등록된 함수를 호출한다.
		if ((iter->second->Key->Up || Verification) &&
			iter->second->KeyHold)
		{
			iter->second->KeyHold = false;
			size_t	Size = iter->second->FunctionList[EInputType::Up].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->FunctionList[EInputType::Up][i].Func(DeltaTime);
			}
		}
	}
}

unsigned char CInput::ConvertKey(unsigned char Key)
{
    if (mInputType == EInputSystem_Type::DInput)
    {
		switch (Key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			//return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}

		return 0xff;
    }

	return Key;
}
