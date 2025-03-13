#include "FontManager.h"
#include "Device.h"

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
    mFontMap.clear();

    auto    iter = mFontColorMap.begin();
    auto    iterEnd = mFontColorMap.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_RELEASE(iter->second);
    }

    SAFE_RELEASE(mWriteFactory);
}

bool CFontManager::Init()
{
    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(mWriteFactory), (IUnknown**)&mWriteFactory)))
        return false;

    LoadFontCollection("Default", TEXT("Font/NotoSansKR-Regular.otf"));

    // en-us
    // GetFontFaceName 함수를 이용해서 DefaultCollection에 있는
    // FontFaceName을 얻어온다.
    LoadFont("Default", GetFontFaceName("Default"), 600, 20.f, TEXT("ko"));

    CreateFontColor(0, 0, 0, 255);
    CreateFontColor(255, 255, 255, 255);
    CreateFontColor(255, 0, 0, 255);
    CreateFontColor(0, 255, 0, 255);
    CreateFontColor(0, 0, 255, 255);
    CreateFontColor(255, 0, 255, 255);
    CreateFontColor(255, 255, 0, 255);
    CreateFontColor(0, 255, 255, 255);

    return true;
}

bool CFontManager::LoadFont(const std::string& Name, 
    const TCHAR* FontName, int Weight, float FontSize,
    const TCHAR* LocalName, int Stretch)
{
    CFont* Font = FindFont(Name);

    if (Font)
        return true;

    Font = new CFont;

    Font->mName = Name;

    if (!Font->LoadFont(mWriteFactory, FontName, Weight, FontSize,
        LocalName, Stretch))
    {
        SAFE_DELETE(Font);
        return false;
    }

    mFontMap.insert(std::make_pair(Name, Font));

    return true;
}

bool CFontManager::LoadFontCollection(const std::string& Name, 
    const TCHAR* FileName)
{
    CFontCollection* FontCollection = FindFontCollection(Name);

    if (FontCollection)
        return true;

    FontCollection = new CFontCollection;

    FontCollection->mName = Name;

    if (!FontCollection->LoadCollection(mWriteFactory, FileName))
    {
        SAFE_DELETE(FontCollection);
        return false;
    }

    mCollectionMap.insert(std::make_pair(Name, FontCollection));

    return true;
}

const TCHAR* CFontManager::GetFontFaceName(const std::string& CollectionName)
{
    CFontCollection* FontCollection = FindFontCollection(CollectionName);

    if (!FontCollection)
        return nullptr;

    return FontCollection->GetFontFaceName();
}

bool CFontManager::CreateFontColor(unsigned char r, unsigned char g, 
    unsigned char b, unsigned char a)
{
    ID2D1SolidColorBrush* Brush = nullptr;

    unsigned int    Color = a;
    Color = Color << 8;

    Color = Color | r;
    Color = Color << 8;

    Color = Color | g;
    Color = Color << 8;

    Color = Color | b;

    if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
        D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f),
        &Brush)))
        return false;

    mFontColorMap.insert(std::make_pair(Color, Brush));

    return true;
}

bool CFontManager::CreateFontColor(const FVector4D& Color)
{
    ID2D1SolidColorBrush* Brush = nullptr;

    unsigned char r, g, b, a;
    r = (unsigned char)(Color.x * 255);
    g = (unsigned char)(Color.y * 255);
    b = (unsigned char)(Color.z * 255);
    a = (unsigned char)(Color.w * 255);

    unsigned int    Key = a;
    Key = Key << 8;

    Key = Key | r;
    Key = Key << 8;

    Key = Key | g;
    Key = Key << 8;

    Key = Key | b;

    if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
        D2D1::ColorF(Color.x, Color.y, Color.z, Color.w),
        &Brush)))
        return false;

    mFontColorMap.insert(std::make_pair(Key, Brush));

    return true;
}

CFont* CFontManager::FindFont(const std::string& Name)
{
    auto    iter = mFontMap.find(Name);

    if (iter == mFontMap.end())
        return nullptr;

    return iter->second;
}

CFontCollection* CFontManager::FindFontCollection(const std::string& Name)
{
    auto    iter = mCollectionMap.find(Name);

    if (iter == mCollectionMap.end())
        return nullptr;

    return iter->second;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned char r,
    unsigned char g, unsigned char b, unsigned char a)
{
    unsigned int    Key = a;
    Key = Key << 8;

    Key = Key | r;
    Key = Key << 8;

    Key = Key | g;
    Key = Key << 8;

    Key = Key | b;

    if (mFontColorMap.count(Key) == false)
    {
        CreateFontColor(r, g, b, a);
    }

    auto iter = mFontColorMap.find(Key);
    return iter->second;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const FVector4D& Color)
{
    unsigned char r, g, b, a;
    r = (unsigned char)(Color.x * 255);
    g = (unsigned char)(Color.y * 255);
    b = (unsigned char)(Color.z * 255);
    a = (unsigned char)(Color.w * 255);

    unsigned int    Key = a;
    Key = Key << 8;

    Key = Key | r;
    Key = Key << 8;

    Key = Key | g;
    Key = Key << 8;

    Key = Key | b;

    if (mFontColorMap.count(Key) == false)
    {
        CreateFontColor(r, g, b, a);
    }

    auto iter = mFontColorMap.find(Key);
    return iter->second;
}
