﻿#include "GameInfo.h"
#include "Etc/JsonController.h"
#include "Etc/NotionDBController.h"
#include "Etc/DataStorageManager.h"

DEFINITION_SINGLE(CJsonController);

CJsonController::CJsonController() {}

CJsonController::~CJsonController() {}

template<typename T>
nlohmann::json CJsonController::ConvertToJson(const T& data) { return nlohmann::json(); }

template<>
nlohmann::json CJsonController::ConvertToJson(const FUserRankInfo& data)
{
	return {
		{"parent", {
			{"database_id", CDataStorageManager::GetInst()->GetConfig().DatabaseID}
		}},
		{"properties", {
			{"title", {
				{"title", {{
					{"text", {{"content", "Untitled"}}}
				}}}
			}},
			{"name", {
				{"rich_text", {{
					{"text", {{"content", data.Name}}}
				}}}
			}},
			{"map", {{"number", data.Map}}},
			{"character", {{"number", data.Character}}},
			{"distance", {{"number", data.Distance}}},
			{"item_0", {{"number", data.Item_0}}},
			{"item_1", {{"number", data.Item_0}}},
			{"item_2", {{"number", data.Item_2}}}
		}}
	};
}

template<typename T>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<std::string, T>& datas) { return false; }

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<std::string, FColorInfo>& datas) 
{
	if (json.contains("colors") && json["colors"].is_array())
	{
		for (auto& jsonColorInfo : json["colors"])
		{
			auto _info = ParseJsonFColorInfo(jsonColorInfo);
			datas.emplace(std::make_pair(_info.ColorName, _info));
		}
	}

	if (datas.empty())
		return false;

	return true; 
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json
	, std::map<std::string, std::map<std::string, FSpriteSheetInfo>>& datas)
{
	FSpriteAtlasInfo atlasInfo;
	std::map<std::string, FSpriteSheetInfo> spriteSheetInfosByName;

	if (json.contains("fileName"))
		atlasInfo.FileName = json["fileName"].get<std::string>();

	if (json.contains("prefix"))
		atlasInfo.Prefix = json["prefix"].get<std::string>();

	if (json.contains("sizeX"))
		atlasInfo.SizeX = json["sizeX"].get<float>();

	if (json.contains("sizeY"))
		atlasInfo.SizeY = json["sizeY"].get<float>();

	if (json.contains("sprites") && json["sprites"].is_array())
	{
		for (auto& jsonSpriteSheetInfo : json["sprites"])
		{
			auto spriteSheetInfo = ParseJsonFSpriteSheetInfo(jsonSpriteSheetInfo);
			spriteSheetInfosByName.insert(std::make_pair(spriteSheetInfo.Name, spriteSheetInfo));
		}
	}

	datas.insert(std::make_pair(atlasInfo.FileName, spriteSheetInfosByName));
	return true;
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<std::string, FUserRankInfo>& datas)
{
	datas.clear();

	if (json.contains(RESULT) && json[RESULT].is_array())
	{
		for (const auto& rankInfoJson : json[RESULT])
		{
			FUserRankInfo rankInfo = ParseJsonFUserRankInfo(rankInfoJson);
			datas.emplace(std::pair<std::string, FUserRankInfo>(rankInfo.PageId, rankInfo));
		}
	}

	if (datas.empty())
		return false;

	return true;
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<std::string, FSpriteAtlasInfo>& datas)
{
	FSpriteAtlasInfo atlasInfo;

	if (json.contains("fileName"))
		atlasInfo.FileName = json["fileName"];

	if (json.contains("prefix"))
		atlasInfo.Prefix = json["prefix"];

	if (json.contains("sizeX"))
		atlasInfo.SizeX = json["sizeX"].get<float>();

	if (json.contains("sizeY"))
		atlasInfo.SizeY = json["sizeY"].get<float>();

	if (json.contains("sprites") && json["sprites"].is_array())
	{
		for (auto& jsonSpriteSheetInfo : json["sprites"])
		{
			auto spriteSheetInfo = ParseJsonFSpriteSheetInfo(jsonSpriteSheetInfo);
			atlasInfo.Sprites.push_back(spriteSheetInfo);
		}
	}

	datas.insert(std::make_pair(atlasInfo.FileName, atlasInfo));
	return true;
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<std::string, FStatInfo>& datas)
{
	if (json.contains("stat_type_list") && json["stat_type_list"].is_array())
	{
		for (auto& character : json["stat_type_list"])
		{
			FStatInfo statInfo;

			if (character.contains("index"))
				statInfo.Index = character["index"].get<int>();

			if (character.contains("type"))
				statInfo.Type = character["type"].get<int>();

			if (character.contains("name"))
				statInfo.Name = character["name"].get<std::string>();

			datas.insert(std::make_pair(statInfo.Name, statInfo));
		}
	}

	return true;
}

