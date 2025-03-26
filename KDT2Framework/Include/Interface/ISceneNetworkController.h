#pragma once
#include "GameInfo.h"

class ISceneNetworkController abstract
{
protected:
	std::list<class IObjectNetworkController*> mObjNetworkController;

    // CScene 또는  상속받은 클래스의 Update() 안에서 호출 되어야 함.
    virtual void ProcessMessage() = 0;
    /*  void ProcessMessage()
		{
			// 메시지 큐에서 메시지 뽑는 역할.
			RecvMessage msg;
			if (CNetworkManager::GetInst()->PollMessage(msg))
			{
				DistributeMessage(msg);

				// 순서: 커넥트 -> 방장누구? -> 기존인원 리스트
				switch (msg.msgType)
				{
				case (int)ServerMessage::Type::MSG_JOIN:
				{
					break;
				}
				}
			}
		}    */

    // ProcessMessage() 내에서 메시지 가져온 다음에 호출 함.
    virtual void DistributeMessage(const struct RecvMessage& msg) = 0;
    /*void DistributeMessage(struct RecvMessage& msg)
    {
        for (auto it : mObjNetworkController)
        {
            (it)->ProcessMessage(msg);
        }
    }*/

public:
    // IObjectNetworkController 상속받은 씬내 오브젝트들이 호출 함.
    virtual void AddListener(class IObjectNetworkController* obj) = 0;
    /*void AddListener(IObjectNetworkController* obj)
    {
        mObjNetworkController.push_back(obj);
    }*/

    // IObjectNetworkController 상속받은 씬내 오브젝트들이 호출 함.
    virtual void RemoveListener(class IObjectNetworkController* obj) = 0;
    /*void RemoveListener(IObjectNetworkController* obj)
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
    }*/

};
