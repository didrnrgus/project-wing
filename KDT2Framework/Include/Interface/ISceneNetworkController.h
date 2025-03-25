#pragma once
#include "GameInfo.h"
#include "Interface/IObjectNetworkController.h"

class ISceneNetworkController abstract
{
private:
	std::list<IObjectNetworkController*> mObjNetworkController;

protected:
	// CScene::Update() 안에서 호출되고, 함수내부는 메시지 큐에서 뽑아서
	// 자기한테 구독한 오브젝트들에게 뿌린다.
    // 씬이 쓰고싶으면, ProcessMessage(struct RecvMessage& msg)
	virtual void ProcessMessage() = 0;
    virtual void ProcessMessage(struct RecvMessage& msg) = 0;

    void DistributeMessage(struct RecvMessage& msg)
    {
        for (auto it : mObjNetworkController)
        {
            (it)->ProcessMessage(msg);
        }
    }
public:

    // Object 입장에선 this 포인터를 줘도 되지않나?
    // -> IObjectNetworkController 상속 받을꺼니까.
    void AddListener(IObjectNetworkController* obj)
    {
        mObjNetworkController.push_back(obj);
    }

    // Object 입장에선 this 포인터를 줘도 되지않나?
    // -> IObjectNetworkController 상속 받을꺼니까.
    void RemoveListener(IObjectNetworkController* obj)
    {
        if (obj)
        {
            auto it = std::find_if(mObjNetworkController.begin(), mObjNetworkController.end(),
                [obj](const IObjectNetworkController* const _obj) 
                {
                    return _obj == obj; 
                });

            if (it != mObjNetworkController.end()) 
            {
                mObjNetworkController.erase(it);
            }
        }
    }
};
