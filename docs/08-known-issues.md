# 08. 알려진 이슈

2026-11-24 코드 분석 중 발견한 문제입니다. 실행해서 재현한 것이 아니라 코드를 읽고 찾은 것이므로, 고치기 전에 한 번 재현해 보는 것을 권합니다. 심각도: **높음**(보안, 데이터 손상, 멈춤), **중간**(잘못된 동작), **낮음**(불일치, 잠재 위험).

## 보안

| 심각도 | 문제 | 위치 | 영향 | 권장 조치 |
| --- | --- | --- | --- | --- |
| 높음 | Notion API 키가 소스 주석에 그대로 적혀 git에 커밋됨(커밋 `1d43ce0`부터 HEAD까지). 원격 `origin`(GitHub)에도 올라가 있음 | `KDT2Framework/Include/Etc/JsonContainer.h`의 `FConfig` 위 주석 | 레포를 볼 수 있는 누구나 랭킹 DB를 읽고 쓰고 지울 수 있음 | Notion에서 키를 즉시 폐기하고 새로 발급. 주석에서 키 삭제. 필요하면 git 히스토리 정리 |
| 높음 | 같은 키를 공개 웹페이지 `config.json`(`api_key`)으로 배포 | GitHub Pages | 키를 바꿔도 다시 공개됨 | 클라이언트에 키를 주지 않는 구조로 변경(랭킹 등록을 중간 서버나 서버리스 함수로 대리) |

## 랭킹

| 심각도 | 문제 | 위치 | 영향 |
| --- | --- | --- | --- |
| 높음 | 등록할 때 `item_1`에 `data.Item_0`을 넣음 | `JsonController.cpp` `ConvertToJson(FUserRankInfo)` | 모든 랭킹 기록의 두 번째 아이템이 첫 번째 아이템으로 저장됨 |
| 중간 | 꼴찌 삭제를 카테고리별로 따로 실행. 맵 카테고리 꼴찌가 캐릭터 카테고리에서는 상위권일 수 있는데 그대로 삭제됨. 반대로 캐릭터 조건만 만족해도 맵 카테고리 꼴찌가 이미 지워졌을 수 있음 | `NotionDBController.cpp` `CheckForUpdate` | 순위권 기록이 이유 없이 사라질 수 있음 |
| 중간 | 조회 시 페이지네이션 없음. Notion 쿼리는 한 번에 최대 100개만 반환 | `ReadRecords` | 기록이 100개를 넘으면 일부가 안 보이고, 등록 판정도 틀어짐 |
| 중간 | HTTP 응답 코드를 확인하지 않음. 오류 응답 JSON도 정상 응답처럼 파싱 | `CCURL::SendRequest`, `CreateUserRecord` | 등록 실패해도 "New Record"로 표시될 수 있음 |
| 낮음 | 표시 개수 불일치: 로딩 문구는 "5위까지", 결과 화면 5위, 랭킹 화면 10위, 로비 3위, 보관 수는 `config.rank_count_by_category` | `TitleWidget`, `ResultWidget.h`, `RankWidget.h`, `ConstValues.h` | 기획 의도 확인 필요 |
| 낮음 | 캐릭터 카테고리 필터에 `else` 누락(`_subCategoryValue < 0`이면 조건이 두 번 검사됨) | `DataStorageManager.cpp` `GetArrayUserRankByCategory` | 현재는 하위 값이 항상 0 이상이라 영향 없음 |

## 멀티플레이 / 네트워크

| 심각도 | 문제 | 위치 | 영향 |
| --- | --- | --- | --- |
| 높음 | 접속을 끊는 코드가 없음. `CNetworkManager::Clear()`가 소멸자에서만 불림. 로비에서 "이전" 버튼으로 타이틀에 가도 소켓, 멀티 모드(`IsMultiplay`), 방 정보(`CMultiplayManager`)가 그대로 남음 | `LobbyWidget.cpp` 이전 버튼, `SceneLobby/InGame`의 `MSG_END` 처리 | 타이틀로 돌아온 뒤 싱글 플레이를 눌러도 멀티 모드로 동작. 다시 멀티 플레이를 누르면 기존 소켓 위에 또 접속 |
| 높음 | `Clear()`가 수신 스레드를 `join`한 **뒤에** 소켓을 닫음. 수신 스레드는 `recv`에서 막혀 있으므로 연결이 살아 있으면 `join`이 끝나지 않음 | `NetworkManager.cpp` `Clear()` | 멀티 접속 중 게임을 끄면 종료 시 멈출 수 있음. `closesocket`(또는 `shutdown`)을 `join`보다 먼저 호출해야 함 |
| 중간 | 접속 실패 처리 없음. `ConnetServer()`가 false를 반환해도 로딩 화면이 계속 떠 있음. 주석의 "Update()에서 재시도"는 구현되어 있지 않음 | `TitleWidget.cpp` `LoadProcess` | 잘못된 IP나 서버 실행 실패 시 타이틀이 로딩 상태로 멈춤 |
| 중간 | 거절 사유(`MSG_CONNECTED_REJECT`, 예: "Room is full.")를 화면에 표시하지 않음 | `TitleWidget::ProcessMessage` | 사용자는 왜 안 들어가지는지 모름 |
| 낮음 | `MSG_NEW_OWNER`에서 새 방장만 `isHost = true`로 바꾸고 이전 방장 플래그를 지우지 않음 | `MultiplayManager.cpp` `SetHostFromId` | 서버는 방장이 나갈 때만 위임하므로 지금은 문제가 드러나지 않음. 방장이 남아 있는 채로 위임하는 기능을 넣으면 왕관이 두 개 표시됨 |
| 중간 | 내 높이는 클라이언트가, 남이 보는 내 높이는 서버가 따로 계산. 서로 보정하지 않음 | `PlayerInGameObject`, 서버 | 프레임 차이나 지연으로 다른 사람 화면의 내 위치가 점점 어긋날 수 있음 |
| 낮음 | `MSG_ROOM_FULL_INFO` 파싱에서 바디 길이를 플레이어마다 확인하지 않고, `strcpy_s(nickname, ptr)`로 원본 길이 제한 없이 복사 | `NetworkManager.cpp` | 서버가 잘못된 패킷을 보내면 메모리 범위를 넘어 읽음 |
| 낮음 | 인게임 상태값 `Pause`, `Exit`, `End` 미사용. 인게임 중 나가기 기능 없음 | `GameInfo.h` `EGamePlayState` | 기능 공백 |
| 낮음 | 마우스(상승) 입력을 창 포커스와 관계없이 읽음. **실행해서 확인함**(2026-11-24): 한 PC에서 클라이언트 2개를 띄우면 한 창에서 누른 마우스가 두 창 모두에 들어가 두 플레이어가 같이 움직임 | `Common/Include/Scene/Input.cpp`: 창 모드에서 DirectInput 키보드/마우스를 `DISCL_BACKGROUND \| DISCL_NONEXCLUSIVE`로 설정 | 한 PC에서 멀티 테스트할 때만 영향. 실제 플레이(PC당 클라이언트 1개)에는 영향 없음 |

