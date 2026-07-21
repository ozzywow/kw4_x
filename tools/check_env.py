#!/usr/bin/env python3
"""빌드 전 환경 점검.

git pull 후 빌드가 깨지는 원인 대부분은 "저장소에 없는 것"과
"새 파일이 빌드 목록에 안 들어간 것" 두 가지다. 그걸 미리 잡아준다.

    python tools/check_env.py            # 전체 점검
    python tools/check_env.py --ios      # iOS 항목까지 (Mac)

비밀번호 값은 출력하지 않는다. 실패가 있으면 종료코드 1.
"""

import argparse
import os
import re
import sys

# Windows 콘솔 코드페이지가 949 여도 한글이 깨지지 않도록
try:
    sys.stdout.reconfigure(encoding="utf-8")
except Exception:
    pass

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PROJ = os.path.join(ROOT, "kw4_x")
ENGINE = os.path.join(ROOT, "cocos2d-x")

OK, WARN, FAIL = "OK  ", "주의", "실패"
results = []


def report(level, title, detail=""):
    results.append((level, title, detail))


def read(path, encoding="utf-8", errors="ignore"):
    try:
        with open(path, encoding=encoding, errors=errors) as f:
            return f.read()
    except OSError:
        return None


# ── 1. 엔진 ────────────────────────────────────────────────────
def check_engine():
    header = os.path.join(ENGINE, "cocos", "cocos2d.h")
    if not os.path.isfile(header):
        report(FAIL, "cocos2d-x 엔진 없음",
               f"{ENGINE} 에 3.17.2 를 배치하고 download-deps.py 실행")
        return
    ver = read(header) or ""
    m = re.search(r"COCOS2D_VERSION\s+(0x[0-9a-fA-F]+)", ver)
    if m and m.group(1).lower() != "0x00031702":
        report(WARN, f"엔진 버전이 3.17.2 가 아님 ({m.group(1)})")
    else:
        report(OK, "cocos2d-x 3.17.2")

    if not os.path.isdir(os.path.join(ENGINE, "external")):
        report(FAIL, "엔진 external 없음", "cocos2d-x 에서 python download-deps.py 실행")


def check_engine_patch():
    """patches/ 의 Android 패치가 적용됐는지 결과물로 판단한다."""
    checks = [
        (os.path.join(ENGINE, "cocos", "platform", "CMakeLists.txt"),
         "platform/android/jni/JniHelper.cpp", True,
         "Android 소스에 JniHelper.cpp 누락 → 심볼 undefined"),
        (os.path.join(ENGINE, "cocos", "platform", "android", "libcocos2dx", "build.gradle"),
         'namespace "org.cocos2dx.lib"', True,
         "AGP 8 문법 미적용 → Gradle 구성 실패"),
        (os.path.join(ENGINE, "cocos", "platform", "android", "java", "src",
                      "org", "cocos2dx", "lib", "Cocos2dxHelper.java"),
         "com.enhance.gameservice", False,
         "존재하지 않는 gameservice 참조 → Java 컴파일 실패"),
    ]
    missing = []
    for path, needle, want_present, why in checks:
        text = read(path)
        if text is None:
            continue
        if (needle in text) != want_present:
            missing.append(why)
    if missing:
        report(FAIL, "엔진 패치 미적용",
               "cd cocos2d-x && git apply ../patches/cocos2d-x-3.17.2-android.patch\n"
               + "\n".join("       - " + m for m in missing))
    elif os.path.isdir(ENGINE):
        report(OK, "엔진 Android 패치 적용됨")


# ── 2. 리소스 ──────────────────────────────────────────────────
def check_font():
    font = os.path.join(PROJ, "Resources", "fonts", "malgun.ttf")
    if not os.path.isfile(font):
        report(FAIL, "malgun.ttf 없음",
               "낱말 글자가 렌더되지 않는다. Windows C:/Windows/Fonts/malgun.ttf 복사")
    elif os.path.getsize(font) < 1_000_000:
        report(WARN, "malgun.ttf 크기가 비정상적으로 작음")
    else:
        report(OK, "한글 폰트")


