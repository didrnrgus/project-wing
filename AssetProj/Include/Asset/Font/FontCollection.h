#pragma once

#include "../Asset.h"

class CFontCollection :
    public CAsset
{
    friend class CFontManager;

private:
    CFontCollection();
    ~CFontCollection();

private:
    IDWriteFontCollection1* mCollection = nullptr;
    TCHAR                   mFontFaceName[128] = {};

public:
    const TCHAR* GetFontFaceName()  const
    {
        return mFontFaceName;
    }

public:
    bool LoadCollection(IDWriteFactory5* Factory, const TCHAR* FileName);
};

