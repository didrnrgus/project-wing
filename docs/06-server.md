# 06. 소켓 서버

멀티플레이 서버(`project-wing-socket-server`)의 소스는 `Server/Include/`에 있고 `ProjectWing.sln`에 포함됩니다. 빌드하면 `KDT2Framework/Bin/Server/{Debug|Release}/`에 바로 출력됩니다. 방장 클라이언트가 이 exe를 자식 프로세스로 띄우고, 참가자 전원(방장 포함)이 TCP 12345로 접속합니다.

> 원본 서버 소스는 유실되어, 클라이언트 코드가 주고받는 메시지와 원본 exe의 문자열·실행 측정을 기준으로 서버를 다시 작성했습니다(2026-11-24). 원본과 일부러 다르게 한 부분은 아래와 같습니다.
>
> | 항목 | 새 서버 | 원본 exe 측정값 |
> | --- | --- | --- |
> | 구조 | `select()` 단일 스레드, 50ms 게임 틱 | - |
> | 카운트다운 (`START_ACK` → `COUNTDOWN_FINISHED`) | 7.5초 (`COUNTDOWN_SEC`) | 5.0초 |
> | 거리/높이/HP 동기화 | 플레이어마다 50ms(20Hz)에 한 번 | 약 180Hz |
> | 게임 중 접속 | `CONNECTED_REJECT("Game is in progress.")` | 거절하지 않음(이후 카운트다운이 오지 않는 문제) |
> | `START` 거절(`START_ACK 0`) | 방장에게만 | 전원 |
> | `JOIN` 닉네임 | 널 없이 길이만큼 | 64바이트 패딩 |
> | 게임 종료 후 | 전원 Ready 해제, 캐릭터 0, 아이템 -1 (클라 `ResetPlayerAfterInGame`과 같음) | - |
> | 하트비트 타임아웃 | 10초 | - |
> | 게임 데이터 | WinHTTP로 GitHub Pages에서 받음 (libcurl, pem 불필요). 실패하면 기본값 | libcurl |
>
> 상수는 `Server/Include/GameServer.h` 맨 위에 모여 있습니다.

## 1. 소스와 빌드

| 파일 | 내용 |
| --- | --- |
| `Server/Include/Protocol.h` | `ClientMessage`, `ServerMessage` enum, `MessageHeader`(12바이트), 포트/인원/닉네임 길이 상수. 클라이언트 `NetworkManager.h`와 같아야 함 |
| `Server/Include/GameServer.h/.cpp` | `CGameServer`: 접속 관리, 메시지 처리, 게임 틱. 서버 상수(`TICK_INTERVAL_SEC`, `COUNTDOWN_SEC`, `PROTECTION_SEC` 등)는 헤더 맨 위 |
| `Server/Include/GameData.h/.cpp` | `CGameData`: GitHub Pages JSON(config, map, character, item)을 WinHTTP로 받아 서버 계산용 값만 보관 |
| `Server/Include/main.cpp` | 진입점. 인자로 포트를 받을 수 있음(기본 12345). Ctrl+C로 종료 |
| `Server/Include/project-wing-socket-server.vcxproj` | x64 Debug/Release. `ProjectWing.sln`에 포함 |

- 솔루션을 빌드하면 서버도 같이 빌드되어 `KDT2Framework/Bin/Server/{Debug|Release}/project-wing-socket-server.exe`로 바로 출력됩니다. 중간 파일은 `Server/BinObj/`(gitignore).
- `.gitignore`가 모든 `*.exe`를 제외하지만 `KDT2Framework/Bin/Server/**/*.exe`는 예외로 커밋합니다. 서버를 고쳤으면 Debug, Release를 둘 다 빌드해 두 exe를 함께 커밋해야 클라이언트 빌드 구성과 맞습니다. GitHub Release zip에 들어가는 서버 exe는 커밋된 exe가 아니라 CI에서 새로 빌드한 것입니다([09-release.md](09-release.md)).
- 외부 의존: nlohmann/json(`KDT2Framework/Include/json-develop`을 include 경로로 공유), `ws2_32.lib`, `winhttp.lib`. libcurl과 `pem/`은 필요 없습니다. 폴더에 남아 있는 `libcurl-x64.*`, `pem/`은 원본 서버용입니다.
- Release는 정적 CRT(`/MT`)라 VC 재배포 패키지 없이 exe 하나로 실행됩니다.
- 수동 실행: `project-wing-socket-server.exe [port]`. 클라이언트 없이 띄워 두고 프로토콜 테스트를 돌릴 때 씁니다.

