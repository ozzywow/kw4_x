#!/bin/bash
# setup_builds.sh
# iOS Xcode 프로젝트 생성 스크립트
# - 유료(kw4)·무료(kw4_lite) 두 실행 타깃을 한 프로젝트에 생성한다.
# - Xcode 상단 스킴 선택으로 버전을 전환한다(스킴은 Xcode 가 타깃별로 자동 생성).
#
# 사용법: ./setup_builds.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build_ios"

echo "=== iOS Xcode 프로젝트 생성 중 ==="
mkdir -p "$BUILD_DIR"
cmake -GXcode \
    -DIOS=TRUE \
    -DCMAKE_OSX_SYSROOT=iphoneos \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
    -S "$SCRIPT_DIR" \
    -B "$BUILD_DIR"

# CMake Xcode 제너레이터는 xcassets/storyboard 를 PBXCopyFilesBuildPhase 에 넣지만
# actool/ibtool 이 실행되려면 PBXResourcesBuildPhase 여야 한다.
# 두 앱 타깃의 Resources 페이즈를 올바른 타입으로 교체한다.
echo "=== xcassets/storyboard 빌드 페이즈 패치 중 ==="
python3 - "$BUILD_DIR/kw4_x.xcodeproj/project.pbxproj" <<'PYEOF'
import re, sys

pbxproj = sys.argv[1]
with open(pbxproj, "r") as f:
    content = f.read()

changed = 0
# kw4/kw4_lite Resources 페이즈: dstSubfolderSpec=6, dstPath="" 인 CopyFiles 페이즈를
# xcassets 가 포함된 경우에만 ResourcesBuildPhase 로 변환한다.
pattern = (
    r'(\w{24} /\* (?:Copy files|Resources) \*/ = \{\n'
    r'\t\t\tisa = PBXCopyFilesBuildPhase;\n'
    r'\t\t\tbuildActionMask = 2147483647;\n'
    r'\t\t\tdstPath = "";\n'
    r'\t\t\tdstSubfolderSpec = 6;\n'
    r'\t\t\tfiles = \([^)]*Images\.xcassets[^)]*\);\n'
    r'\t\t\trunOnlyForDeploymentPostprocessing = 0;\n'
    r'\t\t\};)'
)

def replace_phase(m):
    block = m.group(1)
    uuid_comment = re.match(r'(\w{24} /\* )(?:Copy files|Resources)( \*/ = \{)', block).group(0)
    block = block.replace(
        uuid_comment,
        uuid_comment.replace('Copy files', 'Resources').replace('Resources', 'Resources')
    )
    block = re.sub(r'\tisa = PBXCopyFilesBuildPhase;', '\tisa = PBXResourcesBuildPhase;', block)
    block = re.sub(r'\tdstPath = "";\n', '', block)
    block = re.sub(r'\tdstSubfolderSpec = 6;\n', '', block)
    return block

new_content = re.sub(pattern, replace_phase, content)
if new_content != content:
    with open(pbxproj, "w") as f:
        f.write(new_content)
    print("  패치 완료: PBXCopyFilesBuildPhase → PBXResourcesBuildPhase")
else:
    print("  (이미 패치됨 또는 불필요)")
PYEOF

echo ""
echo "=== 완료 ==="
echo "Xcode에서 열기: open $BUILD_DIR/kw4_x.xcodeproj"
echo ""
echo "스킴 선택:"
echo "  kw4      → 정식(유료) 버전 (Bundle ID: com.ozzywow.kw4iphone,  KW4_PAID 정의)"
echo "  kw4_lite → 무료(라이트) 버전 (Bundle ID: com.ozzywow.kw4iphonelite, LITE_VER 정의)"
