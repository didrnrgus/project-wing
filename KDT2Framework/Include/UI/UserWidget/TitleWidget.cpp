#include "TitleWidget.h"
#include "Device.h"
#include "GameManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Etc/CURL.h"
#include "Etc/TaskManager.h"
#include "Etc/DataStorageManager.h"
#include "Etc/ProcessManager.h"
#include "Etc/NetworkManager.h"
#include "Interface/ISceneNetworkController.h"

extern BOOL option2Visible;

CTitleWidget::CTitleWidget()
{
	mMaxReConnectionTime = 10.0f;
	mCurReConnectionTime = 0.0f;
}

CTitleWidget::~CTitleWidget()
{
	RemoveListener();
}

bool CTitleWidget::Init()
{
	CUserWidget::Init();
	AddListener();

	CLog::PrintLog("CTitleWidget::Init()");
	FResolution RS = CDevice::GetInst()->GetResolution();
	FVector2D size = FVector2D(200.0f, 100.0f);
	FVector2D singlePos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 4) - size * 0.5f;
	FVector2D multiPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 3) - size * 0.5f;
	FVector2D rankPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 2) - size * 0.5f;
	FVector2D exitPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 1) - size * 0.5f;

	SetButtonWithTextBlock(mSinglePlayButton, "SinglePlay", singlePos
		, &CTitleWidget::SinglePlayButtonClick, mSinglePlayTextBlock, TEXT("SinglePlay"));
	SetButtonWithTextBlock(mMultiPlayButton, "MultiPlay", multiPos
		, &CTitleWidget::MultiPlayButtonClick, mMultiPlayTextBlock, TEXT("MultiPlay"));
	SetButtonWithTextBlock(mRankButton, "Rank", rankPos
		, &CTitleWidget::RankButtonClick, mRankTextBlock, TEXT("Rank"));
	SetButtonWithTextBlock(mExitButton, "Exit", exitPos
		, &CTitleWidget::ExitButtonClick, mExitTextBlock, TEXT("Exit"));

	return true;
}

void CTitleWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (CNetworkManager::GetInst()->IsConnection()
		&& !CNetworkManager::GetInst()->IsConnectCompleted())
	{
		mCurReConnectionTime += DeltaTime;

		if (mCurReConnectionTime >= mMaxReConnectionTime)
		{
			mCurReConnectionTime = 0.0f;
			CLog::PrintLog("CNetworkManager::GetInst()->ConnetServer();");
			CNetworkManager::GetInst()->ConnetServer();
		}
	}
}

void CTitleWidget::SetButtonWithTextBlock(CSharedPtr<CButton>& button, std::string name, FVector2D pos
	, void(CTitleWidget::* Func)(), CSharedPtr<CTextBlock>& textBlock, const wchar_t* textBlockContent)
{
	button = mScene->GetUIManager()->CreateWidget<CButton>(name + "Button");
	AddWidget(button);
	button->SetPos(pos);
	button->SetSize(200.f, 100.f);
	button->SetColor(0, 0, 0, 0);
	button->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, Func);
	textBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>(name + "Text");
	button->SetChild(textBlock);
	textBlock->SetText(textBlockContent);
	textBlock->SetTextColor(FVector4D::Green);
	textBlock->SetAlignH(ETextAlignH::Center);
	textBlock->SetFontSize(30.f);
	textBlock->SetShadowEnable(true);
	textBlock->SetShadowOffset(3.f, 3.f);
	textBlock->SetTextShadowColor(FVector4D::Gray30);
}

