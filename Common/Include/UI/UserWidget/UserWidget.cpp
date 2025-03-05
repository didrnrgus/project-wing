#include "UserWidget.h"

CUserWidget::CUserWidget()
{
}

CUserWidget::~CUserWidget()
{
}

bool CUserWidget::Init()
{
    CWidget::Init();

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
