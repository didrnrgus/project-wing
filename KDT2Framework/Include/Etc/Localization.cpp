#include "Localization.h"
#include "GameInfo.h"
#include <atomic>
#include <cassert>

namespace
{
	struct FLocEntry
	{
		ELocKey::Type key;
		const wchar_t* text[ELanguage::End];	// { English, Korean }
	};

	const FLocEntry gLocTable[] =
	{
		// Win32 메뉴 / 팝업
		{ ELocKey::MenuNickname,		{ L"Nickname",					L"닉네임 설정" } },
		{ ELocKey::MenuNicknameInput,	{ L"Enter Nickname",			L"닉네임 입력" } },
		{ ELocKey::MenuMultiplay,		{ L"Multiplayer",				L"멀티플레이" } },
		{ ELocKey::MenuJoinRoom,		{ L"Join Another Room",			L"다른 방에 참가하기" } },
		{ ELocKey::MenuHostRoom,		{ L"Host My Own Room",			L"직접 방 만들기" } },
		{ ELocKey::MenuHostIP,			{ L"Host IP",					L"방장 IP 설정" } },
		{ ELocKey::MenuHostIPInput,		{ L"Enter IP Address",			L"IP 주소 입력" } },
		{ ELocKey::MenuLanguage,		{ L"Language",					L"언어(Language)" } },
		{ ELocKey::PopupOK,				{ L"OK",						L"확인" } },
		{ ELocKey::PopupCancel,			{ L"Cancel",					L"취소" } },
		{ ELocKey::PopupNicknameHint,	{ L"* Letters and numbers only",	L"* 영문, 숫자만 입력 가능" } },
		{ ELocKey::WindowTitleFormat,	{ L"ProjectWing | Nickname: %s | Host IP: %s",	L"ProjectWing | 닉네임: %s | 방장 IP: %s" } },

		// 로딩
		{ ELocKey::LoadingWord,			{ L"Loading",					L"로딩 중" } },
		{ ELocKey::LoadingWait,			{ L"Please wait. Loading... 🙇🏻",			L"잠시만 기다려 주세요. 로딩 중입니다. 🙇🏻" } },
		{ ELocKey::LoadingToTitle,		{ L"Moving to the title screen. 🏃🏻",		L"타이틀 화면으로 이동 중입니다. 🏃🏻" } },
		{ ELocKey::LoadingToLobby,		{ L"Moving to the lobby. 🏃🏻",			L"로비 화면으로 이동 중입니다. 🏃🏻" } },
		{ ELocKey::LoadingToInGame,		{ L"Moving to the game screen. 🏃🏻",		L"게임 화면으로 이동 중입니다. 🏃🏻" } },
		{ ELocKey::LoadingToResult,		{ L"Moving to the results screen. 🏃🏻",	L"결과 화면으로 이동 중입니다. 🏃🏻" } },
		{ ELocKey::LoadingToRank,		{ L"Moving to the ranking screen. 🏃🏻",	L"랭킹 화면으로 이동 중입니다. 🏃🏻" } },

		// 타이틀
		{ ELocKey::TitleSinglePlay,		{ L"SinglePlay",				L"싱글 플레이" } },
		{ ELocKey::TitleMultiPlay,		{ L"MultiPlay",					L"멀티 플레이" } },
		{ ELocKey::TitleRank,			{ L"Rank",						L"랭킹" } },
		{ ELocKey::TitleExit,			{ L"Exit",						L"종료" } },
		{ ELocKey::TitleLoadConfig,		{ L"Loading config data.\n👨🏻‍💻",
										  L"설정 데이터를 불러오는 중입니다.\n👨🏻‍💻" } },
		{ ELocKey::TitleLoadMap,		{ L"Loading map data.\nThere are three maps, one for each difficulty.\n🏜️,🏖️,🏞️",
										  L"맵 데이터를 불러오는 중입니다.\n맵은 난이도별로 세 가지가 있어요.\n🏜️,🏖️,🏞️" } },
		{ ELocKey::TitleLoadCharacter,	{ L"Loading character data.\nThere are five characters.\n👹.👺.💀.👻.👽",
										  L"캐릭터 데이터를 불러오는 중입니다.\n캐릭터는 다섯 가지가 있어요.\n👹.👺.💀.👻.👽" } },
		{ ELocKey::TitleLoadItem,		{ L"Loading item data.\nThere are four items, each with a passive stat bonus.",
										  L"아이템 데이터를 불러오는 중입니다.\n아이템은 네 가지이고, 모두 능력치 패시브 효과예요." } },
		{ ELocKey::TitleLoadStat,		{ L"Loading stat data.\nThere are four stats: HP, Speed, Dex, and Def.",
										  L"능력치 데이터를 불러오는 중입니다.\n능력치는 체력, 속도, 민첩, 방어 네 가지가 있어요." } },
		{ ELocKey::TitleLoadColor,		{ L"Loading color data.\n🎨✖️30",
										  L"색상 데이터를 불러오는 중입니다.\n🎨✖️30" } },
		{ ELocKey::TitleLoadRank,		{ L"Loading ranking data. ✨🎉🎊🎉🎊✨\nShowing the top 5 by distance for each map and character.",
										  L"랭킹 데이터를 불러오는 중입니다. ✨🎉🎊🎉🎊✨\n랭킹은 맵별, 캐릭터별 거리 순으로 5위까지 보여 줘요." } },
		{ ELocKey::TitleLaunchServer,	{ L"Starting the game server.\nYou're the host this time!🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻",
										  L"게임 서버를 실행하는 중입니다.\n이번 판은 내가 호스트예요!🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻🧑🏻‍💻" } },
		{ ELocKey::TitleConnectHost,	{ L"Connecting to the host server.\nDid you check the host info in the menu bar?🛜🛜🛜🛜🛜",
										  L"호스트 서버에 접속하는 중입니다.\n메뉴 바에서 호스트 정보를 확인하셨나요?🛜🛜🛜🛜🛜" } },

		// 인게임
		{ ELocKey::InGameReady,			{ L"READY~",					L"준비~" } },
		{ ELocKey::InGameStart,			{ L"START!!",					L"시작!!" } },
		{ ELocKey::InGameDistFormat,	{ L"Dist: %.0fm",				L"거리: %.0fm" } },

		// 로비
		{ ELocKey::LobbyPlayerEmpty,	{ L"Player_Empty",				L"빈 자리" } },
		{ ELocKey::LobbyPlayerTitle,	{ L"PLAYER TITLE",				L"플레이어 목록" } },
		{ ELocKey::LobbyRankByCharacter,{ L"Rank by Char",				L"캐릭터별 랭킹" } },
		{ ELocKey::LobbyRankByMap,		{ L"Rank by Map",				L"맵별 랭킹" } },

		// 정보 라벨
		{ ELocKey::LabelName,			{ L"Name",						L"이름" } },
		{ ELocKey::LabelColor,			{ L"Color",						L"색상" } },
		{ ELocKey::LabelDiffRate,		{ L"DiffRate",					L"난이도" } },
		{ ELocKey::LabelColDmg,			{ L"ColDmg",					L"충돌 피해" } },
		{ ELocKey::LabelHP,				{ L"HP",						L"체력" } },
		{ ELocKey::LabelSpeed,			{ L"Speed",						L"속도" } },
		{ ELocKey::LabelDex,			{ L"Dex",						L"민첩" } },
		{ ELocKey::LabelDef,			{ L"Def",						L"방어" } },
		{ ELocKey::LabelStun,			{ L"Stun",						L"기절" } },
		{ ELocKey::LabelSize,			{ L"Size",						L"크기" } },
		{ ELocKey::LabelDesc,			{ L"Desc",						L"설명" } },
		{ ELocKey::LabelStatType,		{ L"StatType",					L"능력치" } },
		{ ELocKey::LabelAddVal,			{ L"AddVal",					L"증가량" } },

		// 랭킹 / 결과
		{ ELocKey::RankTitle,			{ L"🏴🏴🏴RANKING🏴🏴🏴",		L"🏴🏴🏴랭킹🏴🏴🏴" } },
		{ ELocKey::ResultTitle,			{ L"RESULT",					L"결과" } },
		{ ELocKey::ResultNewRecord,		{ L"[NEW RECORD!!]",			L"[신기록!!]" } },
		{ ELocKey::TabMap,				{ L"Map",						L"맵" } },
		{ ELocKey::TabCharacter,		{ L"Character",					L"캐릭터" } },
		{ ELocKey::HeaderName,			{ L"NAME",						L"이름" } },
		{ ELocKey::HeaderMap,			{ L"MAP",						L"맵" } },
		{ ELocKey::HeaderCharacter,		{ L"CHARACTER",					L"캐릭터" } },
		{ ELocKey::HeaderDistance,		{ L"DISTANCE",					L"거리" } },
		{ ELocKey::HeaderItem,			{ L"ITEM",						L"아이템" } },
	};

