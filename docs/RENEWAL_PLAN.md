# KW4_X 리뉴얼 계획

> 대상: **우리아이 한글떼기** (cocos2d-x 3.17.x)
> 진행 원칙: [CLAUDE.md](../kw4_x/CLAUDE.md) — 최소·국소 변경, Scene 구조 유지, 3플랫폼(Win/Android/iOS) 컴파일 유지.
> 구조 파악은 [ARCHITECTURE.md](ARCHITECTURE.md), 빌드는 [BUILD.md](BUILD.md) 참고.

리뉴얼은 두 단계로 진행한다.
**1단계 = 시스템 안정성** (사용자에게 안 보이지만 크래시·품질 기반) →
**2단계 = UI 개선** (안정된 기반 위에서 시각/경험 개선).
안정성을 먼저 잡아야 UI 변경 시 회귀 원인을 구분할 수 있다.

각 항목의 **[근거]** 는 실제 코드에서 확인된 지점, **[영향]** 은 사용자 체감/위험도다.

---

## 진행 현황 (2026-07-24 기준)

| 항목 | 상태 |
|------|------|
| C1 배포 아카이브 추적 제거 + 히스토리 재작성 | ✅ 완료 (.git 1.5GB→40MB, force-push, [[project_history_rewrite_2026-07]]) |
| C2 HelloWorldScene 제거 | ✅ 완료 |
| C3 레거시 UI 폴더 제거 | ✅ 완료 |
| A1 GetRandNum -1 크래시 | ✅ 완료 (Windows 빌드 검증) |
| A2 ChangeEmotion NULL 역참조 | ✅ 완료 (Windows 빌드 검증) |
| B1 데이터 정합성 | ✅ 정상 확인 — 불일치는 결함 아님(레벨 5는 설계상 음성 없음) |
| D1 UIAlertView→UIAlertController | ✅ 코드 완료 — ⚠️ iOS 전용이라 Xcode/실기기 검증 필요 |
| A3 회귀 점검 | ✅ 완료 — AppleTreeScene stopActionByTag(101) 회귀 복원 + wordFactory NULL 가드(신규) |
| B2 랜덤 헬퍼 헤더 분리 | ✅ 완료 (Windows 빌드 검증) |
| B3 SoundFactory const | ✅ 완료 (Windows 빌드 검증) |
| D2 구매/복구 흐름 | ✅ 코드 검토 완료 — 로직 정상, 변경 불필요. ⚠️ 실기기 결제/복구 테스트 필요 |
| **1단계 안정성** | ✅ **주요 항목 완료** (남은 것: iOS 실기기 검증 D1·D2) |
| 2단계 UI (U2~U5, U1 제외) | ⬜ 미착수 |

### iOS 실기기 검증 체크리스트 (다음 Mac 작업 시)

- [ ] **D1**: 결제 실패(카드 거절 등) 시 UIAlertController 알림이 실제로 뜨는가 (최신 iOS)
- [ ] **D1**: 자녀보호로 IAP 비활성일 때 "In-App Purchasing disabled" 알림 표시 확인
- [ ] **D2**: 단계별 상품 구매 → 전체 단계 즉시 언락 + 재실행 후 유지(SaveData) 확인
- [ ] **D2**: "구매 복구" → 복구 완료 팝업, 복구 중 levelup 사운드 미재생 확인
- [ ] **D2**: 구매/복구 중 버튼 연타해도 중복 처리 안 되는지(isProgress) 확인

---

## 1단계 — 시스템 안정성 점검 및 개선

### 1-A. 확인된 크래시/버그 수정 (최우선)

