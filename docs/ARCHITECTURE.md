# KW4_X 프로젝트 구조 문서

> 정식명칭: **우리아이 한글떼기**
> 유아 대상 한글 학습 게임. 그림카드를 보고 맞는 한글 낱말을 글자 조각으로 조합해 맞춘다.
> 빌드/서명/플랫폼 함정은 [BUILD.md](BUILD.md), 스토어 문구는 [store_listing.md](store_listing.md) 참고.

---

## 1. 개요

| 항목 | 내용 |
|------|------|
| 장르 | 유아 한글 학습 게임 |
| 엔진 | cocos2d-x 3.17.x (C++) |
| 플랫폼 | Windows(Win32), Android, iOS/macOS |
| 주 타겟 | 한국 마켓 (비한국어권 사용자 ~10% 유입) |
| 아키텍처 | cocos2d-x Scene 기반 + Singleton 매니저 |

**서비스 중인 앱**

| 마켓 | 번들 ID | 형태 |
|------|---------|------|
| Google Play | `com.ozzywow.kw4android` | 무료 + 인앱결제(전체잠금해제) |
| App Store (LITE) | `com.ozzywow.kw4iphonelite` | 무료 + 인앱결제 |
| App Store (정식) | `com.ozzywow.kw4iphone` | 유료 (전체 개방) |

---

## 2. 폴더 구조

```
kw4_x/
├─ Classes/              # 게임 로직 (C++)
├─ Resources/            # 이미지·사운드·데이터
│  ├─ UI4HD/             # HD UI 스프라이트 (-hd / -hdx)
│  ├─ word_card_pic_h/   # 낱말 그림카드 (jpg, 한글 파일명) — 415개
│  ├─ word_card_sound/   # 낱말 발음 (mp3, 한글 파일명) — 256개
│  ├─ Sound/             # 효과음·BGM·음성
│  ├─ fonts/             # malgun.ttf(한글), arial.ttf
│  └─ word_card_data_x.xml  # ★ 낱말 카드 마스터 데이터 (350개)
├─ proj.win32/           # Visual Studio / CMake
├─ proj.android/         # Android Studio (Gradle)
└─ proj.ios_mac/         # Xcode
cocos2d-x/               # 엔진 (git 제외 — BUILD.md 참고)
docs/                    # 문서 (본 파일 포함)
```

---

## 3. 실행 흐름 & 씬 전환

`AppDelegate::applicationDidFinishLaunching`
→ 해상도 정책 `FIXED_WIDTH`(640 고정폭) 설정
→ 리소스 검색경로 설정
→ `PointManager::LoadXML()` (낱말 데이터 로드)
→ `MainMenuScene` 실행

```
MainMenuScene ──[시작]──► StudyScene ◄──► StudyScene (다음/이전 낱말)
     │                        │
     │                        ├─[정답 6개 누적]──► AppleTreeScene
     ├─[칭찬나무]──────────────┴────────────────► AppleTreeScene
     │
     └─[정보]────────────────► InfoScene (레벨선택·힌트옵션·인앱결제·복구·초기화)
```

- **씬 전환은 항상 `Director::replaceScene`** + Transition(SlideIn / PageTurn) 사용.
- 낱말 선택·레벨 진행 로직은 씬이 아니라 `PointManager`가 소유한다 (씬은 표시만 담당).

---

## 4. 핵심 클래스

### 씬 (cocos2d::Scene)

| 클래스 | 역할 |
|--------|------|
| `MainMenuScene` | 메인 메뉴. 시작/정보/칭찬나무/공유 버튼, (iOS LITE) 정식버전 배너 |
| `StudyScene` | **핵심 게임플레이.** 그림카드 표시 + 글자조각 드래그로 낱말 조합 |
| `AppleTreeScene` | 획득한 사과(포인트 보상)를 나무에 모으는 보상 씬. 벌레(lavar)/나비 상호작용 |
| `InfoScene` | 설정: 레벨 선택, 힌트 on/off, 인앱결제, 구매복구, 진행초기화 |