## 2. 실행 방식 (클라이언트 쪽)

`CTitleWidget::LoadProcess()` → `CProcessManager::LaunchProcess()`

| 항목 | 동작 |
| --- | --- |
| 실행 조건 | 타이틀에서 "멀티 플레이"를 눌렀고, 메뉴 "다른 방에 참가하기"가 꺼져 있을 때 |
| 경로 | 작업 폴더 기준 `../Bin/Server/Debug/...`(Debug 클라이언트) 또는 `../Bin/Server/Release/...` |
| 창 | Debug 클라이언트는 새 콘솔 창(`CREATE_NEW_CONSOLE`), Release는 창 없음(`CREATE_NO_WINDOW`) |
| 작업 폴더 | 지정 안 함 → 클라이언트의 작업 폴더를 물려받음 |
| 수명 | Job Object에 `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`로 묶음. 클라이언트 프로세스가 끝나면 서버도 강제 종료 |
| 중복 실행 | 같은 클라이언트에서 다시 실행하면 이전 서버를 `TerminateProcess`로 끄고 새로 띄움 |

- 서버의 작업 폴더는 클라이언트 작업 폴더를 물려받지만, 새 서버는 상대 경로 파일을 읽지 않으므로 영향이 없습니다.
- 서버는 listen을 먼저 시작한 뒤 게임 데이터를 받습니다. 그동안 들어온 클라이언트 접속은 backlog에서 기다렸다가 처리되므로, 클라이언트가 서버를 띄우자마자 접속해도 실패하지 않습니다.
- 방장이 게임을 끄면 서버가 함께 죽으므로 다른 참가자는 모두 끊깁니다(`MSG_END` → 타이틀). 방장 위임(`MSG_NEW_OWNER`)은 서버 프로세스를 띄운 PC와 무관하게 "방장 권한"만 넘깁니다.
- 다른 PC에서 접속하려면 방장 PC의 방화벽에서 TCP 12345 인바운드를 허용해야 합니다.

## 3. 서버 구조

- **단일 스레드 `select()` 루프**(`CGameServer::Run`). 루프 한 번에 접속 수락 → 수신/메시지 처리 → 50ms가 지났으면 게임 틱과 타임아웃 검사 → 끊을 클라이언트 정리 → 송신 버퍼 flush. 공유 상태가 한 스레드에만 있으므로 락이 없습니다.
- 송신은 클라이언트별 버퍼에 모았다가 루프 끝에 한 번에 보냅니다(`TCP_NODELAY`, 송신 타임아웃 3초). 실패하면 그 클라이언트를 다음 루프에서 정리합니다.
- 수신은 클라이언트별 버퍼에 쌓고 헤더(12바이트) + 바디가 다 모이면 처리합니다. `bodyLen`이 0~4096 밖이면 연결을 끊습니다.
- 방은 하나이고 상태는 `Lobby` → `Countdown` → `Playing` → (전원 사망) `Lobby`입니다(`ERoomPhase`).
- 클라이언트 상태 `FClient`: 소켓, id, 닉네임, 로비 상태(ready, 캐릭터, 아이템 3칸), 인게임 상태(HP, 스탯, 거리, 높이, 상승/부스트 입력, 스턴/보호 타이머, 사망).

## 4. 메시지 처리 (실제 동작)

