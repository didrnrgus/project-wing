#pragma once

#include "../GameInfo.h"

class CObject	abstract
{
public:
	CObject();
	virtual ~CObject();

protected:
	int		mRefCount = 0;
	bool	mActive = true;
	bool	mEnable = true;

public:
	bool IsActive()	const
	{
		return mActive;
	}

	bool IsEnable()	const
	{
		return mEnable;
	}

	int GetRefCount()	const
	{
		return mRefCount;
	}

public:
	virtual void AddRef();
	virtual void Release();
	virtual void Destroy();
	void SetEnable(bool Enable)
	{
		mEnable = Enable;
	}
};