### 매니저 (Singleton)

| 클래스 | 역할 |
|--------|------|
| `PointManager` | ★ 게임 상태의 중심. 낱말 데이터·레벨·마스터 여부·포인트·구매(cart)·씬 전환 결정·저장/로드 |
| `CharacterFactory` / `Character` | 사과/벌레/나비 오브젝트 풀 관리 (최대 64개). 칭찬나무에 배치 |
| `SoundFactory` | 효과음/음성 재생 (SimpleAudioEngine 래퍼) |
| `WordFactory` | 오답용 랜덤 한글 글자 생성, 빈 글자 레이어 생성 |

### UI/입력 컴포넌트

| 클래스 | 역할 |
|--------|------|
| `TextLayer` | 글자 한 조각(배경+한글 라벨). 드래그 대상. 정답칸 배치 상태 추적 |
| `TouchedHandleLayer` | StudyScene의 글자 드래그&드롭 처리 (터치 → 정답칸 스냅) |
| `UI_GameResultWindow`, `UI_PopupWindowBase` | 결과/확인 팝업 |
| `NativeShare` | 네이티브 공유 시트 (모바일) / URL 열기 (데스크톱) |

### 결제 (플랫폼 분기)

| 클래스 | 플랫폼 |
|--------|--------|
| `AndroidBilling` | Android (Google Play Billing, JNI) |
| `MKStoreManager*` / `MKStoreManagerDelegate` | iOS (StoreKit) |

`MKStoreManagerDelegate`의 콜백(`productPurchased` 등)을 `MainMenuScene`/`InfoScene`이 구현하여 구매 결과를 `PointManager`의 cart에 반영한다.

---

## 5. 데이터 모델

### 낱말 카드 — `word_card_data_x.xml` (350개)

```xml
<root>
  <card>
    <word>강아지</word>   <!-- 정답 낱말 (= 그림/사운드 파일명) -->
    <level>1</level>      <!-- 난이도 1~5 -->
    <text>...</text>      <!-- level 5(문장 읽기)에서 화면에 표시할 텍스트 -->
  </card>
  ...
</root>
```

- 그림: `word_card_pic_h/{word}.jpg`, 발음: `word_card_sound/{word}.mp3` (파일명이 한글 낱말).
- `PointManager::LoadXML()`이 로드하여 `m_mCardsByWord`(낱말→카드), `m_mCardsByLevel`(레벨→카드목록) 두 인덱스로 보관.

**레벨 구성 (현재 데이터 기준)**

| 레벨 | 카드 수 | 성격 |
|------|--------|------|
| 1 | 33 | 기초 낱말 (무료) |
| 2 | 97 | |
| 3 | 45 | |
| 4 | 27 | |
| 5 | 148 | 문장 읽기 (`text` 표시, 발음재생 없음) |

> LITE 버전은 레벨 2 이상 진입 시 해당 단계 미구매면 `InfoScene`(결제)로 유도.
> 인앱결제는 어느 상품을 사든 전체(STEP2~5+TOTAL) 개방하도록 개선됨 (`MainMenuScene::productPurchased`).

### 저장 데이터

| 저장소 | 키/파일 | 내용 |
|--------|---------|------|
| `UserDefault` | `curr_stage`, `point`, `level`, `hint`, `cart_{0..5}` | 진행/설정/구매(LITE) |
| Writable XML | `mast_words.xml` | 마스터(완료)한 낱말 목록 |
| Writable XML | `characters.xml` | 칭찬나무 사과/오브젝트 상태(위치·타입·bite수) |

---

## 6. 게임플레이 로직 (StudyScene)