| # | 항목 | 근거 | 영향 |
|---|------|------|------|
| A1 | `GetRandNum()`가 실패 시 **-1 반환** → `m_arrayTextLayer[-1]` 배열 범위 밖 접근 | [common_define.h:98](../kw4_x/Classes/common_define.h#L98), [StudyScene.cpp:188](../kw4_x/Classes/StudyScene.cpp#L188) | 낱말 길이/랜덤 조합에 따라 **간헐적 크래시** 가능 |
| A2 | `Sprite::create` 반환 **NULL 미검증** (전체 53곳). 예: `ChangeEmotion` default 분기에서 `faceBtn`가 NULL이면 바로 역참조 | [StudyScene.cpp:352](../kw4_x/Classes/StudyScene.cpp#L352) 외 | 리소스 누락/오타 시 크래시. 핵심 경로부터 방어 |
| A3 | 기존 메모리에 기록된 크래시 패턴 **회귀 점검**: AppleTreeScene 스프라이트 제거·복사, Android APK 에셋 `fopen` | auto-memory `project_appletreescene_bugs`, `project_android_asset_fopen` | 이미 수정됐다면 회귀 테스트로 고정 |

### 1-B. 방어 코드 · 데이터 정합성

| # | 항목 | 근거 | 영향 |
|---|------|------|------|
| ~~B1~~ | ✅ **정상 확인 — 결함 아님.** 카드 350 vs 발음 256 불일치는 **레벨 5(문장 읽기)가 설계상 음성이 없기 때문**. 그림 누락은 코드에서 이미 처리됨 | — | 조치 불필요 |
| B2 | 헤더의 `static` 가변 전역(`arrRandFlag`, `GetRandNum`)은 헤더 포함 TU마다 별도 사본 — 현재는 StudyScene 단독 사용이라 우연히 동작 | [common_define.h:91-116](../kw4_x/Classes/common_define.h#L91) | 향후 확장 시 숨은 버그원. 구조 정리 |
| B3 | `SoundFactory::play(char*)` 비-const + 호출부 `const_cast` | [SoundFactory.h:57](../kw4_x/Classes/SoundFactory.h#L57), [StudyScene.cpp:456](../kw4_x/Classes/StudyScene.cpp#L456) | 안전하지 않은 시그니처. `const std::string&` 로 정리 |

### 1-C. 저장소 위생 · 죽은 코드 (즉시 처리 가능, 위험 낮음)

| # | 항목 | 근거 | 영향 |
|---|------|------|------|
| C1 | **38MB `한글떼기.ipa` + 빌드 로그가 git에 커밋됨** (소스 폴더 `Classes/` 내부) | `kw4_x/Classes/kw4_x_lite 2026-05-13 .../` (git 추적 확인됨) | 저장소 비대·클론 지연. **git에서 제거 + .gitignore** |
| C2 | `HelloWorldScene.cpp/.h` 미사용 (cocos 템플릿 잔재, 실코드 참조 0) | grep 결과 참조 없음 | 죽은 코드 제거로 혼선 감소 |
| C3 | 레거시 `Resources/UI/` 폴더 코드 참조 **0** (전부 `UI4HD/` 사용) | grep: `UI/`=0, `UI4HD/`=77 | 빌드 산출물에 불필요 에셋 포함. 확인 후 제거 → 앱 용량↓ |

### 1-D. iOS 결제(IAP) 현대화

| # | 항목 | 근거 | 영향 |
|---|------|------|------|
| D1 | `MKStoreManager.mm` deprecated API: `UIAlertView` 3곳, `NSURLConnection sendSynchronousRequest` 2곳 | [BUILD.md 알려진 경고](BUILD.md) | 최신 iOS에서 **결제 실패/오류 알림이 안 뜰 수 있음**. `UIAlertController`로 이전 |
| D2 | 구매 복구/전체잠금해제 흐름 검증 (한 상품 구매 시 전체 개방 로직) | [MainMenuScene.cpp:204](../kw4_x/Classes/MainMenuScene.cpp#L204) | 결제는 매출 직결 — 실기기 회귀 테스트 필수 |

> **1단계 산출물**: 크래시 수정 커밋 + 데이터 정합성 리포트 + 저장소 정리 + IAP 실기기 검증 체크리스트.

---

## 2단계 — UI 개선

안정성 확보 후 착수. 큰 리스크 순으로 정리.

| # | 항목 | 근거/현황 | 방향 |
|---|------|-----------|------|
| ~~U1~~ | ❌ **범위 제외 — 현지화 계획 없음(확정).** 비한국어권 ~10% 유입이 있으나 한글 학습 앱 특성상 현지화하지 않는다. 다시 제안하지 말 것 | — | — |
| U2 | 화면비/해상도 대응 QA | `FIXED_WIDTH` + `-hd`/`-hdx` 분기, 최근 개선됨 | 실기기 매트릭스(구형 폰~아이패드~긴 화면) 시각 검수, 잘림/여백 확인 |
| U3 | 한글 렌더 가독성 일관화 | malgun.ttf 사용, `PrintStyle` 외곽선 라벨 | 폰트 크기/외곽선/색 대비 유아 가독성 기준으로 정리 (auto-memory `reference_korean_text_rendering` 준수) |
| U4 | 화면별 시각 폴리시 | MainMenu/Study/AppleTree/Info | 버튼·팝업·전환 애니메이션·피드백 개선. 스타일 통일 |
| U5 | 앱 아이콘/스토어 이미지 갱신 | 유료/무료 아이콘 분리돼 있음 | 리뉴얼 톤에 맞춰 갱신 (스토어 문구는 [store_listing.md](store_listing.md)) |

> **2단계 산출물**: 기기 매트릭스 QA 결과 + 화면별 UI 개선 커밋 + (결정 시)현지화 리소스.

---

## 권장 진행 순서

1. **C1~C3 (저장소/죽은 코드 정리)** — 위험 없고 즉시 효과. 리뉴얼 작업의 깨끗한 출발점.
2. **A1~A3 (크래시 수정)** — 안정성의 핵심. 각 수정마다 3플랫폼 컴파일 확인.
3. **B1 (데이터 정합성 감사)** — 스크립트로 누락 목록 뽑아 콘텐츠 보강 판단.
4. **B2~B3, D1~D2** — 코드 위생 + iOS 결제 현대화(실기기 필요).
5. → 1단계 안정화 후 **2단계 UI** 착수. U2(해상도 QA)로 현재 상태 기준선 잡고 U3~U5 순차 개선. (U1 현지화는 계획 없음으로 제외 확정)

각 단계는 독립 커밋으로 쪼개 회귀 추적이 쉽도록 한다.
