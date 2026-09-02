# 레포 통합 기록 (2026-09-02)

## 배경

하나의 프로젝트가 3개 레포로 나뉘어 있었다.

| 레포 | 커밋 | 자체 소스 파일 |
|---|---|---|
| `project-wing` (이 레포) | 148 | 260 |
| `project-wing-socket-server` | 28 | 12 |
| `project-wing-webserver` | 38 | 11 |

서버·웹은 레포를 따로 둘 규모가 아니었고, 기여자도 3레포 동일했다.

## 통합 내역

- `project-wing-socket-server` → `server/` (히스토리 27커밋 보존)
- `project-wing-webserver` → `web/` (히스토리 38커밋 보존)
- 클라이언트 파일은 **이동하지 않았다** (아래 "남은 작업" 참고)

통합 후 총 216커밋 = 148(클라) + 27(서버) + 38(웹) + 2(subtree merge) + 1(이 커밋).

구 소켓서버 레포의 `1.0.0` 태그는 이 레포에 이미 동명 태그가 있어(2025-04-23 `서버빌드`)
`server-1.0.0`으로 이름을 바꿔 가져왔다.

흡수 시 `x64/` 하위의 빌드 산출물(`.exe`, `.pdb`)을 히스토리에서 제거했다.
원본 레포에서 루트 `x64/`가 `.gitignore`에 빠져 있어 23MB PDB가 16회 재커밋되어
`.git`이 40MB까지 불어난 상태였다. 루트 `.gitignore`의 `**/*.exe`, `**/*.pdb`
규칙이 이미 이를 막으므로 재발하지 않는다.

단 `x64/`는 산출물 전용 폴더가 아니었다. 다음은 **빌드 입력물**이라 되살렸다.

- `libcurl-x64.lib` — `server/.../GameInfo.h`의 `#pragma comment(lib, ...)`가 링크.
  vcxproj의 `AdditionalLibraryDirectories`가 `$(SolutionDir)x64\$(Configuration)`을 가리킨다.
- `libcurl-x64.dll`, `.def`, `.exp` — 런타임 재배포물
- `pem/cacert.pem` — `server/.../Etc/CURL.h`가 HTTPS 인증서 검증에 사용

## 통합 과정에서 발견한 문제

### 1. 배포된 서버 exe에 대응하는 소스가 커밋되지 않았다 ⚠️

`KDT2Framework/Bin/Server/Release/project-wing-socket-server.exe`는
2025-04-23 커밋 `6643e66 "서버빌드"`에서 156,672 → **159,232바이트**로 갱신되었다.
그런데 서버 소스를 마지막으로 수정한 커밋은 2025-04-16 `42d8567 "방 인원 수 조정"`이고,
그 시점 exe가 156,672바이트다.

즉 **현재 동봉된 서버 실행 파일을 만든 소스가 어느 레포에도 없다.**
로컬 워킹트리도 clean이어서 복구 경로가 없다. 서버를 다시 빌드하면
배포본과 동작이 달라질 수 있으므로, 재개 시 이 차이를 먼저 확인해야 한다.

레포가 갈려 있어서 "소스는 A 레포, 빌드 결과는 B 레포"가 된 결과다.
통합의 직접적인 근거이기도 하다.

### 2. 네트워크 프로토콜이 두 곳에 중복 정의되어 있다 ⚠️

| 위치 | 정의 |
|---|---|
| `KDT2Framework/Include/Etc/NetworkManager.h` | `ClientMessage` 14개 + `ServerMessage` 28개 |
| `server/project-wing-socket-server/Include/GameInfo.h` | 동일 |

현재는 **순서까지 완전 일치**한다(검증 완료). 하지만 값이 순서 기반 정수이므로
한쪽 중간에 메시지를 삽입하면 컴파일은 통과한 채 패킷 해석만 조용히 깨진다.
`shared/Protocol.h` 하나로 합쳐야 한다.

### 3. 클라/서버가 같은 파일을 복사해서 쓴다