1. `initVal(word, level, text)`로 그림/사운드/레벨 세팅. 0.5초 뒤 발음 재생(레벨 5 제외).
2. 하단에 8개 글자조각(`TextLayer`) 배치 — 정답 글자들을 랜덤 위치에 심고 나머지는 오답(`WordFactory::RandomWord`).
3. 사용자가 글자를 정답칸(빈칸 배열 `arrayPoint`)으로 드래그. `TouchedHandleLayer`가 처리.
4. `checkWord()`로 조합 문자열 == `m_wordName` 확인.
5. **정답(`OnPassed`)**: 마스터 처리, 포인트+1, 사과 그리기.
   - 포인트 6점 누적 시 → 사과 캐릭터 획득(`CharacterFactory`), 포인트 리셋.
   - 포인트 0(=방금 6점 채움)이거나 벌레 이벤트 활성 시 → `AppleTreeScene`로 이동.
   - 그 외 → `TurnPage()`로 다음 낱말.
6. **넘기기/힌트**: `OnSkip`(포인트 리셋+힌트 후 다음), 힌트 버튼(포인트 차감+정답 애니메이션).

**보조 요소**

- `boxboy` 캐릭터 표정(`ChangeEmotion`): 기본/행복/물음/슬픔.
- 벌레(lavar) 타임어택: 30분의 1 확률로 10초 제한 이벤트(`TimeRun`).
- `m_isSuccessed` 플래그로 버튼 중복 입력 방지.

---

## 7. 크로스플랫폼 처리 (중요 패턴)

| 주제 | 처리 방식 | 위치 |
|------|-----------|------|
| **해상도** | `FIXED_WIDTH` 640 고정폭, 높이는 기기별 가변. `ACTIVE_HEIGHT`/`H_OFFSET`로 세로 위치 보정 | `common_define.h`, `AppDelegate.cpp` |
| **배경 에셋** | 화면비 > 1.6이면 `-hdx`(긴 화면), 아니면 `-hd`(iPad/데스크톱) 선택 후 cover 스케일 | `StudyScene::initVal` |
| **한글 폰트** | `KR_FONT_TTF = "fonts/malgun.ttf"`. arial.ttf엔 한글 글리프 없음 → 한글은 반드시 malgun 지정 | `stdafx.h` |
| **한글 인코딩** | Win32는 `UTF8()` 매크로(CP_ACP→UTF-8) 변환, 그 외는 그대로. 소스 리터럴 대신 UTF-8 바이트 직접지정하기도 함 | `KoreanUTF8.h` |
| **Android 에셋** | APK 내 파일은 `fopen()` 금지 → `getDataFromFile()` 사용 | `PointManager::LoadXML` |
| **사운드 포맷** | Android는 ogg/m4a→ogg, iOS는 aiff/m4a 분기 | `SoundFactory.h` |
| **리소스 경로** | 플랫폼별 `searchPaths` 분기 | `AppDelegate.cpp` |
| **무료/유료 분기** | 기본 `LITE_VER`(무료). iOS 유료 타깃(`kw4`)만 `KW4_PAID` 정의 → `common_define.h`에서 `LITE_VER`를 끔. 즉 두 디파인이 한 쌍 | `common_define.h`, `CMakeLists.txt` |

> ⚠️ 한글 렌더 관련 함정은 auto-memory `reference_korean_text_rendering`,
> Android 에셋 크래시는 `project_android_asset_fopen`에도 정리되어 있음.

---

## 8. 코드 관례 (작업 시 유의)

- **Scene 기반 구조 유지.** 클래스/파일 이름 변경 금지 (`CLAUDE.md` 규칙).
- 게임 상태·진행 결정은 `PointManager`로 집중. 씬은 렌더/입력만.
- `common_define.h`의 z-order 태그(`kGameSceneTag*`)로 자식 노드 식별/제거.
- 매니저는 `Singleton<T>` 템플릿 사용 (`Instance()`).
- Windows(VS/CMake)·Android(Gradle)·iOS(Xcode) **세 플랫폼 모두 컴파일** 유지가 원칙.
