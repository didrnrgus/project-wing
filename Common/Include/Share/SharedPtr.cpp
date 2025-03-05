#include "SharedPtr.h"
#include "Object.h"

CSharedPtr::CSharedPtr()
{
}

CSharedPtr::CSharedPtr(CObject* Obj)
{
    mObj = Obj;

    // 오브젝트가 있을 경우 참조가 일어나는 것이기 때문에 참조 카운트를 1 증가시킨다.
    if (mObj)
        mObj->AddRef();
}

CSharedPtr::CSharedPtr(const CSharedPtr& Ptr)
{
    mObj = Ptr.mObj;

    // 오브젝트가 있을 경우 참조가 일어나는 것이기 때문에 참조 카운트를 1 증가시킨다.
    if (mObj)
        mObj->AddRef();
}

CSharedPtr::CSharedPtr(CSharedPtr&& Ptr)
{
    mObj = Ptr.mObj;

    // 오브젝트가 있을 경우 참조가 일어나는 것이기 때문에 참조 카운트를 1 증가시킨다.
    if (mObj)
        mObj->AddRef();
}

CSharedPtr::~CSharedPtr()
{
    // 오브젝트가 있을 경우 참조카운트가 증가되었기 때문에 참조카운트를 감소시킨다.
    if (mObj)
        mObj->Release();
}

void CSharedPtr::operator=(CObject* Obj)
{
    if (mObj)
        mObj->Release();

    mObj = Obj;

    // 오브젝트가 있을 경우 참조가 일어나는 것이기 때문에 참조 카운트를 1 증가시킨다.
    if (mObj)
        mObj->AddRef();
}

void CSharedPtr::operator=(const CSharedPtr& Ptr)
{
    if (mObj)
        mObj->Release();

    mObj = Ptr.mObj;

    // 오브젝트가 있을 경우 참조가 일어나는 것이기 때문에 참조 카운트를 1 증가시킨다.
    if (mObj)
        mObj->AddRef();
}

void CSharedPtr::operator=(CSharedPtr&& Ptr)
{
    if (mObj)
        mObj->Release();

    mObj = Ptr.mObj;

    // 오브젝트가 있을 경우 참조가 일어나는 것이기 때문에 참조 카운트를 1 증가시킨다.
    if (mObj)
        mObj->AddRef();
}

bool CSharedPtr::operator==(CObject* Obj) const
{
    return mObj == Obj;
}

bool CSharedPtr::operator==(const CSharedPtr& Ptr) const
{
    return mObj == Ptr.mObj;
}

bool CSharedPtr::operator==(CSharedPtr&& Ptr) const
{
    return mObj == Ptr.mObj;
}

bool CSharedPtr::operator!=(CObject* Obj) const
{
    return mObj != Obj;
}

bool CSharedPtr::operator!=(const CSharedPtr& Ptr) const
{
    return mObj != Ptr.mObj;
}

bool CSharedPtr::operator!=(CSharedPtr&& Ptr) const
{
    return mObj != Ptr.mObj;
}

CObject* CSharedPtr::operator->() const
{
    return mObj;
}

CSharedPtr::operator class CObject* () const
{
    return mObj;
}

CObject* CSharedPtr::Get() const
{
    return mObj;
}
