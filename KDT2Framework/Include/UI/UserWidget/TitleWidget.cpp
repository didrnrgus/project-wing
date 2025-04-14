#include "TitleWidget.h"
#include "Device.h"
#include "GameManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneTitle.h"
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

#ifdef _DEBUG
	mWaitTime = 500;
	mIsSkip = true;
#else
	mWaitTime = 500;
	mIsSkip = true;
#endif // _DEBUG

}

CTitleWidget::~CTitleWidget()
{
	RemoveListener();
}

bool CTitleWidget::Init()
{
	CSceneWidget::Init();
	CLog::PrintLog("CTitleWidget::Init()");
	AddListener();

	FResolution RS = CDevice::GetInst()->GetResolution();
	mResolution = FVector2D(RS.Width, RS.Height);

	mTitleText = mScene->GetUIManager()->CreateWidget<CTextBlock>("mTitleText");
	AddWidget(mTitleText);
	mTitleText->SetPivot(FVector2D::One * 0.5f);
	mTitleText->SetSize(FVector2D(1000.0f, 100.0f));
	mTitleText->SetPos(FVector2D(mResolution.x * 0.5f, mResolution.y * 0.8f));
	mTitleText->SetText(L"🐦PROJ-WING🐦");
	mTitleText->SetTextColor(FVector4D::Green);
	mTitleText->SetAlignH(ETextAlignH::Center);
	mTitleText->SetFontSize(100.0f);
	mTitleText->SetShadowEnable(true);
	mTitleText->SetShadowOffset(5.f, 5.f);
	mTitleText->SetTextShadowColor(FVector4D::DarkGray);

	FVector2D size = FVector2D(200.0f, 100.0f);
	FVector2D singlePos = FVector2D(mResolution.x * 0.5f, mResolution.y * 0.15f * 4) - size * 0.5f;
	FVector2D multiPos = FVector2D(mResolution.x * 0.5f, mResolution.y * 0.15f * 3) - size * 0.5f;
	FVector2D rankPos = FVector2D(mResolution.x * 0.5f, mResolution.y * 0.15f * 2) - size * 0.5f;
	FVector2D exitPos = FVector2D(mResolution.x * 0.5f, mResolution.y * 0.15f * 1) - size * 0.5f;

	SetButtonWithTextBlock(ETitleUIType::SinglePlay, mSinglePlayButton, "SinglePlay", singlePos
		, &CTitleWidget::SinglePlayButtonClick, mSinglePlayTextBlock, TEXT("SinglePlay"));
	mArrTextBlock.push_back(mSinglePlayTextBlock);
	SetButtonWithTextBlock(ETitleUIType::MultiPlay, mMultiPlayButton, "MultiPlay", multiPos
		, &CTitleWidget::MultiPlayButtonClick, mMultiPlayTextBlock, TEXT("MultiPlay"));
	mArrTextBlock.push_back(mMultiPlayTextBlock);
	SetButtonWithTextBlock(ETitleUIType::Rank, mRankButton, "Rank", rankPos
		, &CTitleWidget::RankButtonClick, mRankTextBlock, TEXT("Rank"));
	mArrTextBlock.push_back(mRankTextBlock);
	SetButtonWithTextBlock(ETitleUIType::Exit, mExitButton, "Exit", exitPos
		, &CTitleWidget::ExitButtonClick, mExitTextBlock, TEXT("Exit"));
	mArrTextBlock.push_back(mExitTextBlock);

	if (CDataStorageManager::GetInst()->GetIsLoadedData() == false)
		LoadGameData();

	return true;
}

