#include "SceneWidget.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Etc/ConstValues.h"
#include "Etc/ZOrderContainer.h"
#include "Etc/TaskManager.h"
#include "Share/Timer.h"

CSceneWidget::CSceneWidget()
{
    mIsSkipLoadingTextUpdate = false;
}

CSceneWidget::~CSceneWidget()
{
}

bool CSceneWidget::Init()
{
	CUserWidget::Init();

	mLoadingTextStrings.push_back(L"Loading 🛼");
	mLoadingTextStrings.push_back(L"Loading  🛼");
	mLoadingTextStrings.push_back(L"Loading   🛼");
	mLoadingTextStrings.push_back(L"Loading    🛼");

	FResolution RS = CDevice::GetInst()->GetResolution();
	mLoadingBackImage = mScene->GetUIManager()->CreateWidget<CImage>("LoadingBackImage");
	AddWidget(mLoadingBackImage);
	mLoadingBackImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mLoadingBackImage->SetPivot(FVector2D::Zero);
	mLoadingBackImage->SetSize(FVector2D(RS.Width, RS.Height));
	mLoadingBackImage->SetColor(FVector4D::Black);
	mLoadingBackImage->SetOpacity(0.8f);
	mLoadingBackImage->SetPos(FVector2D::Zero);
	mLoadingBackImage->SetZOrder(ZORDER_LOADING_BACK);
	mLoadingBackImage->SetEnable(false);

	FVector2D loadingSize = FVector2D(200.0f, 120.0f);
	mLoadingText = mScene->GetUIManager()->CreateWidget<CTextBlock>("LoadingText");
	AddWidget(mLoadingText);
	mLoadingText->SetText(mLoadingTextStrings[curLoadingTextIndex]);
	mLoadingText->SetTextColor(FVector4D::Green);
	mLoadingText->SetAlignH(ETextAlignH::Left);
	mLoadingText->SetFontSize(30.f);
	mLoadingText->SetShadowEnable(true);
	mLoadingText->SetShadowOffset(3.f, 3.f);
	mLoadingText->SetTextShadowColor(FVector4D::Gray30);
	mLoadingText->SetSize(loadingSize);
	mLoadingText->SetPos(FVector2D(RS.Width, RS.Height) * 0.5f - loadingSize * 0.5f - FVector2D(0.0f, 60.0f));
	mLoadingText->SetZOrder(ZORDER_LOADING);
	mLoadingText->SetEnable(false);

	FVector2D descSize = FVector2D(1000.0f, 120.0f);
	mLoadingDescText = mScene->GetUIManager()->CreateWidget<CTextBlock>("LoadingDescText");
	AddWidget(mLoadingDescText);
	mLoadingDescText->SetText(L"조금만 기다리세요.");
	mLoadingDescText->SetTextColor(FVector4D::Yellow);
	mLoadingDescText->SetAlignH(ETextAlignH::Center);
	mLoadingDescText->SetFontSize(30.f);
	mLoadingDescText->SetShadowEnable(true);
	mLoadingDescText->SetShadowOffset(3.f, 3.f);
	mLoadingDescText->SetTextShadowColor(FVector4D::Gray30);
	mLoadingDescText->SetSize(descSize);
	mLoadingDescText->SetPos(FVector2D(RS.Width, RS.Height) * 0.5f - descSize * 0.5f + FVector2D::Axis[EAxis::Y] * 40);
	mLoadingDescText->SetZOrder(ZORDER_LOADING);
	mLoadingDescText->SetEnable(false);

	FVector2D fpsTextSize = FVector2D(300.0f, 20.0f);
	mFpsText = mScene->GetUIManager()->CreateWidget<CTextBlock>("FpsText");
	AddWidget(mFpsText);
	mFpsText->SetPivot(FVector2D::Zero);
	mFpsText->SetText(L"FPS: ");
	mFpsText->SetTextColor(FVector4D::Green);
	mFpsText->SetAlignH(ETextAlignH::Left);
	mFpsText->SetFontSize(20.f);
	mFpsText->SetShadowEnable(false);
	mFpsText->SetSize(fpsTextSize);
	mFpsText->SetPos(FVector2D::One * 0.0f);
	mFpsText->SetZOrder(ZORDER_FPS);
    
	return true;
}

void CSceneWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	// loading
	if (mIsLoading)
		UpdateLoading(DeltaTime);

	if (mFpsText)
		mFpsText->SetText(CTimer::GetFPS());
}

void CSceneWidget::SetEnableFPS(bool _isEnable)
{
    mFpsText->SetEnable(_isEnable);
}

void CSceneWidget::ShowLoading(bool _isLoading)
{
	mIsLoading = _isLoading;

	mLoadingBackImage->SetEnable(_isLoading);
	mLoadingText->SetEnable(_isLoading);
	mLoadingDescText->SetEnable(_isLoading);
	mIsSkipLoadingTextUpdate = !_isLoading;
}

void CSceneWidget::AddQueueLoadingDescText(const std::wstring _wstrDesc, bool _isSkip)
{
	if (_isSkip)
	{
		mLoadingTextQueue.push_back(_wstrDesc);
		return;
	}

	for (int i = 1; i <= _wstrDesc.length(); i++)
	{
		auto str = _wstrDesc.substr(0, i);

		std::lock_guard<std::mutex> lock(mQueueMutex);
		mLoadingTextQueue.push_back(str);
#ifdef _DEBUG
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
#else
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
#endif // _DEBUG
	}
}

void CSceneWidget::UpdateLoading(float DeltaTime)
{
	loadingUpdateTime += DeltaTime;

	if (loadingUpdateTime > 0.2f)
	{
		loadingUpdateTime = 0.0f;
		curLoadingTextIndex++;

		if (curLoadingTextIndex == mLoadingTextStrings.size())
		{
			curLoadingTextIndex = 0;
		}

		mLoadingText->SetText(mLoadingTextStrings[curLoadingTextIndex]);
	}


	if (mIsSkipLoadingTextUpdate)
		return;

	loadingTextUpdateTime += DeltaTime;

	if (loadingTextUpdateTime > 0.03f)
	{
		loadingTextUpdateTime = 0.0f;
		std::lock_guard<std::mutex> lock(mQueueMutex);
		if (mLoadingTextQueue.size() > 0)
		{
			mLoadingDescText->SetText(mLoadingTextQueue.begin()->c_str());
			mLoadingTextQueue.pop_front();
		}
	}
}


void CSceneWidget::LoadScene(EGameScene::Type _sceneType)
{
	mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
		[this, _sceneType]()
		{
			ShowLoading(true);
			mScene->SetIsLoadingScene();

			if (_sceneType == EGameScene::Title)
				AddQueueLoadingDescText(L"타이틀 화면으로 이동중입니다!!!");
			else if (_sceneType == EGameScene::Lobby)
				AddQueueLoadingDescText(L"로비 화면으로 이동중입니다!!!");
			else if (_sceneType == EGameScene::InGame)
				AddQueueLoadingDescText(L"인게임 화면으로 이동중입니다!!!");
			else if (_sceneType == EGameScene::Result)
				AddQueueLoadingDescText(L"결과 화면으로 이동중입니다!!!");
			else if (_sceneType == EGameScene::Rank)
				AddQueueLoadingDescText(L"랭킹 화면으로 이동중입니다!!!");

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			CTaskManager::GetInst()->RemoveTask(mTaskID);

			if (_sceneType == EGameScene::Title)
				mScene->GotoTitle();
			else if (_sceneType == EGameScene::Lobby)
				mScene->GotoLobby();
			else if (_sceneType == EGameScene::InGame)
				mScene->GotoInGame();
			else if (_sceneType == EGameScene::Result)
				mScene->GotoResult();
			else if (_sceneType == EGameScene::Rank)
				mScene->GotoRank();

		})));
}