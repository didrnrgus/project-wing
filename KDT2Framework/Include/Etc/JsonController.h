#pragma once

#include "GameInfo.h"
#include "Etc/ConstValues.h"
#include "Etc/JsonContainer.h"

namespace nlohmann {
	template <>
	struct adl_serializer<FUserInfo>
	{
		static void to_json(json& j, const FUserInfo& userInfo)
		{
			j = nlohmann::json
			{
				{PROP_NAME, userInfo.Name}
				,{PROP_MAP, userInfo.Map}
				,{PROP_DISTANCE, userInfo.Distance}
				,{PROP_TIME, userInfo.Time}
			};
		}

		static void from_json(const json& j, FUserInfo& userInfo)
		{
			j.at(PROP_NAME).get_to(userInfo.Name);
			j.at(PROP_MAP).get_to(userInfo.Map);
			j.at(PROP_DISTANCE).get_to(userInfo.Distance);
			j.at(PROP_TIME).get_to(userInfo.Time);
		}
	};

	template<>
	struct adl_serializer<FLineNode>
	{
		static void to_json(json& j, const FLineNode& lineNode)
		{
			j = nlohmann::json
			{
				{PROP_TOP_Y_POS, lineNode.TopYPos}
				,{PROP_BOTTOM_Y_POS, lineNode.BottomYPos}
				,{PROP_ITEM_TYPE, lineNode.ItemType}
				,{PROP_OBSTACLE_TYPE, lineNode.ObstacleType}
			};
		}

		static void from_json(const json& j, FLineNode& lineNode)
		{
			j.at(PROP_NAME).get_to(lineNode.TopYPos);
			j.at(PROP_BOTTOM_Y_POS).get_to(lineNode.BottomYPos);
			j.at(PROP_ITEM_TYPE).get_to(lineNode.ItemType);
			j.at(PROP_OBSTACLE_TYPE).get_to(lineNode.ObstacleType);
		}
	};
}


class CJsonController
{
public:
	template<typename T>
	nlohmann::json ConvertToJson(const T& data);

	template<typename T>
	bool ParseJson(const nlohmann::json& json, T& data);

	template<typename T>
	bool ParseJson(const nlohmann::json& json, std::map<int,T>& datas);

	template<typename T>
	bool ParseJson(const nlohmann::json& json, std::map<std::string, T>& datas);

	FLineNode ParseJsonFLineNode(const nlohmann::json& json);

	FSpriteSheetInfo ParseJsonFSpriteSheetInfo(const nlohmann::json& json);

	std::string ReadJsonFile(const std::string& filePath) 
	{
		std::ifstream file(filePath);  // 파일 열기
		if (!file.is_open()) 
		{
			throw std::runtime_error("파일을 열 수 없습니다: " + filePath);
		}

		std::stringstream buffer;
		buffer << file.rdbuf();  // 파일 내용을 스트림에 복사
		return buffer.str();  // 스트림의 내용을 std::string으로 반환
	}
private:
	DECLARE_SINGLE(CJsonController);
};
