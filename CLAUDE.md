# CLAUDE.md

ProjectWing: 위아래 벽 사이를 날며 장애물을 피하고 비행 거리로 순위를 겨루는 2D 횡스크롤 게임. 자체 C++/DirectX 11 프레임워크 위에서 동작하며 싱글플레이와 최대 5인(`PLAYER_COUNT_MAX`) 멀티플레이를 지원한다.

기능별 상세 문서는 [docs/](docs/README.md)에 있다(기획, 화면 흐름, 엔진, 게임 코드, 네트워크 프로토콜, 서버, 데이터/Notion, 알려진 이슈). 게임 규칙, 패킷, JSON 스키마를 바꾸면 해당 문서도 함께 고친다.

## 빌드 / 실행

- Visual Studio 2022, 툴셋 v143, Windows SDK 10.0, **x64** 구성을 사용한다 (Win32 구성은 사실상 미사용).
- 솔루션: `ProjectWing.sln` (KDT2Framework가 Common, AssetProj를 ProjectReference로 참조). 멀티플레이 소켓 서버 `project-wing-socket-server`(x64만)도 같은 솔루션에 있어 함께 빌드된다.
- 명령줄 빌드:
  ```
  MSBuild ProjectWing.sln /p:Configuration=Debug /p:Platform=x64 /m
  ```
  - `MSBuild`가 PATH에 없으면 Developer PowerShell을 쓰거나 전체 경로로 호출한다. VS Build Tools만 설치된 환경에서는 `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\amd64\MSBuild.exe`다 (이 환경에서는 `vswhere -requires Microsoft.Component.MSBuild`로 못 찾았다).
