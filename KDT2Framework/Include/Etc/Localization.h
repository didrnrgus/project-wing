#pragma once

namespace ELanguage
{
	enum Type
	{
		English = 0,
		Korean,
		End
	};
}

// 키를 추가하면 Localization.cpp 의 gLocTable 에도 같은 키로 영어/한국어 문구를 추가해야 한다.
namespace ELocKey
{
	enum Type
	{
		// Win32 메뉴 / 팝업
		MenuNickname = 0,
		MenuNicknameInput,
		MenuMultiplay,
		MenuJoinRoom,
		MenuHostRoom,
		MenuHostIP,
		MenuHostIPInput,
		MenuLanguage,
		PopupOK,
		PopupCancel,
		PopupNicknameHint,
		WindowTitleFormat,		// %s: 닉네임, %s: 방장 IP

		// 로딩
		LoadingWord,
		LoadingWait,
		LoadingToTitle,
		LoadingToLobby,
		LoadingToInGame,
		LoadingToResult,
		LoadingToRank,

		// 타이틀
		TitleSinglePlay,
		TitleMultiPlay,
		TitleRank,
		TitleExit,
		TitleLoadConfig,
		TitleLoadMap,
		TitleLoadCharacter,
		TitleLoadItem,
		TitleLoadStat,
		TitleLoadColor,
		TitleLoadRank,
		TitleLaunchServer,
		TitleConnectHost,

		// 인게임
		InGameReady,
		InGameStart,
		InGameDistFormat,		// %.0f: 거리

		// 로비
		LobbyPlayerEmpty,
		LobbyPlayerTitle,
		LobbyRankByCharacter,
		LobbyRankByMap,

		// 정보 라벨 (맵 / 캐릭터 / 아이템)
		LabelName,
		LabelColor,
		LabelDiffRate,
		LabelColDmg,
		LabelHP,
		LabelSpeed,
		LabelDex,
		LabelDef,
		LabelStun,
		LabelSize,
		LabelDesc,
		LabelStatType,
		LabelAddVal,

		// 랭킹 / 결과
		RankTitle,
		ResultTitle,
		ResultNewRecord,
		TabMap,
		TabCharacter,
		HeaderName,
		HeaderMap,
		HeaderCharacter,
		HeaderDistance,
		HeaderItem,

		End
	};
}

class CLocalization
{
public:
	// 저장된 언어 설정을 읽고, 없으면 OS UI 언어로 결정한다.
	static void Init();
	static ELanguage::Type GetLanguage();
	// 언어를 바꾸고 설정 파일에 저장한다. 게임 UI 는 다음 화면 전환부터 반영된다.
	static void SetLanguage(ELanguage::Type _language);
	static const wchar_t* Get(ELocKey::Type _key);
};

#define LOC(key) CLocalization::Get(ELocKey::key)