# ── 3. 소스가 빌드 목록에 들어갔는지 ───────────────────────────
def check_sources_registered():
    """Classes/*.cpp 중 CMakeLists 에 없는 것을 찾는다.

    플랫폼 전용 파일(.mm/.m)과 CMake 가 조건부로 넣는 것은 제외.
    """
    cml = read(os.path.join(PROJ, "CMakeLists.txt"))
    if cml is None:
        report(FAIL, "kw4_x/CMakeLists.txt 를 읽을 수 없음")
        return
    classes = os.path.join(PROJ, "Classes")
    missing = [f for f in sorted(os.listdir(classes))
               if f.endswith(".cpp") and f not in cml]
    if missing:
        report(FAIL, "CMakeLists 에 등록되지 않은 소스",
               "GAME_SOURCE 에 추가할 것: " + ", ".join(missing))
    else:
        report(OK, "모든 .cpp 가 CMakeLists 에 등록됨")


# ── 4. Android 서명 ────────────────────────────────────────────
def check_signing():
    props = os.path.expanduser(os.path.join("~", ".gradle", "gradle.properties"))
    text = read(props, encoding="utf-8")
    if text is None:
        report(WARN, "~/.gradle/gradle.properties 없음", "릴리스 서명 시에만 필요")
        return

    values = {}
    for line in text.splitlines():
        m = re.match(r"\s*(RELEASE_[A-Z_]+)\s*=\s*(.*)$", line)
        if m:
            values[m.group(1)] = m.group(2)

    need = ["RELEASE_STORE_FILE", "RELEASE_KEY_ALIAS",
            "RELEASE_STORE_PASSWORD", "RELEASE_KEY_PASSWORD"]
    absent = [k for k in need if k not in values]
    if absent:
        report(FAIL, "서명 속성 누락: " + ", ".join(absent),
               "tools/KeystorePwFix.java 로 채울 것. 가드가 STORE_FILE 만 보므로\n"
               "       비밀번호만 빠지면 '서명 생략' 이 아니라 평가 예외로 터진다")
        return

    # 값은 출력하지 않고 성질만 본다
    bad = [k for k in ("RELEASE_STORE_PASSWORD", "RELEASE_KEY_PASSWORD")
           if any(ord(c) > 127 for c in values[k])]
    if bad:
        report(FAIL, "서명 비밀번호에 비ASCII 문자: " + ", ".join(bad),
               "gradle.properties 는 ISO-8859-1 로 읽혀 한글이 깨진다.\n"
               "       ASCII 로 바꾸거나 \\uXXXX 이스케이프로 적을 것")
    else:
        report(OK, "서명 속성 (비밀번호 ASCII)")

    store = values["RELEASE_STORE_FILE"]
    if not os.path.isfile(store):
        report(FAIL, "키스토어 파일 없음", store)


def check_gradle_wrapper():
    jar = os.path.join(PROJ, "proj.android", "gradle", "wrapper", "gradle-wrapper.jar")
    if not os.path.isfile(jar):
        report(WARN, "gradle-wrapper.jar 없음",
               "터미널에서 ./gradlew 사용 불가 (Android Studio 는 영향 없음)")


# ── 5. iOS ─────────────────────────────────────────────────────
def check_ios():
    classes = os.path.join(PROJ, "Classes")
    mm = sorted(f for f in os.listdir(classes) if f.endswith(".mm"))
    report(WARN, "iOS 는 Xcode 프로젝트가 저장소에 없음",
           "새 파일은 Xcode 타깃에 직접 추가해야 한다. 현재 .mm 목록:\n       "
           + ", ".join(mm) + "\n       레거시 MKStoreManager.m / MKStoreObserver.m 은"
           " 타깃에 넣지 말 것 (심볼 중복)")

    legacy = [f for f in os.listdir(classes) if f.endswith(".m")]
    if legacy:
        report(WARN, "레거시 .m 파일 존재: " + ", ".join(sorted(legacy)),
               ".mm 버전과 중복. 타깃에 들어가면 duplicate symbol")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--ios", action="store_true", help="iOS 항목까지 점검")
    args = ap.parse_args()

    check_engine()
    check_engine_patch()
    check_font()
    check_sources_registered()
    check_signing()
    check_gradle_wrapper()
    if args.ios:
        check_ios()

    print()
    for level, title, detail in results:
        print(f"[{level}] {title}")
        if detail:
            for line in detail.splitlines():
                print(f"       {line}")
    n_fail = sum(1 for l, _, _ in results if l == FAIL)
    n_warn = sum(1 for l, _, _ in results if l == WARN)
    print(f"\n실패 {n_fail} / 주의 {n_warn} / 전체 {len(results)}")
    return 1 if n_fail else 0


if __name__ == "__main__":
    sys.exit(main())
