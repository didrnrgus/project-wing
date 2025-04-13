#include "RankWidget.h"
#include "Device.h"
#include "GameManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"

CRankWidget::CRankWidget()
{
}

CRankWidget::~CRankWidget()
{
}

bool CRankWidget::Init()
{
    return true;
}

void CRankWidget::Update(float DeltaTime)
{
    CSceneWidget::Update(DeltaTime);

}

void CRankWidget::InitCommonUI()
{
}

void CRankWidget::InitMemu()
{
}

void CRankWidget::InitProperty(FVector2D _basePos)
{
}

void CRankWidget::InitUserRankPrint(FVector2D _basePos, int _count)
{
}

void CRankWidget::SetPositionUserPrintRow(FUserPrintGroup& _groupOut, FVector2D _basePos)
{
}

void CRankWidget::SetInfoTextBlock(CTextBlock* _textBlock, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const wchar_t* _str)
{
}

void CRankWidget::SetImage(CImage* _image, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const char* _imageName, const wchar_t* _imagePath)
{
}

void CRankWidget::OnClickMainCategoryMenuTapButton(ERankMenuTap::Type _tap)
{
}

void CRankWidget::OnClickSubCategoryMenuTapButton(int _index)
{
}

void CRankWidget::UpdateUserRankPrint(std::vector<FUserRankInfo> _arrInfo)
{
}

void CRankWidget::SetButton(CButton& _button, const char* _name, const wchar_t* _path)
{
}
