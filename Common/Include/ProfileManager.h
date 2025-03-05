#pragma once

#include "GameInfo.h"

class CProfileManager
{
private:
	std::unordered_map<std::string, FCollisionProfile*>	mProfileMap;

public:
	bool Init();
	bool CreateProfile(const std::string& Name,
		ECollisionChannel::Type Channel, bool Enable,
		ECollisionInteraction::Type Interaction);
	bool SetInteraction(const std::string& Name,
		ECollisionChannel::Type Channel,
		ECollisionInteraction::Type Interaction);
	bool SetEnable(const std::string& Name,
		bool Enable);
	FCollisionProfile* FindProfile(const std::string& Name);


	DECLARE_SINGLE(CProfileManager)
};