	static_assert(sizeof(gLocTable) / sizeof(gLocTable[0]) == ELocKey::End,
		"gLocTable 항목 수가 ELocKey 와 다릅니다. Localization.cpp 에 문구를 추가하세요.");

	// key 순서와 무관하게 조회할 수 있도록 key -> entry 인덱스 테이블을 만든다.
	struct FLocLookup
	{
		const FLocEntry* entries[ELocKey::End] = {};

		FLocLookup()
		{
			for (const FLocEntry& entry : gLocTable)
			{
				assert(entries[entry.key] == nullptr && "gLocTable 에 중복된 키가 있습니다.");
				entries[entry.key] = &entry;
			}
		}
	};

	const FLocLookup& GetLookup()
	{
		static const FLocLookup lookup;
		return lookup;
	}

	std::atomic<int> gLanguage(ELanguage::English);

	const wchar_t* LANGUAGE_CODE[ELanguage::End] = { L"en", L"ko" };

	// 실행 파일과 같은 폴더의 Language.ini
	std::wstring GetSettingPath()
	{
		wchar_t path[MAX_PATH] = {};
		GetModuleFileNameW(nullptr, path, MAX_PATH);
		std::wstring result(path);
		size_t pos = result.find_last_of(L"\\/");
		if (pos != std::wstring::npos)
			result.erase(pos + 1);
		return result + L"Language.ini";
	}
}

