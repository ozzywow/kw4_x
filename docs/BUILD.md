# 플랫폼별 빌드 안내

Windows(Visual Studio + CMake)에서 개발한 내용을 **Android Studio / Xcode**에서 빌드할 때
막히기 쉬운 지점을 정리한 문서입니다.

버전: cocos2d-x **3.17.2** / Gradle **8.7** / AGP **8.5.x** / NDK **30.0.14904198**

---

## 0. 저장소에 없는 것 — 먼저 챙길 것

`.gitignore`로 제외된 것들이라 클론만 해서는 빌드가 되지 않습니다.

| 대상 | 경로 | 제외 이유 | 준비 방법 |
|---|---|---|---|
| cocos2d-x 엔진 | `<repo>/cocos2d-x/` | 용량 | 3.17.2 를 이 경로에 배치. 최초 1회 `python download-deps.py` → **아래 패치 적용** |
| 한글 폰트 | `kw4_x/Resources/fonts/malgun.ttf` | MS 독점 폰트 | Windows `C:\Windows\Fonts\malgun.ttf` 복사 |
| Android 빌드 산출물 | `proj.android/app/{build,.cxx}` | 수 GB | 빌드하면 생성됨 |
| 로컬 SDK 경로 | `proj.android/local.properties` | 머신마다 다름 | Android Studio가 자동 생성 |

**폰트가 특히 중요합니다.** `StudyScene` · `TextLayer` · `TouchedHandleLayer`가
`KR_FONT_TTF`(= `fonts/malgun.ttf`)로 낱말 글자를 렌더하므로, 없으면 게임의 핵심 화면이
글자 없이 뜹니다. 메인메뉴 버튼 글자는 이미지에 구워져 있어 폰트 없이도 정상입니다.

경로 기준: `COCOS2DX_ROOT_PATH = kw4_x/../cocos2d-x` (`kw4_x/CMakeLists.txt:7`)

### 엔진 패치 (Android 빌드에 필수)

cocos2d-x 3.17.2 는 2018년 릴리스라 **최신 AGP/NDK 로는 Android 빌드가 되지 않습니다.**
필요한 수정을 `patches/cocos2d-x-3.17.2-android.patch` 로 보존해 두었습니다.

```bash
cd cocos2d-x            # 태그 cocos2d-x-3.17.2 체크아웃 상태여야 함
git apply ../patches/cocos2d-x-3.17.2-android.patch
```

패치 내용 (3개 파일):

| 파일 | 수정 | 없으면 |
|---|---|---|
| `cocos/platform/CMakeLists.txt` | Android 소스에 `CCDevice-android.cpp`, `jni/JniHelper.cpp` 추가 | JNI/디바이스 심볼 undefined |
| `cocos/platform/android/libcocos2dx/build.gradle` | AGP 8 문법 (`namespace`, `compileSdk`, `minSdk`) | Gradle 구성 실패 |
| `.../lib/Cocos2dxHelper.java` | 존재하지 않는 `com.enhance.gameservice` 참조 제거 | Java 컴파일 실패 |

**iOS 빌드에는 영향이 없습니다.** 세 수정 모두 Android 전용 경로이고,
`cocos/platform/CMakeLists.txt` 변경도 `if(ANDROID)` 블록 안입니다.

> 엔진은 별도 저장소(upstream `cocos2d/cocos2d-x`)를 detached HEAD 로 체크아웃한 것이라
> 이 수정을 커밋해 둘 곳이 없습니다. 엔진 폴더를 다시 받거나 `git checkout .` 하면 사라지므로,
> **변경이 생기면 패치를 다시 뽑아 두세요.**
>
> ```powershell
> git -C cocos2d-x diff > patches/cocos2d-x-3.17.2-android.patch
> ```

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

### 빌드 경로: Mac 에 있는 기존 Xcode 프로젝트

iOS 는 **Mac 에 있는 기존 `.xcodeproj` 로 빌드합니다.** App Store 출시까지 그 프로젝트로
해 왔습니다.

> **그 프로젝트는 이 저장소에 없습니다.**
> `.xcodeproj` 뿐 아니라 iOS 앱 셸 소스(`main.m`, `AppController.mm`,
> `RootViewController.mm`, `Prefix.pch`, `LaunchScreen.storyboard`, `Images.xcassets`)도
> 추적되지 않습니다. 추적 중인 것은 아이콘·런치 이미지 24개뿐입니다.
> **즉 그 Mac 이 iOS 빌드 환경의 유일한 사본입니다.** 잃어버리면 다시 만들어야 하므로
> 여유가 될 때 커밋해 두는 것을 권합니다 (Mac 에서 `git status` 로 확인).

따라서 빌드 설정(소스 목록, StoreKit 링크, 방향, 번들 ID, 배포 타깃, 서명)은
**모두 그 Xcode 프로젝트가 갖고 있습니다.** 이미 출시에 성공했으므로 그대로 두면 됩니다.

### Windows 에서 새 코드를 받은 뒤 해야 할 일