## 게임플레이

| 심각도 | 문제 | 위치 | 영향 |
| --- | --- | --- | --- |
| 중간 | Debug 빌드에서 시간당 HP 감소가 10배 | `PlayerInGameObject::UpdateDecreaseHp` | Debug로 밸런스를 보면 실제(Release)와 다름 |
| 낮음 | 함수 이름은 `DamagedPerDistance`인데 실제로는 경과 시간 비례. 부스트를 켜도 HP 소모가 같아 부스트에 페널티가 없음 | `IPlayerStatController` | 기획 의도 확인 필요 |
| 낮음 | `GetIsDeath()`가 HP > 0일 때 true를 반환(이름과 반대) | `IPlayerStatController` | 현재 호출하는 곳은 없음. 쓰기 전에 고쳐야 함 |
| 낮음 | 맵 노드의 `item_type`, `obstacle_type`을 읽지 않음 | `JsonController`, `LineGroupObject` | 데이터에 넣어도 효과 없음 |

## 코드 안정성

| 심각도 | 문제 | 위치 | 영향 |
| --- | --- | --- | --- |
| 중간 | 작업 스레드(`CTaskManager`, detach)가 위젯, 씬 플래그, 싱글턴 데이터를 락 없이 변경. 씬 전환 2초 대기 중 씬이 바뀌거나 지워지면 해제된 객체에 접근 | `SceneWidget::LoadScene`, `TitleWidget`, `PlayerInGameObject::OnPlayerDead` | 드물게 크래시 가능 |
| 중간 | 타이틀 로딩 중에 창 크기를 바꾸면 크래시. **실행해서 확인함**(2026-11-24, Debug): 실행 직후 타이틀 데이터를 받는 동안 외부에서 `MoveWindow`로 창 위치/크기를 지정하자 `FVector4D::operator=`(`Vector4D.h:50`)에서 접근 위반(0xC0000005). 창을 건드리지 않으면 재현되지 않았고, 크기를 유지한 채 위치만 옮기는 것은 문제없었음. 호출 스택은 확보하지 못함 | `Vector4D.h:50`, 위 작업 스레드 문제와 같은 원인으로 추정(WM_SIZE 처리와 로딩 스레드의 위젯 변경 경합) | 로딩 중 창 크기 조절/최대화 시 크래시 가능 |
| 중간 | `GetItemInfoDataByIndex`: 없는 양수 인덱스면 반환값 없이 끝나고(정의되지 않은 동작), 음수면 처리 중인 예외 없이 `throw;`를 호출해 프로그램 종료 | `DataStorageManager.h` | 잘못된 아이템 번호가 들어오면 크래시 |
| 낮음 | `FMapInfo::GetIconPath/GetIconName`이 0~2 외에는 반환값 없음 | `JsonContainer.h` | 맵을 4개 이상으로 늘리면 정의되지 않은 동작 |
| 낮음 | 파일 경로 기준이 섞여 있음(텍스처/폰트는 exe 폴더, 셰이더/인증서/서버 exe는 작업 폴더) | `Texture.cpp`, `GraphicShader.cpp`, `CURL.h`, `TitleWidget.cpp` | 다른 폴더에서 exe를 실행하면 셰이더 로드 실패 |
| 낮음 | `Common`(엔진)이 게임 코드(`KDT2Framework/Include`)를 include | `SceneManager.cpp`, `Scene.cpp`, `GameManager.cpp` | 엔진 재사용 불가, 빌드 의존성 꼬임 |
| 낮음 | `RankWidget.h`가 아직 CP949 인코딩 | `UI/UserWidget/RankWidget.h` | 다른 파일과 인코딩이 섞임. 편집 시 주의 |
