#pragma once
#include "GameInfo.h"

class ISceneNetworkController abstract
{
protected:
	std::list<class IObjectNetworkController*> mObjNetworkController;

    virtual void ProcessMessage() = 0;
    virtual void DistributeMessage(struct RecvMessage& msg) = 0;
    /*void DistributeMessage(struct RecvMessage& msg)
    {
        for (auto it : mObjNetworkController)
        {
            (it)->ProcessMessage(msg);
        }
    }*/
public:
    virtual void AddListener(class IObjectNetworkController* obj) = 0;
    virtual void RemoveListener(class IObjectNetworkController* obj) = 0;

    /*void AddListener(IObjectNetworkController* obj)
    {
        mObjNetworkController.push_back(obj);
    }

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
    }*/
};