template<typename T>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<int, T>& datas) { return false; }

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<int, FCharacterState>& datas)
{
	if (json.contains("character_list") && json["character_list"].is_array())
	{
		for (auto& character : json["character_list"])
		{
			FCharacterState characterState;

			if (character.contains("index"))
				characterState.Index = character["index"].get<int>();
			if (character.contains("name"))
				characterState.Name = character["name"].get<std::string>();
			if (character.contains("color_name"))
				characterState.ColorName = character["color_name"].get<std::string>();

			if (character.contains("speed"))
				characterState.Speed = character["speed"].get<float>();
			if (character.contains("hp"))
				characterState.HP = character["hp"].get<float>();
			if (character.contains("dex"))
				characterState.Dex = character["dex"].get<float>();
			if (character.contains("def"))
				characterState.Def = character["def"].get<float>();
			if (character.contains("stun_duration"))
				characterState.StunDuration = character["stun_duration"].get<float>();

			if (character.contains("image_sequence_name"))
				characterState.ImageSequenceName = character["image_sequence_name"].get<std::string>();
			if (character.contains("size_x"))
				characterState.SizeX = character["size_x"].get<float>();
			if (character.contains("size_y"))
				characterState.SizeY = character["size_y"].get<float>();

			datas.insert(std::make_pair(characterState.Index, characterState));
		}
	}

	return true;
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<int, FItemInfo>& datas)
{
	if (json.contains("item_list") && json["item_list"].is_array())
	{
		for (auto& character : json["item_list"])
		{
			FItemInfo itmeInfo;

			if (character.contains("index"))
				itmeInfo.Index = character["index"].get<int>();

			if (character.contains("name"))
				itmeInfo.Name = character["name"].get<std::string>();

			if (character.contains("stat_type"))
				itmeInfo.StatType = character["stat_type"].get<int>();

			if (character.contains("add_value"))
				itmeInfo.AddValue = character["add_value"].get<float>();

			if (character.contains("desc"))
				itmeInfo.Desc = character["desc"].get<std::string>();

			datas.insert(std::make_pair(itmeInfo.Index, itmeInfo));
		}
	}

	return true;
}


template<typename T>
bool CJsonController::ParseJson(const nlohmann::json& json, T& data) { return false; }

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, FConfig& data)
{
	if (json.contains("db_id"))
		data.DatabaseID = json["db_id"].get<std::string>();

	if (json.contains("api_key"))
		data.APIKey = json["api_key"].get<std::string>();

	if (json.contains("map_file_list") && json["map_file_list"].is_array())
	{
		for (auto& mapFile : json["map_file_list"])
		{
			data.mapFileNameList.push_back(mapFile.get<std::string>());
		}
	}

	if (json.contains("character_file"))
		data.CharacterFileName = json["character_file"].get<std::string>();

	if (json.contains("item_file"))
		data.ItemFileName = json["item_file"].get<std::string>();

	if (json.contains("color_file"))
		data.ColorFileName = json["color_file"].get<std::string>();

	if (json.contains("stat_file"))
		data.StatFileName = json["stat_file"].get<std::string>();

	if (json.contains("selectable_item_count"))
		data.SelectableItemCount = json["selectable_item_count"].get<int>();

	if (json.contains("rank_count_by_category"))
		data.RankCountByCategory = json["rank_count_by_category"].get<int>();

	return true;
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, FMapInfo& data)
{
	if (json.contains("index"))
		data.Index = json["index"].get<int>();

	if (json.contains("name"))
		data.Name = json["name"].get<std::string>();

	if (json.contains("color_name"))
		data.ColorName = json["color_name"].get<std::string>();

	if (json.contains("difficulty_rate"))
		data.DifficultyRate = json["difficulty_rate"].get<float>();

	if (json.contains("collision_damage"))
		data.CollisionDamage = json["collision_damage"].get<float>();

	if (json.contains("obstacle_interval_time"))
		data.ObstacleIntervalTime = json["obstacle_interval_time"].get<float>();

	if (json.contains("line_node_list") && json["line_node_list"].is_array())
	{
		for (auto& jsonLineNode : json["line_node_list"])
		{
			FLineNode lineNodeInfo = ParseJsonFLineNode(jsonLineNode);
			data.lineNodes.push_back(lineNodeInfo);
		}
	}

	return true;
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, FLineNode& data)
{
	data = ParseJsonFLineNode(json);
	return true;
}

