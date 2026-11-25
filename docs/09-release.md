# 릴리즈 (GitHub Actions)

`main`에 push하면 GitHub Actions가 버전을 올리고, Release|x64로 빌드한 뒤, GitHub Release에 배포 zip을 올립니다. 워크플로 파일은 [`.github/workflows/release.yml`](../.github/workflows/release.yml)입니다.

## 1. 언제 실행되나

| 트리거 | 동작 |
| --- | --- |
| `main`에 push | 자동 실행. 단, `docs/**`, `*.md`, `.gitignore`만 바뀐 push는 건너뜀 |
| Actions 탭 → Release → Run workflow | 수동 실행. `patch` / `minor` / `major` 중 올릴 자리를 고름 |

여러 push가 연달아 들어와도 실행은 한 번에 하나씩 순서대로 돕니다(`concurrency: release`). 그래서 버전이 겹치지 않습니다.

## 2. 버전 규칙

- 태그 형식은 `X.Y.Z`이고 접두사가 없습니다(기존 `1.0.0`과 같은 형식). `server-1.0.0`처럼 형식이 다른 태그는 계산에서 빠집니다.
- 기본은 patch +1입니다.
- 이번 push에 들어간 커밋(마지막 버전 태그 이후 커밋)의 **제목(첫 줄)**에 `#minor`가 있으면 minor +1, `#major`가 있으면 major +1입니다. 커밋 본문은 보지 않습니다. 본문에서 이 키워드를 설명하다가 실수로 major가 올라간 적이 있어서(잘못 만들어진 `2.0.0`은 삭제함) 제목만 보도록 바꿨습니다.

  ```
  git commit -m "맵 선택 화면 개편 #minor"
  ```

- 태그는 빌드와 패키징이 성공한 뒤 릴리즈를 만들 때 함께 생성됩니다. 빌드가 실패하면 버전이 올라가지 않습니다.
- 실패한 실행을 다시 돌렸을 때 HEAD에 이미 버전 태그가 있으면 그 버전을 그대로 씁니다. 릴리즈가 이미 있으면 zip만 교체합니다.
- exe 파일 속성에는 버전이 들어가지 않습니다. 버전은 태그와 릴리즈 이름에만 있습니다.

## 3. 빌드 환경

- 러너: `windows-2022`(VS 2022, v143 툴셋).
  - `windows-latest`는 2026-06부터 VS 2026이고, VS 2026에는 v143 툴셋이 없습니다(v145만 있음). 그래서 `windows-latest`로 바꾸면 빌드가 실패합니다. `windows-2022` 이미지가 없어지면 프로젝트의 `PlatformToolset`을 v145로 올려야 합니다.
- 빌드 명령: `msbuild ProjectWing.sln /p:Configuration=Release /p:Platform=x64 /m`. 서버도 같이 빌드되므로, 릴리즈 zip에 들어가는 서버 exe는 CI에서 새로 빌드한 것입니다.
- 에셋(png, otf)과 lib/dll은 Git LFS입니다. 워크플로는 `.git/lfs`를 캐시하고 바뀐 객체만 받아서 LFS 대역폭을 아낍니다.

## 4. 배포 zip

이름은 `ProjectWing-<버전>-win-x64.zip`입니다. 압축을 풀면 아래 구조입니다.

```
ProjectWing-<버전>/
├─ ProjectWing.bat              실행용. 작업 폴더를 Release로 맞추고 exe를 띄움
├─ Release/
│  ├─ ProjectWing.exe
│  ├─ fmod.dll, libcurl-x64.dll
│  ├─ VC++ 런타임 DLL (msvcp140.dll, vcruntime140*.dll 등)
│  └─ pem/cacert.pem
└─ Bin/
   ├─ Asset/                    폰트, 텍스처
   ├─ Shader/                   *.fx (실행할 때 컴파일)
   ├─ Server/Release/project-wing-socket-server.exe
   └─ Log/
```

- 게임은 exe 폴더나 작업 폴더를 기준으로 `../Bin/...`, `./pem/`을 읽습니다([03-client-engine.md](03-client-engine.md)의 경로 표 참고). 그래서 저장소의 `KDT2Framework/Release` + `KDT2Framework/Bin` 구조를 그대로 옮겼습니다. `Release/ProjectWing.exe`를 탐색기에서 더블클릭해도 실행됩니다.
- 게임 exe는 동적 CRT(`/MD`)라 VC++ 런타임이 필요합니다. 러너의 VS 재배포 폴더(`VC\Redist\MSVC\*\x64\Microsoft.VC14*.CRT`)에서 DLL을 복사해 넣었으므로, 받는 사람이 재배포 패키지를 따로 설치하지 않아도 됩니다. 서버는 정적 CRT(`/MT`)라 필요 없습니다.
- `.cso`, `.lib`, `.pdb`, `.def`, `.exp`는 넣지 않습니다. 셰이더는 실행할 때 `.fx`에서 컴파일합니다.
- 실행할 때 필요한 파일이 새로 생기면(새 DLL, 새 에셋 폴더 등) 워크플로의 Package 단계도 함께 고쳐야 합니다.

## 5. 릴리즈 노트

GitHub의 자동 생성 노트(`--generate-notes`)를 씁니다. 이전 버전 태그부터 이번 커밋까지의 변경이 들어갑니다.

## 6. 확인 방법

- Actions 탭에서 Release 실행 로그를 봅니다. Compute version 단계에 `previous: X -> next: Y`가 찍히고, Package 단계에 zip에 들어간 파일 목록이 찍힙니다.
- Releases 페이지에서 zip을 받아 압축을 풀고 `ProjectWing.bat`으로 실행해 봅니다.
- 릴리즈 생성 단계가 권한 오류로 실패하면 저장소 Settings → Actions → General → Workflow permissions에서 쓰기 권한이 막혀 있지 않은지 확인합니다.