| 단계 | 서버 동작 | 메시지 |
| --- | --- | --- |
| 접속 | 수락 즉시 id(1부터 증가)를 발급하고 `WHO_ARE_YOU_REQ(id)` 전송. `WHO_ARE_YOU_RES`가 와야 방에 들어온 것으로 칩니다 | `WHO_ARE_YOU_REQ/RES` |
| 입장 | `CONNECTED(id)` → `ROOM_FULL_INFO`(나 포함 전원) 순서로 보내고, 기존 인원에게 `JOIN(id + 닉네임, 널 없음)`. 첫 입장자가 방장 | `CONNECTED`, `ROOM_FULL_INFO`, `JOIN` |
| 거절 | 게임 중이면 `"Game is in progress."`, 5명이 차 있으면 `"Room is full."`. 거절 메시지를 보낸 뒤 송신만 닫고(FIN), 클라이언트가 끊거나 2초가 지나면 정리. 그동안 오는 하트비트는 읽어서 버림 | `CONNECTED_REJECT` |
| 로비 중계 | 캐릭터, 아이템(슬롯 0~2만), Ready/Unready(방장 제외), 맵(방장만)을 저장하고 **보낸 사람 포함 전원**에게 senderId를 붙여 중계. 로비 상태일 때만 받음 | `PICK_*`, `READY`, `UNREADY` |
| 닉네임 변경 | 언제든 받음. 63자로 자르고 널 없이 전원에게 중계 | `CHANGE_NICKNAME` |
| 시작 | 방장의 `START`만 받음. 방장 외 전원 Ready면 스탯을 계산하고 `START_ACK(1)`을 전원에게, 아니면 `START_ACK(0)`을 방장에게만 | `START` → `START_ACK` |
| 카운트다운 | `START_ACK` 후 7.5초 뒤 전원에게 알림. 클라이언트 씬 로딩(약 2초) + "Ready, 3, 2, 1, Start"(5초)에 맞춘 값 | `COUNTDOWN_FINISHED` |
| 입력 | `MOVE_UP/DOWN`, `BOOST_ON/OFF`는 플레이어 상태만 바꾸고 중계하지 않음 | |
| 게임 틱(50ms) | 살아 있는 플레이어마다 아래 공식으로 갱신하고 `PLAYER_DISTANCE`, `PLAYER_HEIGHT`, `TAKEN_DAMAGE(현재 HP)`를 전원에게 | |
| 충돌 | 진행 중이고 살아 있고 스턴/보호 중이 아닐 때만 처리. HP가 이미 0이면 사망. 아니면 HP −= max(0, 데미지 − Def), 스턴 시작, `TAKEN_DAMAGE`와 `TAKEN_STUN`을 전원에게. HP가 0이 되면 사망 | `TAKE_DAMAGE` |
| 사망 | 마지막 거리와 HP 0을 보내고 `PLAYER_DEAD`(senderId = 죽은 사람)를 전원에게 | `PLAYER_DEAD` |
| 장애물 | 맵 `obstacle_interval_time`(최소 0.2초)마다 크기 100~149, 회전 0~359, 높이 −360~359를 정해 전원에게 같은 값 | `OBSTACLE` |
| 종료 | 살아 있는 플레이어가 없으면 `GAME_OVER("All players dead. Game over.\0")`. 이어서 전원 로비 상태로 초기화: Ready 해제, 캐릭터 0, 아이템 −1(클라이언트 `ResetPlayerAfterInGame`과 같음) | `GAME_OVER` |
| 하트비트 | 받으면 바로 `HEARTBEAT_ACK`. 10초 동안 아무것도 안 오면 끊음 | `HEARTBEAT` |
| 퇴장 | `DISCONNECT(id)`를 남은 전원에게. 방장이었으면 가장 먼저 들어온 사람을 새 방장으로 정하고 `NEW_OWNER`. 게임 중이면 게임 종료 조건을 다시 검사. 아무도 없으면 방 초기화(맵 0) | `DISCONNECT`, `NEW_OWNER` |

### 게임 틱 공식

| 상태 | 처리 |
| --- | --- |
| 스턴 중 (`stun_duration`) | 거리, 높이, HP 모두 멈춤. 끝나면 보호 시작 |
| 보호 중 (2초) | 거리와 높이는 갱신, HP 감소 없음, 충돌 무시 |
| 정상 | 거리 += Speed × dt × 0.01 × (부스트 2 / 아니면 1), 높이 += (상승 ? +1 : −1) × Dex × dt (−360~360으로 제한), HP −= dt × max(0, 1 − Def/100). HP가 0이 되면 사망 |

스탯은 시작할 때 캐릭터 기본값 + 장착 아이템 `add_value`로 계산합니다(HP 아이템은 시작 HP에 더함). 충돌 데미지는 클라이언트가 보낸 값(맵 `collision_damage`)을 씁니다.

### 원본 서버 exe에서 확인한 것

원본 exe의 문자열과 실제 실행 측정으로 확인한 내용이며, 새 서버도 같은 동작을 따릅니다. 로그 문구 일부(`_statInfo.HP:`, `DamagedPerDistance Dead id:`, `MSG_TAKE_DAMAGE Dead########`, `readyFlag`)도 같게 남겼습니다.

| 기능 | 근거 |
| --- | --- |
| 인원 초과 거절 사유 | `Room is full.` |
| 게임 데이터 다운로드 | `https://didrnrgus.github.io/project-wing-webserver/`, `config.json`, `map_file_list`, `character_list` |
| 맵 설정 사용 | `collision_damage`, `obstacle_interval_time` |
| 시간당 HP 감소와 충돌로 사망 | `DamagedPerDistance Dead id: `, `ClientMessage::MSG_TAKE_DAMAGE Dead######## id: ` |
| 전원 사망 시 종료 | `All players dead. Game over.` |
| 카운트다운 | 측정값 5.0초 (새 서버는 7.5초) |

