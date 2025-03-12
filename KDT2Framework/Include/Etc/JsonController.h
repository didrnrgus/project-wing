#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

namespace nlohmann {
  /*  template <>
    struct adl_serializer<FUserInfo> {
        static void to_json(json& j, const FUserInfo& userInfo) {
            j = nlohmann::json{
                {PROP_NAME, userInfo.Name},
                {PROP_PHONE_NUMBER, userInfo.PhoneNumber},
                {PROP_AGE, userInfo.Age},
                {PROP_IS_MARRIAGE, userInfo.bIsMarriage}
            };
        }

        static void from_json(const json& j, FUserInfo& userInfo) {
            j.at(PROP_NAME).get_to(userInfo.Name);
            j.at(PROP_PHONE_NUMBER).get_to(userInfo.PhoneNumber);
            j.at(PROP_AGE).get_to(userInfo.Age);
            j.at(PROP_IS_MARRIAGE).get_to(userInfo.bIsMarriage);
        }
    };*/
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
