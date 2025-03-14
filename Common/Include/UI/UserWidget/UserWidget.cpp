#include "UserWidget.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"

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
}

bool CUserWidget::Init()
{
    CWidget::Init();

    mLoadingTextStrings.push_back(L"Loading.");
    mLoadingTextStrings.push_back(L"Loading..");
    mLoadingTextStrings.push_back(L"Loading...");
    mLoadingTextStrings.push_back(L"Loading....");

    FResolution RS = CDevice::GetInst()->GetResolution();
    mLoadingBackImage = mScene->GetUIManager()->CreateWidget<CImage>("LoadingBackImage");
    AddWidget(mLoadingBackImage);
    mLoadingBackImage->SetTexture("LoadingBackImage", TEXT("Texture/basic.png"));
    mLoadingBackImage->SetPivot(FVector2D::Zero);
    mLoadingBackImage->SetSize(FVector2D(RS.Width, RS.Height));
    mLoadingBackImage->SetColor(FVector4D::Black);
    mLoadingBackImage->SetOpacity(0.8f);
    mLoadingBackImage->SetPos(FVector2D::Zero);
    mLoadingBackImage->SetZOrder(ZORDER_LOADING);
    mLoadingBackImage->SetEnable(false);

    mLoadingText = mScene->GetUIManager()->CreateWidget<CTextBlock>("LoadingText");
    AddWidget(mLoadingText);
    mLoadingText->SetText(mLoadingTextStrings[curLoadingTextIndex]);
    mLoadingText->SetTextColor(FVector4D::Green);
    mLoadingText->SetAlignH(ETextAlignH::Center);
    mLoadingText->SetFontSize(30.f);
    mLoadingText->SetShadowEnable(true);
    mLoadingText->SetShadowOffset(3.f, 3.f);
    mLoadingText->SetTextShadowColor(FVector4D::Gray30);
    FVector2D commentSize = FVector2D(500.0f, 100.0f);
    mLoadingText->SetSize(commentSize);
    mLoadingText->SetPos(FVector2D(RS.Width, RS.Height) * 0.5f - commentSize * 0.5f);
    mLoadingText->SetZOrder(ZORDER_LOADING);
    mLoadingText->SetEnable(false);

    //ShowLoading(true);

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
    if(mIsLoading)
        UpdateLoading(DeltaTime);
}

void CUserWidget::Render()
{
    CWidget::Render();

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

bool CUserWidget::SortCollision(const CSharedPtr<CWidget>& Src,
    const CSharedPtr<CWidget>& Dest)
{
    return Src->GetZOrder() < Dest->GetZOrder();
}

bool CUserWidget::SortRender(const CSharedPtr<CWidget>& Src, 
    const CSharedPtr<CWidget>& Dest)
{
    return Src->GetZOrder() < Dest->GetZOrder();
}