### 규칙을 서버와 클라이언트가 같이 들고 있는 부분

멀티에서는 아래 값이 서버 계산 결과로 덮어쓰이므로, 기획 수치나 공식을 바꿀 때 서버도 같이 바꿔야 합니다.

| 규칙 | 클라이언트 위치(싱글용) | 서버 |
| --- | --- | --- |
| 거리 = Speed × dt × 0.01 × 부스트(2배) | `CPlayerInGameObject::UpdateDistance` | `CGameServer::Tick` |
| 시간당 HP 감소 = dt × (1 − Def/100) | `IPlayerStatController::DamagedPerDistance` | `CGameServer::Tick` |
| 충돌 데미지 = 데미지 − Def | `IPlayerStatController::Damaged` | `CGameServer::HandleTakeDamage` |
| 스턴 시간, 보호 2초 | `IPlayerStatController::ReleaseStun/ReleaseProtection` | `Tick`의 스턴/보호 타이머, `PROTECTION_SEC` |
| 아이템 스탯 보너스, HP 아이템 | `CPlayerInGameObject::Init` | `CGameServer::InitPlayerStat` |
| 장애물 크기 100~149, 회전 0~359, 높이 화면 안 | `CObstacleGroupObject::Update` | `Tick`의 장애물 생성 |
| 상하 이동 = Dex px/초, 화면 안(−360~360) | `CPlayerInGameObject::MoveUpHold/MoveDown` | `Tick`의 높이 계산 |
| 게임 후 캐릭터/아이템 초기화 | `CMultiplayManager::ResetPlayerAfterInGame` | `FClient::ResetLobbyState` |

## 5. 검증

테스트 프로젝트는 없습니다. 2026-11-24에 아래 두 방법으로 확인했습니다.

- **프로토콜 테스트**: Python 테스트 클라이언트 2~3개를 서버에 직접 붙여 34개 항목을 확인했습니다. 대상은 접속 순서, 81바이트 방 정보, 로비 중계, 방장 아닌 사람의 맵 변경 무시, 시작 거절/승인, 게임 중 접속 거절, 카운트다운, 거리/높이/HP 동기화, 부스트, 스턴 중 정지와 데미지 무시, 장애물, 사망, 게임 오버, 게임 후 재시작, 퇴장과 방장 위임입니다. 전부 통과했습니다.
- **실제 클라이언트 플레이**: Debug 클라이언트 2개(방장 + "다른 방에 참가하기")로 두 판을 끝까지 진행했습니다. 로비 동기화, 시작, 카운트다운, 같은 장애물, 충돌 데미지(맵 30 − Def), 사망, 결과 화면이 양쪽에서 같았고, 서버 로그의 HP와 사망 거리가 화면 값과 일치했습니다.

## 6. 서버를 고칠 때 체크리스트

- [ ] `Server/Include/Protocol.h`의 `ClientMessage`, `ServerMessage` enum 순서와 값이 클라이언트(`NetworkManager.h`)와 같은가. 클라이언트의 `ServerMessage::MSG_END`(27)는 클라이언트 내부용이므로 서버가 이 값을 다른 의미로 쓰지 않는가
- [ ] `MSG_ROOM_FULL_INFO` 플레이어 레코드가 81바이트(int, bool 1바이트, int, int×3, char[64]) 패킹을 지키는가
- [ ] 닉네임: `WHO_ARE_YOU_RES`는 널 포함, `CHANGE_NICKNAME`은 널 없이 옴. `JOIN`은 바디 나머지 전체를 닉네임으로 읽음
- [ ] 게임 데이터 JSON을 클라이언트와 같은 웹서버에서 받는가. 스키마를 바꾸면 양쪽 파서를 같이 바꿨는가
- [ ] 클라이언트가 `ROOM_FULL_INFO`를 받을 때마다 플레이어를 추가만 하므로, 접속할 때 한 번만 보내는가. `CONNECTED`를 그보다 먼저 보내는가(방장 판정)
- [ ] 게임 규칙을 바꿨다면 위 "같이 들고 있는 부분" 표의 클라이언트 쪽도 같이 바꿨는가
- [ ] Debug, Release를 둘 다 빌드해 `KDT2Framework/Bin/Server/`의 exe 두 개를 함께 커밋하는가
