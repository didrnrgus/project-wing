#include "UserWidget.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Etc/ConstValues.h"
#include "Etc/ZOrderContainer.h"

CUserWidget::CUserWidget()
{
}

CUserWidget::~CUserWidget()
{
}

void CUserWidget::ShowLoading(bool isLoading)
{
	mIsLoading = isLoading;

	mLoadingBackImage->SetEnable(isLoading);
	mLoadingText->SetEnable(isLoading);
	mLoadingDescText->SetEnable(isLoading);
}

void CUserWidget::AddQueueLoadingDescText(const std::wstring wstrDesc)
{
	for (int i = 1; i <= wstrDesc.length(); i++)
	{
		auto str = wstrDesc.substr(0, i);
		mLoadingTextQueue.push_back(str);
#ifdef _DEBUG
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
#else
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
#endif // _DEBUG
	}
}

void CUserWidget::UpdateLoading(float DeltaTime)
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

	if (mLoadingTextQueue.size() > 0)
	{
		mLoadingDescText->SetText(mLoadingTextQueue.begin()->c_str());
		mLoadingTextQueue.pop_front();
	}
}

bool CUserWidget::Init()
{
	CWidget::Init();

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
    mLoadingBackImage->SetZOrder(ZORDER_LOADING);
	mLoadingBackImage->SetEnable(false);

	FVector2D loadingSize = FVector2D(190.0f, 120.0f);
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

	return true;
}

void CUserWidget::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

	auto    iter = mWidgetList.begin();
	auto    iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	// loading
	if (mIsLoading)
		UpdateLoading(DeltaTime);
}

void CUserWidget::Render()
{
	CWidget::Render();

	if (mWidgetList.size() >= 2)
	{
		std::sort(mWidgetList.begin(), mWidgetList.end(), CUserWidget::SortRender);
	}

	auto    iter = mWidgetList.begin();
	auto    iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render();
		++iter;
	}
}

void CUserWidget::Render(const FVector3D& Pos)
{
	CWidget::Render(Pos);

	if (mWidgetList.size() >= 2)
	{
		std::sort(mWidgetList.begin(), mWidgetList.end(),
			CUserWidget::SortRender);
	}

	auto    iter = mWidgetList.begin();
	auto    iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(Pos);
		++iter;
	}
}

bool CUserWidget::CollisionMouse(CWidget** Result, const FVector2D& MousePos)
{
	if (mWidgetList.size() >= 2)
	{
		std::sort(mWidgetList.begin(), mWidgetList.end(),
			CUserWidget::SortCollision);
	}

	auto    iter = mWidgetList.begin();
	auto    iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}
		++iter;
	}

	auto    riter = mWidgetList.rbegin();
	auto    riterEnd = mWidgetList.rend();

	for (; riter != riterEnd;)
	{
		if (!(*riter)->IsEnable())
		{
			++riter;
			continue;
		}

		if ((*riter)->CollisionMouse(Result, MousePos))
		{
			return true;
		}
		++riter;
	}

	return false;
}

bool CUserWidget::SortCollision(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}

bool CUserWidget::SortRender(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
	/*if (Src->GetZOrder() >= ZORDER_LOBBY_TOOLTIP_BACK
		|| Dest->GetZOrder() >= ZORDER_LOBBY_TOOLTIP_BACK)
	{
		CLog::PrintLog("");
	}*/
	return Src->GetZOrder() < Dest->GetZOrder();
}
