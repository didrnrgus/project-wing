#pragma once

#include "GameInfo.h"
#include "Etc/ConstString.h"
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
	//nlohmann::json ConvertToJson(const FUserInfo& userInfo);
	nlohmann::json ConvertToJson(const FLineNode& lineNodeInfo);
	//bool ParseUserInfo(const nlohmann::json& jsonData, std::map<std::string, FUserInfo>& users);
	bool ParseLineNodeInfo(const nlohmann::json& jsonData, std::map<std::string, FLineNode>& nodes);

	DECLARE_SINGLE(CJsonController);
};