void CTitleWidget::Update(float DeltaTime)
{
	CSceneWidget::Update(DeltaTime);

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

void CTitleWidget::SetButtonWithTextBlock(ETitleUIType _type
	, CSharedPtr<CButton>& button
	, std::string name
	, FVector2D pos
	, void(CTitleWidget::* ClickCallbackFunc)()
	, CSharedPtr<CTextBlock>& textBlock
	, const wchar_t* textBlockContent)
{
	button = mScene->GetUIManager()->CreateWidget<CButton>(name + "Button");
	AddWidget(button);
	button->SetPos(pos);
	button->SetSize(200.f, 100.f);
	button->SetColor(0, 0, 0, 0);
	button->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, ClickCallbackFunc);
	button->SetEventCallback(EButtonEventState::Hovered
		, [this, _type]()
		{
			mArrTextBlock[(int)_type]->SetTextColor(FVector4D::Green);
		});
	button->SetEventCallback(EButtonEventState::Unhoverd
		, [this, _type]()
		{
			mArrTextBlock[(int)_type]->SetTextColor(FVector4D::White);
		});
	textBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>(name + "Text");
	button->SetChild(textBlock);
	textBlock->SetText(textBlockContent);
	textBlock->SetTextColor(FVector4D::White);
	textBlock->SetAlignH(ETextAlignH::Center);
	textBlock->SetFontSize(30.f);
	textBlock->SetShadowEnable(true);
	textBlock->SetShadowOffset(3.f, 3.f);
	textBlock->SetTextShadowColor(FVector4D::DarkGray);
}

void CTitleWidget::LoadGameData()
{
	mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
		[this]()
		{
			ShowLoading(true);
			mScene->SetIsLoadingScene(true);

			// config load
			AddQueueLoadingDescText(L"Config Data를 로딩 중 입니다.\n👨🏻‍💻", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
			std::string webserverPath = WEBSERVER_PATH;
			std::string path = webserverPath + CONFIG_PATH;
			std::string configResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
			CLog::PrintLog("configResult: " + configResult);
			CDataStorageManager::GetInst()->SetConfigData(configResult);

			// maps load
			AddQueueLoadingDescText(L"맵 데이터를 로딩 중 입니다.\n맵은 난이도별로 세가지가 있어요.\n🏜️,🏖️,🏞️", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
			for (auto mapFileName : CDataStorageManager::GetInst()->GetConfig().mapFileNameList)
			{
				path = webserverPath + mapFileName;
				std::string mapResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
				CLog::PrintLog("mapResult: " + mapResult);
				CDataStorageManager::GetInst()->SetMapData(mapResult);
			}

			// characters load
			AddQueueLoadingDescText(L"캐릭터 데이터를 로딩 중 입니다.\n캐릭터는 다섯가지가 있어요.\n👹.👺.💀.👻.👽", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
			path = webserverPath + CDataStorageManager::GetInst()->GetConfig().CharacterFileName;
			std::string charactersResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
			CLog::PrintLog("charactersResult: " + charactersResult);
			CDataStorageManager::GetInst()->SetCharacterData(charactersResult);

			// item load
			AddQueueLoadingDescText(L"아이템 데이터를 로딩 중 입니다.\n아이템 은 4가지 이고, 소유한 아이템의 스텟 패시브 효과만 있어요.", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
			path = webserverPath + CDataStorageManager::GetInst()->GetConfig().ItemFileName;
			std::string itemResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
			CLog::PrintLog("itemResult: " + itemResult);
			CDataStorageManager::GetInst()->SetItemInfoData(itemResult);
			
			// stat load
			AddQueueLoadingDescText(L"스텟 데이터를 로딩 중 입니다.\n스텟은 체력.스피드.민첩.디펜스 4가지가 있어요.", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
			path = webserverPath + CDataStorageManager::GetInst()->GetConfig().StatFileName;
			std::string statsResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
			CLog::PrintLog("statsResult: " + statsResult);
			CDataStorageManager::GetInst()->SetStatInfoData(statsResult);

			// color load
			AddQueueLoadingDescText(L"컬러 데이터를 로딩 중 입니다.\n🎨✖️30", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
			path = webserverPath + CDataStorageManager::GetInst()->GetConfig().ColorFileName;
			std::string colorResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
			CLog::PrintLog("colorResult: " + colorResult);
			CDataStorageManager::GetInst()->SetColorInfoData(colorResult);

			// 컨피그 관련 데이터 로드 끝 표시.
			CDataStorageManager::GetInst()->SetIsLoadedData(true);

			CTaskManager::GetInst()->RemoveTask(mTaskID);
			mScene->SetIsLoadingScene(false);
			ShowLoading(false);

			LoadRankData();
			auto _sceneTitle = dynamic_cast<CSceneTitle*>(mScene);
			_sceneTitle->InitObjectAfterDataLoad();
		})));
}

void CTitleWidget::LoadRankData()
{
	mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
		[this]()
		{
			ShowLoading(true);
			mScene->SetIsLoadingScene(true);
			// rank load
			AddQueueLoadingDescText(L"Ranking Data를 로딩 중 입니다. ✨🎉🎊🎉🎊✨\n랭킹은 맵, 캐릭터 기준 거리 랭킹으로 각 카테고리 내 5위까지 제공됩니다.", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
			CDataStorageManager::GetInst()->UpdateUserRankInfos();

			CTaskManager::GetInst()->RemoveTask(mTaskID);
			mScene->SetIsLoadingScene(false);
			ShowLoading(false);
		})));
}

void CTitleWidget::LoadProcess()
{
	mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
		[this]()
		{
			ShowLoading(true);

			if (!option2Visible)
			{
				AddQueueLoadingDescText(L"서버 프로세스를 Child로 실행합니다.\n내가 호스트니까요~🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻", mIsSkip);
				std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));
#ifdef _DEBUG
				CProcessManager::GetInst()->LaunchProcess(L"../Bin/Server/Debug/project-wing-socket-server.exe");
#else
				CProcessManager::GetInst()->LaunchProcess(L"../Bin/Server/Release/project-wing-socket-server.exe");
#endif
			}

			CNetworkManager::GetInst()->SetIsMultiplay(true);
			AddQueueLoadingDescText(L"호스트 서버에 접속 중 입니다.\n메뉴 바 에서 호스트 정보 확인하셨죠??🛜🛜🛜🛜🛜", mIsSkip);
			std::this_thread::sleep_for(std::chrono::milliseconds(mWaitTime));

			// 첫번쨰 시도 -> 실패해도 Update() 에서 될때까지 커넥션 한다.
			CNetworkManager::GetInst()->ConnetServer();

		})));
}

