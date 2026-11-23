#include "GameData.h"

#include <windows.h>
#include <winhttp.h>
#include <cstdio>
#include <vector>

#include "nlohmann/json.hpp"

#pragma comment(lib, "winhttp.lib")

using json = nlohmann::json;

namespace
{
	// HTTPS GET. 실패하면 빈 문자열.
	std::string HttpGet(const std::wstring& _path)
	{
		std::string result;

		HINTERNET session = WinHttpOpen(L"project-wing-socket-server/1.0",
			WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		if (!session)
			return result;

		WinHttpSetTimeouts(session, 5000, 5000, 5000, 10000);

		HINTERNET connect = WinHttpConnect(session, WEBSERVER_HOST, INTERNET_DEFAULT_HTTPS_PORT, 0);
		HINTERNET request = connect ? WinHttpOpenRequest(connect, L"GET", _path.c_str(), nullptr,
			WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE) : nullptr;

		DWORD statusCode = 0;
		if (request
			&& WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)
			&& WinHttpReceiveResponse(request, nullptr))
		{
			DWORD size = sizeof(statusCode);
			WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
				WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &size, WINHTTP_NO_HEADER_INDEX);

			DWORD available = 0;
			while (WinHttpQueryDataAvailable(request, &available) && available > 0)
			{
				std::vector<char> buffer(available);
				DWORD read = 0;
				if (!WinHttpReadData(request, buffer.data(), available, &read) || read == 0)
					break;
				result.append(buffer.data(), read);
			}
		}

		if (statusCode != 200)
			result.clear();

		if (request) WinHttpCloseHandle(request);
		if (connect) WinHttpCloseHandle(connect);
		WinHttpCloseHandle(session);
		return result;
	}

	bool GetJson(const std::string& _fileName, json& _out)
	{
		std::wstring path = WEBSERVER_DIR + std::wstring(_fileName.begin(), _fileName.end());
		std::string body = HttpGet(path);
		if (body.empty())
		{
			printf("[GameData] download failed: %s\n", _fileName.c_str());
			return false;
		}

		_out = json::parse(body, nullptr, false);
		if (_out.is_discarded())
		{
			printf("[GameData] parse failed: %s\n", _fileName.c_str());
			return false;
		}
		return true;
	}

	float GetFloat(const json& _j, const char* _key, float _default)
	{
		auto it = _j.find(_key);
		return (it != _j.end() && it->is_number()) ? it->get<float>() : _default;
	}

	int GetInt(const json& _j, const char* _key, int _default)
	{
		auto it = _j.find(_key);
		return (it != _j.end() && it->is_number()) ? it->get<int>() : _default;
	}
}

bool CGameData::Load()
{
	json config;
	if (!GetJson("config.json", config))
		return false;

	bool ok = true;

	// 맵
	if (config.contains("map_file_list") && config["map_file_list"].is_array())
	{
		int order = 0;
		for (auto& fileName : config["map_file_list"])
		{
			json mapJson;
			if (fileName.is_string() && GetJson(fileName.get<std::string>(), mapJson))
			{
				FMapStat map;
				map.CollisionDamage = GetFloat(mapJson, "collision_damage", mDefaultMap.CollisionDamage);
				map.ObstacleIntervalTime = GetFloat(mapJson, "obstacle_interval_time", mDefaultMap.ObstacleIntervalTime);
				mMaps[GetInt(mapJson, "index", order)] = map;
			}
			else
				ok = false;
			++order;
		}
	}

	// 캐릭터
	json characterJson;
	if (config.contains("character_file") && GetJson(config["character_file"].get<std::string>(), characterJson)
		&& characterJson.contains("character_list"))
	{
		int order = 0;
		for (auto& c : characterJson["character_list"])
		{
			FCharacterStat stat;
			stat.HP = GetFloat(c, "hp", mDefaultCharacter.HP);
			stat.Speed = GetFloat(c, "speed", mDefaultCharacter.Speed);
			stat.Dex = GetFloat(c, "dex", mDefaultCharacter.Dex);
			stat.Def = GetFloat(c, "def", mDefaultCharacter.Def);
			stat.StunDuration = GetFloat(c, "stun_duration", mDefaultCharacter.StunDuration);
			mCharacters[GetInt(c, "index", order)] = stat;
			++order;
		}
	}
	else
		ok = false;

	// 아이템
	json itemJson;
	if (config.contains("item_file") && GetJson(config["item_file"].get<std::string>(), itemJson)
		&& itemJson.contains("item_list"))
	{
		int order = 0;
		for (auto& i : itemJson["item_list"])
		{
			FItemStat item;
			item.StatType = GetInt(i, "stat_type", EStatType::End);
			item.AddValue = GetFloat(i, "add_value", 0.0f);
			mItems[GetInt(i, "index", order)] = item;
			++order;
		}
	}
	else
		ok = false;

	printf("[GameData] maps: %zu, characters: %zu, items: %zu\n", mMaps.size(), mCharacters.size(), mItems.size());

	mIsLoaded = ok;
	return ok;
}

const FCharacterStat& CGameData::GetCharacter(int _index) const
{
	auto it = mCharacters.find(_index);
	return it != mCharacters.end() ? it->second : mDefaultCharacter;
}

const FItemStat* CGameData::GetItem(int _index) const
{
	auto it = mItems.find(_index);
	return it != mItems.end() ? &it->second : nullptr;
}

const FMapStat& CGameData::GetMap(int _index) const
{
	auto it = mMaps.find(_index);
	return it != mMaps.end() ? it->second : mDefaultMap;
}
