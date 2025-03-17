#include "ProfileManager.h"
#include "Etc/ConstString.h"

DEFINITION_SINGLE(CProfileManager)

CProfileManager::CProfileManager()
{
}

CProfileManager::~CProfileManager()
{
	auto	iter = mProfileMap.begin();
	auto	iterEnd = mProfileMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CProfileManager::Init()
{
	// create
	CreateProfile(PROFILE_DEFAULT, ECollisionChannel::Default, false,
		ECollisionInteraction::Ignore);

	CreateProfile(PROFILE_PLAYER_MINE, ECollisionChannel::PlayerMine, true,
		ECollisionInteraction::Collision);

	CreateProfile(PROFILE_PLAYER_OTHER, ECollisionChannel::PlayerOther, false,
		ECollisionInteraction::Ignore);

	CreateProfile(PROFILE_MAP, ECollisionChannel::Map, true,
		ECollisionInteraction::Collision);

	// interraction playerMine
	SetInteraction(PROFILE_PLAYER_MINE, ECollisionChannel::Default,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_PLAYER_MINE, ECollisionChannel::PlayerMine,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_PLAYER_MINE, ECollisionChannel::PlayerOther,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_PLAYER_MINE, ECollisionChannel::Map,
		ECollisionInteraction::Collision);

	// interraction playerOther
	SetInteraction(PROFILE_PLAYER_OTHER, ECollisionChannel::Default,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_PLAYER_OTHER, ECollisionChannel::PlayerMine,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_PLAYER_OTHER, ECollisionChannel::PlayerOther,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_PLAYER_OTHER, ECollisionChannel::Map,
		ECollisionInteraction::Ignore);

	// interaction map
	SetInteraction(PROFILE_MAP, ECollisionChannel::Default,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_MAP, ECollisionChannel::PlayerMine,
		ECollisionInteraction::Collision);

	SetInteraction(PROFILE_MAP, ECollisionChannel::PlayerOther,
		ECollisionInteraction::Ignore);

	SetInteraction(PROFILE_MAP, ECollisionChannel::Map,
		ECollisionInteraction::Ignore);

	return true;
}

bool CProfileManager::CreateProfile(const std::string& Name,
	ECollisionChannel::Type Channel,
	bool Enable, ECollisionInteraction::Type Interaction)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	Profile = new FCollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->Enable = Enable;

	for (int i = 0; i < ECollisionChannel::End; ++i)
	{
		Profile->Interaction[i] = Interaction;
	}

	mProfileMap.insert(std::make_pair(Name, Profile));

	return true;
}

bool CProfileManager::SetInteraction(const std::string& Name,
	ECollisionChannel::Type Channel,
	ECollisionInteraction::Type Interaction)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->Interaction[Channel] = Interaction;

	return true;
}

bool CProfileManager::SetEnable(const std::string& Name,
	bool Enable)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->Enable = Enable;

	return true;
}

FCollisionProfile* CProfileManager::FindProfile(
	const std::string& Name)
{
	auto	iter = mProfileMap.find(Name);

	if (iter == mProfileMap.end())
		return nullptr;

	return iter->second;
}
