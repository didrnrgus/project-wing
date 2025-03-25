#pragma once
#include "GameInfo.h"
#include "Interface/IObjectNetworkController.h"

class ISceneNetworkController abstract
{
private:
	std::list<IObjectNetworkController*> mObjNetworkController;

protected:
	// CScene::Update() �ȿ��� ȣ��ǰ�, �Լ����δ� �޽��� ť���� �̾Ƽ�
	// �ڱ����� ������ ������Ʈ�鿡�� �Ѹ���.
    // ���� ���������, ProcessMessage(struct RecvMessage& msg)
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

    // Object ���忡�� this �����͸� �൵ �����ʳ�?
    void AddListener(IObjectNetworkController* obj)
    {
        mObjNetworkController.push_back(obj);
    }

    // Object ���忡�� this �����͸� �൵ �����ʳ�?
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
