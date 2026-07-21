# 플랫폼별 빌드 안내

Windows(Visual Studio + CMake)에서 개발한 내용을 **Android Studio / Xcode**에서 빌드할 때
막히기 쉬운 지점을 정리한 문서입니다.

버전: cocos2d-x **3.17.2** / Gradle **8.7** / AGP **8.5.x** / NDK **30.0.14904198**

---

## 0. 저장소에 없는 것 — 먼저 챙길 것

`.gitignore`로 제외된 것들이라 클론만 해서는 빌드가 되지 않습니다.

| 대상 | 경로 | 제외 이유 | 준비 방법 |
|---|---|---|---|
| cocos2d-x 엔진 | `<repo>/cocos2d-x/` | 용량 | 3.17.2 를 이 경로에 배치. 최초 1회 `python download-deps.py` |
| 한글 폰트 | `kw4_x/Resources/fonts/malgun.ttf` | MS 독점 폰트 | Windows `C:\Windows\Fonts\malgun.ttf` 복사 |
| Android 빌드 산출물 | `proj.android/app/{build,.cxx}` | 수 GB | 빌드하면 생성됨 |
| 로컬 SDK 경로 | `proj.android/local.properties` | 머신마다 다름 | Android Studio가 자동 생성 |

**폰트가 특히 중요합니다.** `StudyScene` · `TextLayer` · `TouchedHandleLayer`가
`KR_FONT_TTF`(= `fonts/malgun.ttf`)로 낱말 글자를 렌더하므로, 없으면 게임의 핵심 화면이
글자 없이 뜹니다. 메인메뉴 버튼 글자는 이미지에 구워져 있어 폰트 없이도 정상입니다.

경로 기준: `COCOS2DX_ROOT_PATH = kw4_x/../cocos2d-x` (`kw4_x/CMakeLists.txt:7`)

---

## 1. Windows (기준 환경)

```powershell
cmake -S kw4_x -B kw4_x/build_win32
cmake --build kw4_x/build_win32 --target kw4_x --config Debug
```

실행 파일은 `kw4_x/build_win32/bin/kw4_x/Debug/kw4_x.exe`.
리소스는 빌드 시 실행 폴더로 복사되지만 **증분 추적이라 새 이미지가 즉시 반영되지 않을 수 있습니다.**
버튼 이미지 등을 교체했는데 화면이 그대로면 실행 폴더의 `Resources/`에 직접 복사해 보세요.

---

## 2. Android Studio

### 여는 방법
`kw4_x/proj.android` 를 프로젝트로 엽니다 (저장소 루트가 아님).

### 서명 설정 — 가장 많이 막히는 부분

`app/build.gradle`의 `signingConfigs.release`가 **전역** `~/.gradle/gradle.properties`의
네 값을 읽습니다.

```properties
RELEASE_STORE_FILE=C:/Users/<user>/Rebuild/keys/kw4x_upload.jks
RELEASE_KEY_ALIAS=KW_UPLOAD
RELEASE_STORE_PASSWORD=<ASCII만>
RELEASE_KEY_PASSWORD=<ASCII만>
```

> **비밀번호는 반드시 ASCII로.**
> `gradle.properties`는 `java.util.Properties` 규격이라 Gradle이 **ISO-8859-1로 읽습니다.**
> UTF-8로 저장된 한글 비밀번호는 바이트 단위로 쪼개져 전혀 다른 문자열이 되고
> (실측: 13자 → 35자), 맞는 비밀번호인데도 `keystore password was incorrect`로 실패합니다.
> 한글을 꼭 써야 하면 `\uXXXX` 이스케이프로 적어야 합니다.

검증 도구: `tools/KeystorePwFix.java` — GUI로 비밀번호를 받아 키스토어로 실제 열리는지
확인한 뒤 `gradle.properties`를 갱신합니다(비밀번호를 콘솔에 출력하지 않음).

```powershell
& "C:\Program Files\Android\Android Studio\jbr\bin\java.exe" tools\KeystorePwFix.java
```

### 서명 설정을 고쳤다면 반드시 Sync

`File → Sync Project with Gradle Files`

서명 설정이 깨지면 Gradle sync가 실패하고, 그러면 Android Studio가 Android 모델을 잃어
**`Generate Signed App Bundle or APK` 메뉴가 사라지고**, 빌드가 존재하지도 않는 태스크를
호출합니다. 설정만 고치고 sync를 안 하면 계속 실패합니다.

### 쓰면 안 되는 메뉴

**`Build → Assemble Project with Tests`** — 항상 실패합니다.
이 메뉴는 `unitTestClasses` / `androidTestClasses`를 호출하는데 AGP 8.5는 그런 태스크를
만들지 않습니다(`assembleUnitTest`가 대응). 테스트 코드도 없습니다.

- 빌드 확인 → `Build → Assemble Project`
- 릴리스 AAB → `Build → Generate App Bundles or APKs → Generate Bundles`

### 터미널 빌드

```powershell
cd kw4_x\proj.android
$env:JAVA_HOME = "C:\Program Files\Android\Android Studio\jbr"
.\gradlew assembleDebug      # 또는 bundleRelease
```

`JAVA_HOME`을 안 잡으면 `JAVA_HOME is not set` 으로 즉시 실패합니다.

### 출시 관련

- 새 AAB를 올릴 때마다 `versionCode` 증가 (`app/build.gradle`)
- ABI는 `arm64-v8a:armeabi-v7a` — x86을 넣으면 64비트 요구사항 위반으로 거부
- `compileSdk`/`targetSdk` 35
- **업로드 키 재설정 진행 중** — 승인 전에는 새 키로 서명한 AAB를 올려도 거부됩니다

---

## 3. Xcode (iOS)