`git pull` 후, **이번에 추가된 파일을 Xcode 타깃에 직접 넣어야 합니다.**
Xcode 프로젝트는 자체 소스 목록을 갖고 있어 자동으로 따라오지 않습니다.

| 항목 | 할 일 | 안 하면 |
|---|---|---|
| `Classes/NativeShare.h` | 타깃에 추가 | — |
| `Classes/NativeShare.cpp` | 타깃에 추가 | — |
| `Classes/NativeShare_apple.mm` | **타깃에 추가 (필수)** | `NativeShare::share` undefined 로 링크 실패 |
| `Resources/UI4HD/shareBtn_n-hd.png`<br>`Resources/UI4HD/shareBtn_s-hd.png` | 리소스가 **폴더 참조(파란 폴더)** 면 자동. 개별 파일 참조면 추가 | 공유 버튼이 안 보임 |

`MainMenuScene::cfShare()` 가 `NativeShare::share()` 를 호출하는데 iOS 구현이
`NativeShare_apple.mm` 에만 있습니다 (`NativeShare.cpp` 의 Apple 분기는 비어 있음).

기존 버튼 이미지 6개(`startBtn`/`infoBtn`/`treeBtn` × n/s)는 **파일명이 그대로**라
따로 추가할 필요 없이 교체된 그림이 반영됩니다.

### CMake 경로 (참고용, 현재 사용하지 않음)

`kw4_x/CMakeLists.txt` 에도 iOS 설정이 있고 2026-07 에 여러 문제를 고쳐 두었습니다
(커밋 `2828eb2`). 다만 위에 적었듯 **앱 셸 소스가 저장소에 없어 이 경로는 지금 그대로는
구성되지 않습니다.** 나중에 CMake 로 옮기거나 프로젝트를 새로 만들 때 참고하세요.

| 문제 | 조치 |
|---|---|
| `MKStoreManager.mm` 이 소스 목록에 없어 링크 실패 | 목록에 추가 |
| StoreKit 미링크 (`_OBJC_CLASS_$_SKPaymentQueue`) | `if(IOS)` 에 `-framework StoreKit` |
| cocos2d-x 기본 plist 템플릿이 **가로 전용** | `proj.ios_mac/ios/kw4xBundleInfo.plist.in` 사본을 세로로 |
| 번들 ID 기본값 `org.cocos2dx.kw4_x` | `com.ozzywow.kw4iphonelite` 로 지정 |
| 홈 화면 이름이 `kw4_x` | `CFBundleDisplayName` 추가 |
| 배포 타깃 8.0 (최신 Xcode 미지원) | 12.0 |

번들 ID 출처는 예전 `Resources/Info_lite.plist` 입니다.
현재 빌드는 `LITE_VER` + `kw4iphonelite.*` 상품 ID 이므로 **라이트판**에 해당합니다.
(유료 정식판은 `com.ozzywow.kw4iphone`, `Resources/Info.plist`)

### 그 밖에 확인할 것

1. **폰트** — 0번 항목. Mac 에 이미 있다면 그대로 두면 됩니다.
2. **버전** — App Store 업데이트라면 `CFBundleShortVersionString` 을 게시된 버전보다 높일 것.

### 알려진 경고 (빌드는 됨)

`Classes/MKStoreManager.mm`은 2010년 MKStoreKit 코드라 오래된 API를 씁니다.

- `UIAlertView` 3곳 — deprecated. 최신 iOS에서 표시가 보장되지 않습니다.
  결제 실패·IAP 비활성 알림 경로라 정상 흐름에는 영향이 적습니다.
  고치려면 `UIAlertController`로 옮겨야 하는데 표시할 뷰컨트롤러를 넘겨야 합니다.
- `NSURLConnection sendSynchronousRequest` 2곳 — deprecated.
  영수증 서버 검증 경로이며 `SERVER_PRODUCT_MODEL`은 `0`입니다.

### 건드리면 안 되는 파일

`Classes/MKStoreManager.m`, `Classes/MKStoreObserver.m` 은 `.mm` 버전과 중복된 레거시입니다.
**Xcode 타깃이나 CMake 목록에 추가하면 심볼 중복으로 링크가 깨집니다.**
새 파일을 타깃에 넣을 때 이 둘이 딸려 들어가지 않도록 주의하세요.

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
| `com.enhance.gameservice` 를 찾을 수 없음 | 엔진 패치 미적용 | `patches/` 의 엔진 패치 적용 |
| `namespace` / `compileSdkVersion` Gradle 오류 | 엔진 패치 미적용 (AGP 8 문법) | 위와 동일 |
| `JniHelper` / `CCDevice` 심볼 undefined | 엔진 패치 미적용 | 위와 동일 |
| iOS 링크 실패 `NativeShare::share` undefined | `NativeShare_apple.mm` 이 Xcode 타깃에 없음 | 3번 항목 표 참고 |
| iOS 에서 공유 버튼이 안 보임 | `shareBtn_*.png` 가 번들에 없음 | 리소스 참조 방식 확인 |
| iOS 링크 실패 `duplicate symbol MKStoreManager` | 레거시 `.m` 파일이 타깃에 들어감 | `.mm` 만 남길 것 |
