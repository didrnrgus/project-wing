#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

class CJsonController
{
public:
	template<typename T>
	nlohmann::json ConvertToJson(const T& data);

	template<typename T>
	bool ParseJson(const nlohmann::json& json, T& data);

	template<typename T>
	bool ParseJson(const nlohmann::json& json, std::map<int, T>& datas);

	template<typename T>
	bool ParseJson(const nlohmann::json& json, std::map<std::string, T>& datas);

	FLineNode ParseJsonFLineNode(const nlohmann::json& json);
	FSpriteSheetInfo ParseJsonFSpriteSheetInfo(const nlohmann::json& json);
	FUserRankInfo ParseJsonFUserRankInfo(const nlohmann::json& json);
	FColorInfo ParseJsonFColorInfo(const nlohmann::json& json);

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