| 파일 | 클라 | 서버 | diff |
|---|---|---|---|
| `GameInfo.h` | 402줄 | 122줄 | 431줄 |
| `JsonContainer.h` | 443줄 | 210줄 | 277줄 |
| `JsonController.cpp` | 449줄 | 306줄 | 171줄 |
| `JsonController.h` | 40줄 | 27줄 | 17줄 |
| `DataStorageManager.h` | 135줄 | 95줄 | 42줄 |
| `CURL.h` | 28줄 | 24줄 | 8줄 |

`CURL.h`의 `WEBSERVER_PATH` 상수도 양쪽에 각각 하드코딩되어 있다.

### 4. 서드파티가 두 벌 들어 있다

`curl-8.12.1_1-win64-mingw`와 `json-develop`(nlohmann)이
`KDT2Framework/Include/`와 `server/project-wing-socket-server/Include/`에 각각 존재한다.
`json-develop`은 `include/`만 빌드에 쓰이는데 `docs/`, `tests/`까지 통째로 들어와 있다.

### 5. Notion 인테그레이션 토큰이 공개 서빙되고 있다 🔴

`web/config.json` 의 `api_key` 는 Notion 내부 인테그레이션 토큰이다.
이 파일은 GitHub Pages 로 서빙되므로 누구나 아래 URL 로 토큰을 읽을 수 있다.

```
https://didrnrgus.github.io/project-wing-webserver/config.json
```

해당 인테그레이션(`yang_test_01`)의 권한에 콘텐츠 업데이트/삽입이 포함되어 있어
**쓰기까지 가능**하다. 2025-03 커밋 이후 계속 공개 상태였다.

`KDT2Framework/Include/Etc/JsonContainer.h` 와
`server/project-wing-socket-server/Include/Etc/JsonContainer.h` 의 주석 블록에도
같은 계열 토큰이 박혀 있었는데, 이쪽은 `struct FConfig` 설명용 샘플이라
동작에 영향이 없어 플레이스홀더로 교체했다.

실제로 쓰이는 값은 `web/config.json` 하나다. **토큰 재발급이 유일한 실질적 조치이며,
새 토큰을 다시 `web/config.json` 에 넣으면 같은 상황이 반복된다.**
Notion 호출을 서버가 대신하고 클라이언트는 서버에만 요청하는 구조로 바꿔야 한다.

> 커밋 히스토리에는 구 토큰이 그대로 남아 있다. 주석을 지운 것으로는
> 이미 발생한 노출이 취소되지 않는다.

## 남은 작업

아래는 **이번 통합에서 하지 않았다.** 이 작업 환경에 Visual Studio C++ 툴체인이
설치되어 있지 않아 빌드 검증이 불가능했고, 검증 없이 넣으면 깨진 상태가 커밋된다.
빌드 가능한 환경에서 진행할 것.

- [ ] 클라이언트 3개 프로젝트를 `client/`로 이동
      → `.vcxproj` 상대경로, `.sln` 경로, include 경로 전부 수정 필요
- [ ] `shared/` 추출 — 위 표의 중복 6쌍 단일화, `Protocol.h` 분리
- [ ] `ProjectWing.sln`에 서버 vcxproj 추가해 원솔루션 빌드
- [ ] `third_party/`로 curl·nlohmann 1벌 통합 (또는 vcpkg manifest로 대체)
- [ ] `json-develop/docs`, `json-develop/tests` 제거 (빌드에 미사용)
- [ ] `web/` → `project-wing-webserver` 미러 푸시 자동화
- [ ] 위 문제 #1(소스 없는 exe) 확인 후 서버 재빌드 여부 결정
- [ ] **문제 #5** — Notion 토큰 재발급 후, 공개 `web/config.json` 에서 제거하고
      Notion 호출을 서버 대리 방식으로 전환

## `project-wing-socket-server` 레포 처리

통합이 끝났으므로 이 레포는 더 커밋하지 않는다.
**삭제하지 말고 archive**할 것. 원본 히스토리(x64 산출물 포함)와
태그 `1.0.0`이 가리키던 구 배포 exe가 그 레포에만 남아 있다.
문제 #1을 조사할 때 필요할 수 있다.