void CTitleWidget::LoadGameData(bool _isActiveServerProcess, bool _isMultiPlay)
{
	ShowLoading(true);

#ifdef _DEBUG
	int waitTime = 1000;
#else
	int waitTime = 2000;
#endif // _DEBUG

	// config load
	AddQueueLoadingDescText(L"Config Data를 로딩 중 입니다.\n👨🏻‍💻");
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	std::string webserverPath = WEBSERVER_PATH;
	std::string path = webserverPath + CONFIG_PATH;
	std::string configResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	CLog::PrintLog("configResult: " + configResult);
	CDataStorageManager::GetInst()->SetConfigData(configResult);

	// characters load
	AddQueueLoadingDescText(L"캐릭터 데이터를 로딩 중 입니다.\n캐릭터는 다섯가지가 있어요.\n👹.👺.💀.👻.👽");
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	path = webserverPath + CDataStorageManager::GetInst()->GetConfig().CharacterFileName;
	std::string charactersResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	CLog::PrintLog("charactersResult: " + charactersResult);
	CDataStorageManager::GetInst()->SetCharacterData(charactersResult);

	// maps load
	AddQueueLoadingDescText(L"맵 데이터를 로딩 중 입니다.\n맵은 난이도별로 세가지가 있어요.\n🏜️,🏖️,🏞️");
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	for (std::string mapFileName : CDataStorageManager::GetInst()->GetConfig().mapFileNameList)
	{
		path = webserverPath + mapFileName;
		std::string mapResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
		CLog::PrintLog("mapResult: " + mapResult);
		CDataStorageManager::GetInst()->SetMapData(mapResult);
	}

	// stat load
	AddQueueLoadingDescText(L"스텟 데이터를 로딩 중 입니다.\n스텟은 체력.스피드.민첩.디펜스 4가지가 있어요.");
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	path = webserverPath + CDataStorageManager::GetInst()->GetConfig().StatFileName;
	std::string statsResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	CLog::PrintLog("statsResult: " + statsResult);
	CDataStorageManager::GetInst()->SetStatInfoData(charactersResult);

	// item load
	AddQueueLoadingDescText(L"아이템 데이터를 로딩 중 입니다.\n아이템 은 4가지 이고, 소유한 아이템의 스텟 패시브 효과만 있어요.");
	std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	path = webserverPath + CDataStorageManager::GetInst()->GetConfig().ItemFileName;
	std::string itemResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	CLog::PrintLog("itemResult: " + itemResult);
	CDataStorageManager::GetInst()->SetItemInfoData(itemResult);

	CTaskManager::GetInst()->RemoveTask(mTaskID);

	if (_isActiveServerProcess)
	{
		AddQueueLoadingDescText(L"서버 프로세스를 Child로 실행합니다.\n내가 호스트니까요~💻💻💻💻💻");
		std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
		CProcessManager::GetInst()->LaunchProcess(L"../Bin/Server/server.exe");
	}

	CNetworkManager::GetInst()->SetIsMultiplay(_isMultiPlay);
	
	if (_isMultiPlay)
	{
		AddQueueLoadingDescText(L"호스트 서버에 접속 중 입니다.\n메뉴 바 에서 호스트 정보 확인하셨죠??");
		std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));

		// 첫번쨰 시도 -> 실패해도 Update() 에서 될때까지 커넥션 한다.
		CNetworkManager::GetInst()->ConnetServer();
	}
	else
	{
		// Single mode.
		// 멀티플레이에서는 연결 성공했을때 하자.
		CSceneManager::GetInst()->CreateLoadScene<CSceneLobby>();
	}

	SetIsSkipLoadingTextUpdate(true);
}

void CTitleWidget::SinglePlayButtonClick()
{
	// 로비로 가야 함.
	CLog::PrintLog("CTitleWidget::SinglePlayButtonClick()");

	if (IsLoading())
		return;

	mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
		[this]()
		{
			LoadGameData(false);
		})));

}

void CTitleWidget::MultiPlayButtonClick()
{
	// 로비로 가야 함.
	CLog::PrintLog("CTitleWidget::MultiPlayButtonClick()");

	if (IsLoading())
		return;

	mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
		[this]()
		{
			LoadGameData(!option2Visible, true);
		})));

}

void CTitleWidget::RankButtonClick()
{
	// Move Rank Scene 
	CLog::PrintLog("CTitleWidget::RankButtonClick()");

	if (IsLoading())
		return;
}

void CTitleWidget::ExitButtonClick()
{
	CLog::PrintLog("CTitleWidget::ExitButtonClick()");

	if (IsLoading())
		return;

	CGameManager::GetInst()->ExitGame();
}

void CTitleWidget::ProcessMessage(const RecvMessage& msg)
{

}

void CTitleWidget::AddListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->AddListener(this);
}

void CTitleWidget::RemoveListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->RemoveListener(this);
}
