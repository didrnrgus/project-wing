# Project Wing — 게임 데이터

캐릭터 / 아이템 / 맵 / 스탯 밸런스 데이터. 클라이언트와 서버가 런타임에 HTTPS 로 내려받는다.

- **원본(수정은 여기서)**: `project-wing` 레포의 `web/`
  https://github.com/didrnrgus/project-wing
- **배포(GitHub Pages, 수정 금지)**: `project-wing-webserver` 레포
  https://didrnrgus.github.io/project-wing-webserver/

원래 project-wing / project-wing-socket-server / project-wing-webserver
3개 레포로 나뉘어 있던 것을 `project-wing` 하나로 통합했다.
`project-wing-webserver` 는 위 Pages URL 을 유지하기 위한 **배포 미러로만** 남겨둔 것이다.

URL 을 바꿀 수 없는 이유: 클라이언트와 서버 양쪽 `Etc/CURL.h` 의 `WEBSERVER_PATH`
상수에 하드코딩되어 있고, 이미 배포된 서버 실행 파일에도 박혀 있다.

## 반영 방법

`project-wing` 레포에서 `web/` 을 수정하고 커밋한 뒤:

```sh
git subtree push --prefix=web \
  https://github.com/didrnrgus/project-wing-webserver.git main
```

> 이 README 는 두 레포에 **동일한 내용으로** 유지해야 한다.
> 한쪽만 수정하면 위 `subtree push` 가 non-fast-forward 로 막힌다.

## 파일

| 파일 | 내용 |
|---|---|
| `config.json` | 다른 데이터 파일 목록 및 맵 구성 |
| `character_list.json` | 캐릭터 정의 |
| `item_list.json` | 아이템 정의 및 밸런스 |
| `stat_type_list.json` | 스탯 타입 정의 |
| `map_0.json` ~ `map_2.json` | 맵 데이터 |
| `color_list.json`, `color_list.png` | 캐릭터 색상 팔레트 |
| `index.html` | Pages 루트 (데이터 서빙용이라 내용은 무의미) |

> `color_list.png` 는 Git LFS 로 추적하면 안 된다. Pages 는 LFS 객체를 서빙하지 않고
> 포인터 텍스트를 그대로 내보내 이미지가 깨진다. `.gitattributes` 가 이를 해제한다.
