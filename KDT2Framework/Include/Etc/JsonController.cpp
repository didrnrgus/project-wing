#include "GameInfo.h"
#include "Etc/JsonController.h"
#include "Etc/NotionDBController.h"

DEFINITION_SINGLE(CJsonController);

CJsonController::CJsonController() {}

CJsonController::~CJsonController() {}

template<typename T>
nlohmann::json CJsonController::ConvertToJson(const T& data) { return nlohmann::json(); }

template<typename T>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<std::string, T>& datas) { return false; }


template<typename T>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<int, T>& datas) { return false; }

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, std::map<int, FCharacterState>& datas)
{
	if (json.contains("character_list") && json["character_list"].is_array())
	{
		for (auto character : json["character_list"])
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


template<typename T>
bool CJsonController::ParseJson(const nlohmann::json& json, T& data) { return false; }

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, FConfig& data)
{
	if (json.contains("db_id"))
		data.DatabaseID = json["db_id"].get<std::string>();

	if (json.contains("db_url"))
		data.DatabaseURL = json["db_url"].get<std::string>();

	if (json.contains("api_key"))
		data.APIKey = json["api_key"].get<std::string>();

	if (json.contains("map_file_list") && json["map_file_list"].is_array())
	{
		for (auto mapFile : json["map_file_list"])
		{
			data.mapFileNameList.push_back(mapFile.get<std::string>());
		}
	}

	if (json.contains("character_file"))
		data.CharacterFileName = json["character_file"].get<std::string>();

	return true;
}

template<>
bool CJsonController::ParseJson(const nlohmann::json& json, FMapInfo& data)
{
	if (json.contains("index"))
		data.Index = json["index"].get<int>();

	if (json.contains("name"))
		data.Name = json["name"].get<std::string>();

	if (json.contains("difficulty_color_name"))
		data.DifficultyColorName = json["difficulty_color_name"].get<std::string>();

	if (json.contains("difficulty_rate"))
		data.DifficultyRate = json["difficulty_rate"].get<float>();

	if (json.contains("line_node_list") && json["line_node_list"].is_array())
	{
		for (auto jsonLineNode : json["line_node_list"])
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

	if (json.contains("item_type"))
		lineNode.ItemType = json["item_type"].get<int>();

	if (json.contains("obstacle_type"))
		lineNode.ObstacleType = json["obstacle_type"].get<int>();

	return lineNode;
}


/*
nlohmann::json CJsonController::ConvertToJson(const FUserInfo& userInfo)
{
	nlohmann::json jsonData;

	// 부모 데이터베이스 ID 설정
	jsonData[ATT_PARENT][ATT_DATABASE_ID] = CNotionDBController::GetInstance()->GetDatabaseID();
	jsonData[ATT_PARENT][ATT_TYPE] = ATT_DATABASE_ID;

	// 개별 속성 설정
	nlohmann::json ageProperty;
	ageProperty[ATT_NUMBER] = userInfo.Age;
	ageProperty[ATT_TYPE] = ATT_NUMBER;

	nlohmann::json marriedProperty;
	marriedProperty[ATT_CHECKBOX] = userInfo.bIsMarriage;
	marriedProperty[ATT_TYPE] = ATT_CHECKBOX;

	nlohmann::json nameProperty;
	nameProperty =
	{
		{ATT_TITLE, {
							{
								{"text", {{"content", userInfo.Name}}},
								{ATT_PLAIN_TEXT, userInfo.Name}
							}
		}}
	};

	//// origin.
	//{
	//    "title": [
	//        {
	//            "text": {
	//                "content": "Yeb"
	//            },
	//            "plain_text": "Yeb"
	//        }
	//    ]
	//}

	//// nlohmann json format in c++.
	//{
	//    {"title", {
	//            {
	//                {"text", {{"content", "Yeb"}}},
	//                {"plain_text", "Yeb"}
	//            }
	//    }}
	//};

	std::cout << nameProperty.dump(4) << std::endl;

	nlohmann::json phoneProperty;
	phoneProperty[PROP_PHONE_NUMBER] = userInfo.PhoneNumber;
	phoneProperty[ATT_TYPE] = PROP_PHONE_NUMBER;

	// 모든 속성을 합쳐서 properties에 추가
	jsonData[PROPERTIES] = {
		{PROP_AGE, ageProperty},
		{PROP_IS_MARRIAGE, marriedProperty},
		{PROP_NAME, nameProperty},
		{PROP_PHONE_NUMBER, phoneProperty}
	};

	return jsonData;
}
*/

/*
bool CJsonController::ParseUserInfo(const nlohmann::json& jsonData, std::map<std::string, FUserInfo>& users)
{
	users.clear();

	if (jsonData.contains(RESULT) && jsonData[RESULT].is_array())
	{
		for (const auto& item : jsonData[RESULT])
		{
			FUserInfo user;
			std::string pageID = item[ID];

			// name
			if (item.contains(PROPERTIES)
				&& item[PROPERTIES].contains(PROP_NAME))
			{
				auto titleArray = item[PROPERTIES][PROP_NAME][ATT_TITLE];
				if (!titleArray.empty() && titleArray[0].contains(ATT_PLAIN_TEXT))
				{
					user.Name = titleArray[0][ATT_PLAIN_TEXT].get<std::string>();
				}
			}

			// phone_number
			if (item[PROPERTIES].contains(PROP_PHONE_NUMBER)
				&& item[PROPERTIES][PROP_PHONE_NUMBER].contains(PROP_PHONE_NUMBER))
			{
				user.PhoneNumber = item[PROPERTIES][PROP_PHONE_NUMBER][PROP_PHONE_NUMBER].get<std::string>();
			}

			// age
			if (item[PROPERTIES].contains(PROP_AGE)
				&& item[PROPERTIES][PROP_AGE].contains(ATT_NUMBER))
			{
				user.Age = item[PROPERTIES][PROP_AGE][ATT_NUMBER].get<int>();
			}

			// is_marriage
			if (item[PROPERTIES].contains(PROP_IS_MARRIAGE)
				&& item[PROPERTIES][PROP_IS_MARRIAGE].contains(ATT_CHECKBOX))
			{
				user.bIsMarriage = item[PROPERTIES][PROP_IS_MARRIAGE][ATT_CHECKBOX].get<bool>();
			}

			users.emplace(std::pair<std::string, FUserInfo>(pageID, user));
		}
	}

	if (users.empty())
		return false;

	return true;
}

*/

