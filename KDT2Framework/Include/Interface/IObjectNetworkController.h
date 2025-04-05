#pragma once
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Interface/ISceneNetworkController.h"
#include "Etc/NetworkManager.h"
/*
protected:
	virtual void AddListener() override;
	virtual void RemoveListener() override;
public:
	virtual void ProcessMessage(const struct RecvMessage& msg) override;
*/
class IObjectNetworkController abstract
{
protected:
	int mNetID = -1;
	std::function<void(int)> playerNetIdCallback;

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

	// ClientMessage::MSG_START
	// ClientMessage::MSG_READY
	// ClientMessage::MSG_UNREADY
	// ClientMessage::MSG_MOVE_UP
	// ClientMessage::MSG_MOVE_DOWN
	// ClientMessage::MSG_PLAYER_DEAD
	void SendMessageTrigger(ClientMessage::Type _msgType)
	{
		CNetworkManager::GetInst()->SendMsg(0, (int)_msgType, nullptr, 0);
	}

	// ClientMessage::MSG_PICK_MAP
	// ClientMessage::MSG_PICK_CHARACTER
	void SendMessageTriggerInt(ClientMessage::Type _msgType, int _data)
	{
		CNetworkManager::GetInst()->SendMsg(0, (int)_msgType, &_data, sizeof(int));
	}

	// ClientMessage::MSG_TAKE_DAMAGE
	void SendMessageTriggerFloat(ClientMessage::Type _msgType, float _data)
	{
		CNetworkManager::GetInst()->SendMsg(0, (int)_msgType, &_data, sizeof(float));
	}

	// ClientMessage::MSG_PICK_ITEM
	void SendMessageTriggerItem(ClientMessage::Type _msgType, int _slotIndex, int _itemId)
	{
		int data[2] = { _slotIndex, _itemId };
		CNetworkManager::GetInst()->SendMsg(0, (int)_msgType, data, sizeof(data));
	}

	// 템플릿 콜백 등록
	template <typename T>
	void SetPlayerNetIdCallback(T* Obj, void(T::* Func)())
	{
		playerNetIdCallback = std::bind(Func, Obj);
	}

	// 람다 콜백 등록
	using CallbackFuncType = std::function<void(int)>; // 타입 이름 충돌 방지
	void SetPlayerNetIdCallback(CallbackFuncType&& Func)
	{
		playerNetIdCallback = std::move(Func);
	}

public:
	virtual void ProcessMessage(const struct RecvMessage& msg) = 0;
	/*void ProcessMessage(const RecvMessage& msg)
	{
		switch (msg.msgType)
		{
		case (int)ServerMessage::MSG_..:
		default:
			break;
		}
	}*/

public:
	void SetNetID(int _id)
	{
		mNetID = _id;

		if (playerNetIdCallback != nullptr)
			playerNetIdCallback(mNetID);
	}
	int GetNetID() { return mNetID; }
};