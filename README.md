# Project Wing

멀티플레이 러닝 게임. 클라이언트(DirectX/C++), 소켓 서버(C++), 데이터 웹서버(GitHub Pages)로 구성된다.

원래 3개 레포로 나뉘어 있었으나 하나의 프로젝트이므로 이 레포로 통합했다.
통합 내역과 남은 정리 작업은 [docs/CONSOLIDATION.md](docs/CONSOLIDATION.md) 참고.

## 구조

| 경로 | 내용 | 빌드 |
|---|---|---|
| `ProjectWing.sln` | 클라이언트 솔루션 (`KDT2Framework`, `Common`, `AssetProj`) | VS 2022 / x64 |
| `server/` | 소켓 서버 + 테스트 클라이언트 | `server/project-wing-socket-server.sln` |
| `web/` | 게임 데이터 JSON **원본** (캐릭터/아이템/맵/스탯 밸런스) | 빌드 없음 |
| `KDT2Framework/Bin/` | 클라이언트 실행 디렉터리 (에셋, 서버 exe 동봉) | - |

## 실행

서버를 먼저 띄우고 클라이언트를 실행한다. 접속 주소는 클라이언트에서 직접 입력하며
기본값은 `127.0.0.1:12345`다. (`KDT2Framework/Include/Etc/ConstValues.h`, `NetworkManager.cpp`)

## web/ 은 배포 미러가 따로 있다

`web/`의 데이터는 런타임에 HTTPS로 내려받는다. 배포처는 별도 레포의 GitHub Pages다.

```
https://didrnrgus.github.io/project-wing-webserver/
```

이 URL이 클라이언트와 서버 양쪽 `Etc/CURL.h`에 하드코딩되어 있어 **변경할 수 없다.**
따라서 `web/`을 수정한 뒤에는 `project-wing-webserver` 레포로 미러 푸시해야 반영된다.

> `web/`에는 LFS를 적용하지 않는다. GitHub Pages는 LFS 객체를 서빙하지 않고
> 포인터 텍스트를 그대로 내보내기 때문에 이미지가 깨진다. `web/.gitattributes`가 이를 해제한다.
