#pragma once

#include "GameInfo.h"
 
#define HEADER_AUTHORIZATION "Authorization: Bearer "
#define HEADER_NOTION_VERSION "Notion-Version: 2022-06-28"
#define HEADER_CONTENT_TYPE "Content-Type: application/json"

#define NOTION_URL_FORMAT_DATABSE_ID_QUERY "https://api.notion.com/v1/databases/%s/query"
#define NOTION_URL_FORMAT_PAGE_ID "https://api.notion.com/v1/pages/%s"
#define NOTION_URL_PAGES "https://api.notion.com/v1/pages"

#define CACERT_PATH "./pem/cacert.pem"
#define WEBSERVER_PATH "https://didrnrgus.github.io/project-wing-webserver/"
#define CONFIG_PATH "config.json"

#define METHOD_POST "POST"
#define METHOD_PATCH "PATCH"
#define METHOD_DELETE "DELETE"
#define METHOD_GET "GET"

class CCURL
{
public:
    std::string SendRequest(const std::string& InURL, const std::string& InMethod, const std::string& InJsonData = "");

    DECLARE_SINGLE(CCURL);
};
