#pragma once
#include "GameInfo.h"
#include "Network/Client.h"
#include "Network/Server.h"
#include "Etc/CURL.h"
#include "Etc/DataStorageManager.h"

void LoadGameData()
{
	// config load
	std::string webserverPath = WEBSERVER_PATH;
	std::string path = webserverPath + CONFIG_PATH;
	std::string configResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	printf(("configResult: " + configResult).c_str());
	CDataStorageManager::GetInst()->SetConfigData(configResult);

	// characters load
	path = webserverPath + CDataStorageManager::GetInst()->GetConfig().CharacterFileName;
	std::string charactersResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	printf(("charactersResult: " + charactersResult).c_str());
	CDataStorageManager::GetInst()->SetCharacterData(charactersResult);

	// maps load
	for (std::string mapFileName : CDataStorageManager::GetInst()->GetConfig().mapFileNameList)
	{
		path = webserverPath + mapFileName;
		std::string mapResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
		printf(("mapResult: " + mapResult).c_str());
		CDataStorageManager::GetInst()->SetMapData(mapResult);
	}

	// stat load
	path = webserverPath + CDataStorageManager::GetInst()->GetConfig().StatFileName;
	std::string statsResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	printf(("statsResult: " + statsResult).c_str());
	CDataStorageManager::GetInst()->SetStatInfoData(statsResult);

	// item load
	path = webserverPath + CDataStorageManager::GetInst()->GetConfig().ItemFileName;
	std::string itemResult = CCURL::GetInst()->SendRequest(path, METHOD_GET);
	printf(("itemResult: " + itemResult).c_str());
	CDataStorageManager::GetInst()->SetItemInfoData(itemResult);
}

int main()
{
	srand(GetTickCount());
	rand();

	LoadGameData();

	CServer server;
	server.Init();
	server.Run();

	return 0;
}