void CTitleWidget::SinglePlayButtonClick()
{
	// 로비로 가야 함.
	CLog::PrintLog("CTitleWidget::SinglePlayButtonClick()");

	if (IsLoading())
		return;

	LoadScene(EGameScene::Lobby);
}

void CTitleWidget::MultiPlayButtonClick()
{
	// 로비로 가야 함.
	CLog::PrintLog("CTitleWidget::MultiPlayButtonClick()");

	if (IsLoading())
		return;

	LoadProcess();
}

void CTitleWidget::RankButtonClick()
{
	// Move Rank Scene 
	CLog::PrintLog("CTitleWidget::RankButtonClick()");

	if (IsLoading())
		return;

	LoadScene(EGameScene::Rank);
}

void CTitleWidget::ExitButtonClick()
{
	CLog::PrintLog("CTitleWidget::ExitButtonClick()");

	if (IsLoading())
		return;

	CGameManager::GetInst()->ExitGame();
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

void CTitleWidget::ProcessMessage(const RecvMessage& msg)
{
	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_ROOM_FULL_INFO:
	{
		LoadScene(EGameScene::Lobby);
		break;
	}
	case (int)ServerMessage::MSG_CONNECTED_REJECT:
	{
		// 이유 알려주는 UI 필요.
		CNetworkManager::GetInst()->SetIsMultiplay(false);
		ShowLoading(false);
		break;
	}
	default:
		break;
	}
}