FLineNode CJsonController::ParseJsonFLineNode(const nlohmann::json& json)
{
	FLineNode lineNode;

	if (json.contains("top_y_pos"))
		lineNode.TopYPos = json["top_y_pos"].get<float>();

	if (json.contains("bottom_y_pos"))
		lineNode.BottomYPos = json["bottom_y_pos"].get<float>();

	return lineNode;
}

FSpriteSheetInfo CJsonController::ParseJsonFSpriteSheetInfo(const nlohmann::json& json)
{
	FSpriteSheetInfo spritSheetInfo;

	if (json.contains("name"))
		spritSheetInfo.Name = json["name"].get<std::string>();

	if (json.contains("x"))
		spritSheetInfo.X = json["x"].get<float>();

	if (json.contains("y"))
		spritSheetInfo.Y = json["y"].get<float>();

	if (json.contains("width"))
		spritSheetInfo.Width = json["width"].get<float>();

	if (json.contains("height"))
		spritSheetInfo.Height = json["height"].get<float>();

	if (json.contains("pivotX"))
		spritSheetInfo.PivotX = json["pivotX"].get<float>();

	if (json.contains("pivotY"))
		spritSheetInfo.PivotY = json["pivotY"].get<float>();

	return spritSheetInfo;
}

FUserRankInfo CJsonController::ParseJsonFUserRankInfo(const nlohmann::json& json)
{
	FUserRankInfo rankInfo;

	if (json.contains(PROPERTIES))
	{
		std::string pageID = json[ID];

		// name
		if (json[PROPERTIES].contains("name")
			&& json[PROPERTIES]["name"].contains("rich_text"))
			rankInfo.Name = json[PROPERTIES]["name"]["rich_text"][0]["plain_text"].get<std::string>();

		// map
		if (json[PROPERTIES].contains("map")
			&& json[PROPERTIES]["map"].contains(ATT_NUMBER))
			rankInfo.Map = json[PROPERTIES]["map"][ATT_NUMBER].get<int>();

		// character
		if (json[PROPERTIES].contains("character")
			&& json[PROPERTIES]["character"].contains(ATT_NUMBER))
			rankInfo.Character = json[PROPERTIES]["character"][ATT_NUMBER].get<int>();

		// distance
		if (json[PROPERTIES].contains("distance")
			&& json[PROPERTIES]["distance"].contains(ATT_NUMBER))
			rankInfo.Distance = json[PROPERTIES]["distance"][ATT_NUMBER].get<int>();

		// item_0
		if (json[PROPERTIES].contains("item_0")
			&& json[PROPERTIES]["item_0"].contains(ATT_NUMBER))
			rankInfo.Item_0 = json[PROPERTIES]["item_0"][ATT_NUMBER].get<int>();

		// item_1
		if (json[PROPERTIES].contains("item_1")
			&& json[PROPERTIES]["item_1"].contains(ATT_NUMBER))
			rankInfo.Item_1 = json[PROPERTIES]["item_1"][ATT_NUMBER].get<int>();

		// item_2
		if (json[PROPERTIES].contains("item_2")
			&& json[PROPERTIES]["item_2"].contains(ATT_NUMBER))
			rankInfo.Item_2 = json[PROPERTIES]["item_2"][ATT_NUMBER].get<int>();

		rankInfo.PageId = pageID;
	}

	return rankInfo;
}

FColorInfo CJsonController::ParseJsonFColorInfo(const nlohmann::json& json)
{
	FColorInfo colorInfo;

	// ColorName
	if (json.contains("color_name"))
		colorInfo.ColorName = json["color_name"].get<std::string>();

	// HexName
	if (json.contains("hex"))
		colorInfo.HexName = json["hex"].get<std::string>();

	// R
	if (json.contains("r"))
		colorInfo.R = json["r"].get<float>();

	// G
	if (json.contains("g"))
		colorInfo.G = json["g"].get<float>();

	// B
	if (json.contains("b"))
		colorInfo.B = json["b"].get<float>();

	// A
	if (json.contains("a"))
		colorInfo.A = json["a"].get<float>();

	return colorInfo;
}


