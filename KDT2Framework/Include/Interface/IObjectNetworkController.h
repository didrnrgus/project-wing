#pragma once
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Interface/ISceneNetworkController.h"
#include "Etc/NetworkManager.h"

class IObjectNetworkController abstract
{
protected:
	virtual void AddListener() = 0;
	/*void AddListener()
	{
		auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
		if (sceneNetController == nullptr)
			return;
		sceneNetController->AddListener(this);
	}	*/

	virtual void RemoveListener() = 0;
	/*void RemoveListener()
	{
		auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
		sceneNetController->RemoveListener(this);
	}*/

	// ClientMessage::Type::MSG_START
	// ClientMessage::Type::MSG_READY
	// ClientMessage::Type::MSG_UNREADY
	// ClientMessage::Type::MSG_MOVE_UP
	// ClientMessage::Type::MSG_MOVE_DOWN
	// ClientMessage::Type::MSG_PLAYER_DEAD
	void SendMessageTrigger(ClientMessage::Type _msgType) 
	{
		CNetworkManager::GetInst()->SendMsg(0, (int)_msgType, nullptr, 0);
	}

	// ClientMessage::Type::MSG_PICK_MAP
	// ClientMessage::Type::MSG_PICK_CHARACTER
	void SendMessageTriggerInt(ClientMessage::Type _msgType, int _data)
	{
		CNetworkManager::GetInst()->SendMsg(0, (int)_msgType, &_data, sizeof(int));
	}

	// ClientMessage::Type::MSG_PICK_ITEM
	void SendMessageTriggerItem(ClientMessage::Type _msgType, int _slotIndex, int _itemId)
	{
		int data[2] = { _slotIndex, _itemId };
		CNetworkManager::GetInst()->SendMsg(0, (int)_msgType, data, sizeof(data));
	}

public:
	virtual void ProcessMessage(const struct RecvMessage& msg) = 0;
	/*
		void ProcessMessage(const RecvMessage& msg)
		{
			switch (msg.msgType)
			{
			case (int)ServerMessage::Type::MSG_..:
			default:
				break;
			}
		}
	*/
};