void CLocalization::Init()
{
	wchar_t code[16] = {};
	GetPrivateProfileStringW(L"Settings", L"Language", L"", code, 16, GetSettingPath().c_str());

	for (int i = 0; i < ELanguage::End; i++)
	{
		if (lstrcmpiW(code, LANGUAGE_CODE[i]) == 0)
		{
			gLanguage = i;
			return;
		}
	}

	// 저장된 설정이 없으면 OS UI 언어 기준 (한국어면 한국어, 그 외는 영어)
	gLanguage = (PRIMARYLANGID(GetUserDefaultUILanguage()) == LANG_KOREAN)
		? ELanguage::Korean : ELanguage::English;
}

ELanguage::Type CLocalization::GetLanguage()
{
	return static_cast<ELanguage::Type>(gLanguage.load());
}

void CLocalization::SetLanguage(ELanguage::Type _language)
{
	if (_language < 0 || _language >= ELanguage::End)
		return;

	gLanguage = _language;
	WritePrivateProfileStringW(L"Settings", L"Language", LANGUAGE_CODE[_language], GetSettingPath().c_str());
}

const wchar_t* CLocalization::Get(ELocKey::Type _key)
{
	if (_key < 0 || _key >= ELocKey::End)
		return L"";

	const FLocEntry* entry = GetLookup().entries[_key];
	if (entry == nullptr)
		return L"";

	const wchar_t* text = entry->text[gLanguage.load()];
	return text ? text : entry->text[ELanguage::English];
}