> **주의: 아래 절차는 Windows에서 검증할 수 없어 실제로 실행해 본 적이 없습니다.**
> CMake 구성 파일의 문법과 Win32 빌드까지만 확인했습니다.

### Xcode 프로젝트 생성

```bash
cmake -S kw4_x -B build_ios -GXcode \
  -DCMAKE_TOOLCHAIN_FILE=cocos2d-x/cmake/ios.toolchain.cmake \
  -DIOS_PLATFORM=OS          # 시뮬레이터는 SIMULATOR
open build_ios/kw4_x.xcodeproj
```

### Windows에서 미리 고쳐둔 것

이 저장소에는 iOS 빌드가 깨지는 문제 네 가지가 이미 수정돼 있습니다 (커밋 `2828eb2`).
**아래는 "이미 해결됨" 목록이므로 다시 손댈 필요가 없습니다.**

| 증상 | 원인 | 조치 |
|---|---|---|
| 링크 실패 `MKStoreManager` undefined | `MKStoreManager.mm`이 CMake 소스 목록에 없었음 | 목록에 추가 |
| 링크 실패 `_OBJC_CLASS_$_SKPaymentQueue` | cocos2d가 StoreKit을 링크하지 않음 | `if(IOS)`에 `-framework StoreKit` |
| 앱이 **가로로 실행됨** | cocos2d-x 기본 plist 템플릿이 가로 전용 | `proj.ios_mac/ios/kw4xBundleInfo.plist.in` 사본을 만들어 세로로 교체 |
| 기존 앱에 업로드 불가 / 인앱결제 상품 조회 안 됨 | 번들 ID가 기본값 `org.cocos2dx.kw4_x` | `com.ozzywow.kw4iphonelite` 로 지정 |
| 홈 화면 이름이 `kw4_x` | 템플릿에 `CFBundleDisplayName` 없음 | 추가 (한글떼기1단계) |
| Xcode가 배포 타깃 거부 | cocos2d-x 기본값 8.0 | 12.0 으로 상향 |

번들 ID 출처는 예전 `Resources/Info_lite.plist`입니다.
현재 빌드는 `LITE_VER` + `kw4iphonelite.*` 상품 ID이므로 **라이트판**에 해당합니다.
(유료 정식판은 `com.ozzywow.kw4iphone`, `Resources/Info.plist`)

### Mac에서 직접 해야 하는 것

1. **폰트 복사** — 0번 항목 참고. 안 하면 낱말 글자가 안 나옵니다.
2. **서명** — `CMakeLists.txt`의 `DEVELOPMENT_TEAM`이 비어 있습니다.
   Xcode에서 본인 팀 지정(자동 서명 권장).
3. **버전** — `CFBundleShortVersionString`이 기본값 `1.0`입니다.
   App Store 업데이트라면 게시된 버전보다 높여야 합니다.
   `CMakeLists.txt`의 `cocos_pak_xcode(... SHORT_VERSION_STRING ...)`로 지정할 수 있습니다.

### 알려진 경고 (빌드는 됨)

`Classes/MKStoreManager.mm`은 2010년 MKStoreKit 코드라 오래된 API를 씁니다.

- `UIAlertView` 3곳 — deprecated. 최신 iOS에서 표시가 보장되지 않습니다.
  결제 실패·IAP 비활성 알림 경로라 정상 흐름에는 영향이 적습니다.
  고치려면 `UIAlertController`로 옮겨야 하는데 표시할 뷰컨트롤러를 넘겨야 합니다.
- `NSURLConnection sendSynchronousRequest` 2곳 — deprecated.
  영수증 서버 검증 경로이며 `SERVER_PRODUCT_MODEL`은 `0`입니다.

### 건드리면 안 되는 파일

`Classes/MKStoreManager.m`, `Classes/MKStoreObserver.m` 은 `.mm` 버전과 중복된 레거시입니다.
빌드에 포함돼 있지 않습니다. **CMake 소스 목록에 추가하면 심볼 중복으로 링크가 깨집니다.**

---

## 4. 플랫폼 분기 요약

| 대상 | 인앱결제 | 공유 | 비고 |
|---|---|---|---|
| iOS | MKStoreKit (StoreKit) | `UIActivityViewController` | 유료 정식판 배너 표시(iOS 전용) |
| Android | Play Billing 7.0 (`AndroidBilling` JNI) | `Intent.ACTION_SEND` | 상품 1종(`.total`)으로 전체 해제 |
| Windows | 스텁 (동작 없음) | 브라우저로 링크 열기 | 개발/확인용 |

관련 매크로는 `Classes/common_define.h`에 모여 있습니다
(`LITE_VER`, 상품 ID, `BUY_AT_STORE_URL`, `SHARE_URL`).

---

## 5. 증상별 대처

| 증상 | 원인 | 해결 |
|---|---|---|
| `keystore password was incorrect` | 비밀번호에 한글 → Gradle이 ISO-8859-1로 오독 | ASCII 비밀번호 또는 `\uXXXX` |
| `Could not get unknown property 'RELEASE_STORE_PASSWORD'` | `gradle.properties`에 해당 줄 없음 | `tools/KeystorePwFix.java` 실행 |
| `unitTestClasses not found` | `Assemble Project with Tests` 메뉴 | `Assemble Project` 사용 |
| `Generate Signed...` 메뉴가 없음 | Gradle sync 실패 상태 | sync 후 재확인 |
| `JAVA_HOME is not set` | 터미널에 JDK 미지정 | Android Studio 번들 JBR 지정 |
| 이미지 교체가 반영 안 됨 | APK/실행 폴더가 옛 에셋 | 재빌드 후 재설치 |
| 낱말 글자가 안 보임 | `malgun.ttf` 없음 | 0번 항목 |
