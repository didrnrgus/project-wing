#pragma once

#include "GameInfo.h"

class CCURL
{
private:
    const std::string HEADER_AUTHORIZATION = "Authorization: ";
    const std::string HEADER_NOTION_VERSION = "Notion-Version: ";
    const std::string HEADER_CONTENT_TYPE = "Content-Type: application/json";

public:
    const std::string CACERT_PATH = "./pem/cacert.pem";
    const std::string METHOD_POST = "POST";
    const std::string METHOD_PATCH = "PATCH";
    const std::string METHOD_DELETE = "DELETE";
    const std::string METHOD_GET = "GET";
    
public:
    // GET Àº InJsonData »ý¶ô.
    std::string SendRequest(const std::string& InURL, const std::string& InMethod, const std::string& InJsonData = "");

    DECLARE_SINGLE(CCURL);
};
