#include "SceneUIManager.h"

CSceneUIManager::CSceneUIManager()
{
}

CSceneUIManager::~CSceneUIManager()
{
}

void CSceneUIManager::AddToViewport(CWidget* Widget)
{
    mWidgetList.emplace_back(Widget);
}

bool CSceneUIManager::Init()
{
    return true;
}

void CSceneUIManager::Update(float DeltaTime)
{
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

bool CSceneUIManager::CollisionMouse(float DeltaTime, 
    const FVector2D& MousePos)
{
    if (mWidgetList.size() >= 2)
    {
        std::sort(mWidgetList.begin(), mWidgetList.end(),
            CSceneUIManager::SortCollision);
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

        CWidget* HoveredWidget = nullptr;

        if ((*iter)->CollisionMouse(&HoveredWidget, MousePos))
        {
            if (mMouseHoveredWidget != HoveredWidget)
            {
                if (mMouseHoveredWidget)
                    mMouseHoveredWidget->MouseUnHovered();

                mMouseHoveredWidget = HoveredWidget;
            }

            return true;
        }

        ++iter;
    }

    // 충돌된 Widget이 없는데 기존에 충돌되었던 Widget이 있을 경우
    if (mMouseHoveredWidget)
    {
        mMouseHoveredWidget->MouseUnHovered();
        mMouseHoveredWidget = nullptr;
    }

    return false;
}

void CSceneUIManager::Collision(float DeltaTime)
{
}

void CSceneUIManager::Render()
{
    if (mWidgetList.size() >= 2)
    {
        std::sort(mWidgetList.begin(), mWidgetList.end(),
            CSceneUIManager::SortRender);
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

    iter = mWidgetList.begin();
    iterEnd = mWidgetList.end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)->EndFrame();
    }
}

bool CSceneUIManager::SortCollision(const CSharedPtr<CWidget>& Src, 
    const CSharedPtr<CWidget>& Dest)
{
    return Src->GetZOrder() > Dest->GetZOrder();
}

bool CSceneUIManager::SortRender(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
    return Src->GetZOrder() < Dest->GetZOrder();
}