- 출력: `KDT2Framework/Debug/ProjectWing.exe` (Release는 `KDT2Framework/Release/`). 두 폴더 모두 `fmod.dll`, `libcurl-x64.dll`, `*.cso`, `pem/`이 이미 들어 있다.
- **Debug 빌드는 디버그 기능이 켜진다**: 콘솔 로그 창, 콜라이더/쿼드트리 외곽선, 플레이어 디버그 텍스트, 반투명 스프라이트, 테스트 키(`Z`, `S`), 거리 비례 HP 감소 10배. 실제 플레이 화면과 밸런스는 **Release**로 확인한다. 전체 목록은 [docs/03-client-engine.md](docs/03-client-engine.md#11-debug--release-빌드-차이).
- 실행 위치가 중요하다. 텍스처/폰트/사운드는 exe 폴더 기준, 셰이더/CA 인증서/서버 exe는 **작업 폴더** 기준 `../Bin/...`, `./pem/`을 쓴다. exe 폴더(`KDT2Framework/Debug`, `Release`)에서 실행하거나 VS 디버거(작업 폴더 `KDT2Framework/Include`)로 실행해야 한다.
  - 에셋: `../Bin/Asset/` (폰트, 텍스처, 스프라이트시트)
  - 셰이더 소스: `KDT2Framework/Bin/Shader/*.fx`
  - 멀티 호스트 시 자식 프로세스로 띄우는 서버: `../Bin/Server/{Debug|Release}/project-wing-socket-server.exe`. 소스는 `Server/Include/`(솔루션에 포함, 빌드하면 이 경로로 바로 출력). 원본 서버 소스는 유실되어 클라이언트 프로토콜 기준으로 다시 작성했다.
- 시작 시 인터넷이 필요하다. 게임 데이터(config, 맵, 캐릭터, 아이템, 스탯, 컬러 JSON)를 GitHub Pages(`WEBSERVER_PATH`, `Etc/CURL.h`)에서 받아오고, 랭킹은 Notion API로 읽고 쓴다.
- 테스트 프로젝트는 없다. 검증은 실행해서 확인한다. 예 (PowerShell, exe 폴더를 작업 폴더로):
  ```
  Start-Process KDT2Framework\Release\ProjectWing.exe -WorkingDirectory KDT2Framework\Release
  ```
- 싱글모드: 타이틀 화면의 "SinglePlay"(한국어: "싱글 플레이") 버튼 → 로비 → 인게임. 서버 불필요.

## 릴리즈 (GitHub Actions)

- `.github/workflows/release.yml`: `main`에 push하면 버전을 올리고 Release|x64로 빌드한 뒤 GitHub Release에 `ProjectWing-<ver>-win-x64.zip`을 올린다. `docs/**`, `*.md`만 바뀐 push에서는 실행되지 않는다. Actions 탭에서 수동 실행(`workflow_dispatch`)도 할 수 있다.
- 버전은 태그 `X.Y.Z`(접두사 없음, 기존 `1.0.0` 형식)다. 기본은 patch +1이고, 커밋 메시지에 `#minor` / `#major`를 넣으면 그 자리를 올린다. 태그는 빌드가 성공해야 생성된다. `server-*` 태그는 계산에서 빠진다.
- 러너는 `windows-2022`에 고정돼 있다. `windows-latest`는 VS 2026(v145 툴셋만 포함)이라 v143 프로젝트가 빌드되지 않는다.
- zip 구성: `ProjectWing.bat`, `Release/`(exe, `fmod.dll`, `libcurl-x64.dll`, VC++ 런타임 DLL, `pem/`), `Bin/Asset`, `Bin/Shader`, `Bin/Server/Release/`(서버 exe), `Bin/Log`. 실행 시 필요한 파일이 늘어나면 워크플로의 Package 단계도 고친다.
- 에셋과 lib/dll은 Git LFS다. 워크플로는 LFS 객체를 캐시해 대역폭을 아낀다.

## 소스 인코딩

- `.cpp`/`.h`는 **UTF-8 with BOM**이어야 한다 (한글 주석/문자열, MSVC). 새 파일도 BOM을 붙인다.
- 예외: `UI/UserWidget/RankWidget.h`는 아직 CP949다. 편집할 때 인코딩을 섞지 말고 원래 인코딩을 유지하거나 파일 전체를 UTF-8 BOM으로 변환한다.
- `auto-script.bat` → `encoding-script.py`는 `KDT2Framework/` 아래 파일을 UTF-8 BOM으로 일괄 변환하는 도구다. 빌드 스크립트가 아니다. (`chardet` 필요)

## 구조

| 프로젝트 | 종류 | 역할 |
| --- | --- | --- |
| `Common/Include` | 정적 lib | 엔진 코어: `CGameManager`(윈도우, 메인 루프), `Device`, Scene/SceneManager/Input/Camera/UIManager, Component, Render, `Share`(SharedPtr, Object, Log, Timer), 기본 UI(Button, Image, TextBlock, ProgressBar, TitleBar) |
| `AssetProj/Include` | 정적 lib | `CAssetManager` 아래 Mesh/Texture/Material/Font/Sound(FMOD)/Animation2D 매니저, Shader + 상수 버퍼 |
| `KDT2Framework/Include` | exe | 게임 코드: `Scene/`, `Object/`, `Component/`(2D 콜라이더), `UI/UserWidget/`, `Etc/`(매니저들), `Interface/` |
| `Server/Include` | exe (콘솔) | 멀티플레이 소켓 서버. `select()` 단일 스레드, 50ms 게임 틱. 엔진과 무관하고 nlohmann/json만 공유. 출력은 `KDT2Framework/Bin/Server/{Debug|Release}/`. 상세는 [docs/06-server.md](docs/06-server.md) |

주의: Common은 include 경로에 `KDT2Framework/Include`를 포함하며, `SceneManager.cpp`와 `Scene.cpp`가 게임 씬(`CSceneTitle` 등)을 직접 include한다. 엔진과 게임이 완전히 분리돼 있지 않으니 씬을 추가/변경하면 Common 쪽도 함께 수정해야 한다.

### 메인 루프

`main.cpp` → `CGameManager::Init/Run` → 매 프레임 `Logic()`: Input → Update → Collision → Render → SoundManager Update. 씬 단위 처리는 `CSceneManager`가 현재 씬에 위임한다.

### 씬 흐름

`EGameScene`: Title → (Single: Lobby | Multi: 서버 접속 후 Lobby) → InGame → Result → Lobby, Title ↔ Rank.

- 씬 전환은 `CSceneWidget::LoadScene()`을 쓴다. `CTaskManager` 스레드에서 로딩 문구를 띄우고 약 2초 뒤 `mScene->GotoXxx()`를 호출한다.
- Title 진입 시 `CTitleWidget::LoadGameData()`가 웹서버 JSON을 받아 `CDataStorageManager`에 저장한 뒤 `LoadRankData()`로 Notion 랭킹을 읽는다.

### 패턴 / 관례

- 싱글턴: `DECLARE_SINGLE(Type)` / `DEFINITION_SINGLE(Type)` (`GameInfo.h`), 접근은 `Type::GetInst()`, 해제는 `DestroyInst()`.
- 참조 카운트 포인터: `CSharedPtr<T>` (`Share/SharedPtr.h`), 엔진 오브젝트는 `CObject` 파생.
- 클래스 접두사 `C`, 구조체 `F`, 인터페이스 `I`, enum은 `namespace EXxx { enum Type {...}; }` 형태. 멤버 변수 `m` 접두사.
- 객체 생성: `mScene->CreateObj<T>(name)`, 컴포넌트 `CreateComponent<T>()`, 위젯 `GetUIManager()->CreateWidget<T>(name)`.
- 입력: `mScene->GetInput()->AddBindKey(name, key)` + `AddBindFunction<T>(name, EInputType::Down/Hold/Up, this, &T::Func)`.
- 로그: `CLog::PrintLog(...)`.
- 키 상수와 에셋 경로/이름 매크로는 `Etc/ConstValues.h`에 있다. 화면에 보이는 문구는 여기 두지 말고 로컬라이징 테이블에 추가한다 (아래 "로컬라이징").

### 로컬라이징 (영어 / 한국어)

- `Etc/Localization.*`: `CLocalization` + `LOC(Key)` 매크로. 문구는 `Localization.cpp`의 `gLocTable`에 키별 `{ 영어, 한국어 }` 쌍으로 들어 있다.
- 새 UI 문구를 넣을 때:
  1. `Localization.h`의 `ELocKey`에 키를 추가한다.
  2. `gLocTable`에 같은 키로 영어/한국어 문구를 추가한다. 빠뜨리면 `static_assert`로 컴파일이 실패한다.
  3. 코드에서 `SetText(LOC(NewKey))`처럼 쓴다. 문자열 리터럴을 UI에 직접 넣지 않는다.
- 포맷 문자열도 키로 관리한다 (`WindowTitleFormat`, `InGameDistFormat`). 언어마다 `%` 인자의 순서와 타입이 같아야 한다.
- 정보 라벨 배열(`JsonContainer.h`의 `gArrMapInfoText` 등)과 랭킹/결과 탭(`mArrMenuTapText`)은 문자열이 아니라 `ELocKey` 배열이다. 화면에 쓸 때 `CLocalization::Get()`으로 변환한다.
- 언어 결정 순서: exe 폴더의 `Language.ini`(`[Settings] Language=en|ko`, gitignore 대상) → 없으면 OS UI 언어(한국어면 ko, 그 외 en). `CGameManager::Init()`에서 윈도우 생성 전에 `CLocalization::Init()`을 호출한다.
- 메뉴바의 `Language` 메뉴로 바꾼다. Win32 메뉴와 창 제목은 즉시 바뀐다 (`CGameManager::UpdateMenu()`가 메뉴 바 전체를 다시 만든다). 게임 UI는 위젯이 씬 전환 때 새로 만들어지므로 다음 화면 전환부터 반영된다.
- 로컬라이징 대상이 아닌 것: 웹서버 JSON에서 받는 데이터(맵/캐릭터 이름, 아이템 설명), 디버그 텍스트, FPS, 로고(`PROJ-WING`), 콘솔 로그.
- 폰트는 `NotoSansKR`(`FontManager.cpp`)라 영어와 한국어 모두 렌더링된다.

### 네트워크 (멀티플레이)

- `CNetworkManager` (`Etc/NetworkManager.*`): Winsock TCP, 포트 12345, 수신 스레드와 하트비트 스레드를 따로 돌린다. 메시지는 `MessageHeader{senderId, msgType, bodyLen}`(pack 1) + body.
- 메시지 타입: `ClientMessage::Type`, `ServerMessage::Type`. 서버(`Server/Include/Protocol.h`)와 순서/값이 맞아야 하므로 enum 중간에 끼워 넣지 말고, 바꾸면 양쪽을 같이 고친다.
- 수신 메시지는 큐에 쌓이고, 씬이 매 프레임 `PollMessage()`로 꺼내 `ISceneNetworkController::DistributeMessage()`로 `IObjectNetworkController` 리스너(오브젝트/위젯)에게 뿌린다. 리스너는 `AddListener`/`RemoveListener`로 등록한다.
- 모드 분기는 `CNetworkManager::GetInst()->IsMultiplay()`. 싱글에서는 로컬에서 거리/HP를 계산하고, 멀티에서는 서버 메시지(`MSG_PLAYER_DISTANCE`, `MSG_TAKEN_DAMAGE` 등)로 갱신한다. 게임 로직을 바꿀 때 두 경로를 모두 확인할 것.
- 호스트 IP와 닉네임은 윈도우 메뉴 팝업(`CGameManager::OpenIPInputPopup` 등)에서 입력한다. 기본값은 `127.0.0.1`.
  - 메뉴바 순서: 닉네임 | 멀티플레이 | (방장 IP) | Language. "다른 방에 참가하기"를 누르면 `option2Visible`이 켜지고 방장 IP 메뉴가 나타난다. 이 상태에서는 멀티플레이 버튼을 눌러도 서버를 직접 띄우지 않고 입력한 IP로 접속만 한다.
- `CMultiplayManager`: 방 인원, 플레이어 선택(캐릭터/아이템/맵), 준비 상태 같은 멀티 공유 상태를 관리한다.

### 데이터 / 외부 연동

- `CCURL::SendRequest(url, method, json)`: libcurl 래퍼, CA 인증서는 `./pem/cacert.pem`.
- `CJsonController` + `Etc/JsonContainer.h`: nlohmann/json 파싱과 `F*` 구조체(`FConfig`, `FMapInfo`, `FCharacterState`, `FItemInfo`, `FUserRankInfo` 등).
- `CDataStorageManager`: 로드한 게임 데이터와 현재 선택(맵/캐릭터/아이템 슬롯), 결과를 보관한다.
- `CNotionDBController`: 랭킹 CRUD. `DatabaseID`와 `APIKey`는 웹서버의 `config.json`에서 받는다.

### 게임플레이 핵심 파일

- `Object/PlayerInGameObject`: 내 플레이어. 좌클릭 상승, 스페이스 부스트, 벽 충돌 시 데미지와 스턴, 거리 기반 HP 감소, 사망하면 Result로 이동.
- `Object/PlayerInGameOtherObject`: 멀티에서 다른 플레이어 표시.
- `Object/LineGroupObject`: 맵 데이터(`lineNodes`)로 위/아래 벽 라인과 `CColliderLine2D`를 생성하고 스크롤한다.
- `Object/ObstacleGroupObject`: 장애물 (멀티에서는 `MSG_OBSTACLE`로 동기화).
- `Scene/SceneCollision`, `CollisionQuadTree`: 충돌 프로파일(`PROFILE_*`)과 쿼드트리.

## 외부 라이브러리 (수정 금지)

`KDT2Framework/Include/curl-8.12.1_1-win64-mingw/`, `KDT2Framework/Include/json-develop/`, `AssetProj/Include/Asset/Sound/fmod*.h`, `AssetProj/Include/Asset/Texture/DirectXTex.h`. 검색할 때는 이 경로들을 제외하면 결과가 깔끔하다.
